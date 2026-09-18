use super::{ResultBuffer, owned, release};
use std::{
    ffi::c_void,
    panic::{AssertUnwindSafe, catch_unwind},
};
type Measure =
    unsafe extern "C" fn(*mut c_void, *const u8, u64, f64, *mut f64, *mut f64, *mut f64) -> u32;
type Cancel = unsafe extern "C" fn(*mut c_void) -> u32;
fn failure(status: u32, message: &str) -> ResultBuffer {
    let diagnostic = serde_json::from_str::<serde_json::Value>(message)
        .ok()
        .filter(|d| d.get("code").is_some() && d.get("message").is_some())
        .unwrap_or_else(
            || serde_json::json!({"code":"boxui-error","severity":"error","message":message}),
        );
    let code = diagnostic["code"].as_str().unwrap_or("");
    let status = if code == "cancelled" {
        4
    } else if code.contains("budget") || code.contains("limit") {
        3
    } else if code.contains("version") || code.contains("profile") || code.contains("contract") {
        2
    } else {
        status
    };
    owned(
        status,
        serde_json::to_vec(
            &serde_json::json!({"contract":"BX-HOST/0.1-draft1","diagnostics":[diagnostic]}),
        )
        .unwrap(),
    )
}
unsafe fn call(
    abi: u32,
    data: *const u8,
    size: u64,
    limit: u64,
    op: impl FnOnce(&[u8]) -> Result<Vec<u8>, String>,
) -> ResultBuffer {
    if abi != 1 {
        return failure(2, "Unsupported BoxUI ABI version");
    }
    if size > limit {
        return failure(3, "BoxUI input byte budget exceeded");
    }
    if size > 0 && data.is_null() {
        return failure(1, "Invalid BoxUI input pointer");
    }
    let bytes = if size == 0 {
        &[]
    } else {
        unsafe { std::slice::from_raw_parts(data, size as usize) }
    };
    match catch_unwind(AssertUnwindSafe(|| op(bytes))) {
        Ok(Ok(v)) => owned(0, v),
        Ok(Err(e)) => failure(
            if e.contains("cancelled") {
                4
            } else if e.contains("budget") || e.contains("exceeds") {
                3
            } else {
                1
            },
            &e,
        ),
        Err(_) => failure(5, "BoxUI engine panic"),
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_boxui_parse_v1(abi: u32, data: *const u8, size: u64) -> ResultBuffer {
    unsafe {
        call(
            abi,
            data,
            size,
            262144,
            xfmd_mermaid_interpreter::boxui::parse,
        )
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_boxui_prepare_v1(
    abi: u32,
    data: *const u8,
    size: u64,
    context: *mut c_void,
    measure: Option<Measure>,
    cancel: Option<Cancel>,
) -> ResultBuffer {
    let (Some(measure), Some(cancel)) = (measure, cancel) else {
        return failure(1, "Missing callbacks");
    };
    let metrics = |text: &str| {
        let (mut w, mut h, mut baseline) = (0., 0., 0.);
        let ok = unsafe {
            measure(
                context,
                text.as_ptr(),
                text.len() as u64,
                16.,
                &mut w,
                &mut h,
                &mut baseline,
            )
        };
        if ok == 0 {
            (w, h, baseline)
        } else {
            (f64::NAN, f64::NAN, f64::NAN)
        }
    };
    let cancelled = || unsafe { cancel(context) != 0 };
    unsafe {
        call(abi, data, size, 8 * 1024 * 1024, |b| {
            xfmd_diagram_layout::boxui::prepare(b, &metrics, &cancelled)
        })
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_boxui_result_free_v1(r: *mut ResultBuffer) {
    unsafe { release(r) }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn bounds_and_owned_errors() {
        unsafe {
            for (abi, size) in [(2, 0), (1, 1), (1, 262145)] {
                let mut r = xfmd_boxui_parse_v1(abi, std::ptr::null(), size);
                assert_ne!(r.status, 0);
                assert!(!r.owner.is_null());
                xfmd_boxui_result_free_v1(&mut r);
                assert!(r.owner.is_null());
                xfmd_boxui_result_free_v1(&mut r);
            }
            let source = b"boxui 0.1\n{}";
            let mut r = xfmd_boxui_parse_v1(1, source.as_ptr(), source.len() as u64);
            assert_eq!(r.status, 1);
            xfmd_boxui_result_free_v1(&mut r);
        }
    }
    #[test]
    fn panic_stays_inside_abi() {
        unsafe {
            let mut r = call(1, std::ptr::null(), 0, 1, |_| panic!("injected"));
            assert_eq!(r.status, 5);
            xfmd_boxui_result_free_v1(&mut r);
        }
    }
}
