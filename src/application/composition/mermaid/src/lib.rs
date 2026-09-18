use std::{
    ffi::c_void,
    panic::{AssertUnwindSafe, catch_unwind},
};
#[repr(C)]
pub struct ResultBuffer {
    pub abi_version: u32,
    pub struct_size: u32,
    pub status: u32,
    pub data: *const u8,
    pub size: u64,
    pub owner: *mut c_void,
}
fn owned(status: u32, data: Vec<u8>) -> ResultBuffer {
    let value = Box::new(data);
    let ptr = value.as_ptr();
    let size = value.len() as u64;
    ResultBuffer {
        abi_version: 1,
        struct_size: std::mem::size_of::<ResultBuffer>() as u32,
        status,
        data: ptr,
        size,
        owner: Box::into_raw(value).cast(),
    }
}
unsafe fn invoke(
    abi: u32,
    data: *const u8,
    size: u64,
    operation: impl FnOnce(&[u8]) -> Result<Vec<u8>, String>,
) -> ResultBuffer {
    if abi != 1 || size > 1024 * 1024 || (size > 0 && data.is_null()) {
        return owned(4, b"Invalid diagram ABI/input".to_vec());
    }
    let bytes = if size == 0 {
        &[]
    } else {
        unsafe { std::slice::from_raw_parts(data, size as usize) }
    };
    match catch_unwind(AssertUnwindSafe(|| operation(bytes))) {
        Ok(Ok(v)) => owned(0, v),
        Ok(Err(e)) => owned(1, e.into_bytes()),
        Err(payload) => {
            let deadline = "Diagram layout time budget exceeded";
            if payload.downcast_ref::<&str>() == Some(&deadline) {
                owned(1, deadline.as_bytes().to_vec())
            } else {
                owned(7, b"Diagram engine panic".to_vec())
            }
        }
    }
}
/// # Safety
/// `data` must remain readable for `size` bytes until this call returns.
/// A null data pointer is permitted only with size zero. Free the owned result
/// once using the matching exported free function; never free `data` directly.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_mermaid_parse_v1(
    abi: u32,
    data: *const u8,
    size: u64,
) -> ResultBuffer {
    unsafe { invoke(abi, data, size, xfmd_mermaid_interpreter::parse) }
}
/// # Safety
/// `data` must remain readable for `size` bytes until this call returns.
/// A null data pointer is permitted only with size zero. Free the owned result
/// once using the matching exported free function; never free `data` directly.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_diagram_layout_v1(
    abi: u32,
    data: *const u8,
    size: u64,
) -> ResultBuffer {
    unsafe { invoke(abi, data, size, xfmd_diagram_layout::layout) }
}
/// # Safety
/// Input and callback context must remain valid until return. The synchronous
/// callback must not unwind or retain borrowed input/output pointers.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_diagram_layout_measured_v1(
    abi: u32,
    data: *const u8,
    size: u64,
    context: *mut c_void,
    callback: Option<xfmd_diagram_layout::text_metrics::Callback>,
) -> ResultBuffer {
    let Some(callback) = callback else {
        return owned(4, b"Missing text measurer".to_vec());
    };
    let metrics = xfmd_diagram_layout::text_metrics::Metrics { context, callback };
    unsafe {
        invoke(abi, data, size, |bytes| {
            xfmd_diagram_layout::layout_measured(bytes, Some(metrics))
        })
    }
}
unsafe fn release(result: *mut ResultBuffer) {
    if let Some(r) = unsafe { result.as_mut() } {
        if !r.owner.is_null() {
            drop(unsafe { Box::from_raw(r.owner.cast::<Vec<u8>>()) });
            r.owner = std::ptr::null_mut();
            r.data = std::ptr::null();
            r.size = 0;
        }
    }
}
/// # Safety
/// `r` is null or points to a writable result returned by the matching call.
/// Do not copy ownership or release a copied result. Releasing the same cleared
/// result again is allowed; this call clears its data, size and owner.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_mermaid_parse_free_v1(r: *mut ResultBuffer) {
    unsafe { release(r) }
}
/// # Safety
/// `r` is null or points to a writable result returned by the matching call.
/// Do not copy ownership or release a copied result. Releasing the same cleared
/// result again is allowed; this call clears its data, size and owner.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_diagram_layout_free_v1(r: *mut ResultBuffer) {
    unsafe { release(r) }
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn abi_bounds_and_ownership() {
        unsafe {
            let mut bad = xfmd_mermaid_parse_v1(2, std::ptr::null(), 0);
            assert_eq!(bad.status, 4);
            xfmd_mermaid_parse_free_v1(&mut bad);
            assert!(bad.owner.is_null());
            xfmd_mermaid_parse_free_v1(&mut bad);
            let mut bad = xfmd_diagram_layout_v1(1, std::ptr::null(), 1);
            assert_eq!(bad.status, 4);
            xfmd_diagram_layout_free_v1(&mut bad);
            let text = b"flowchart LR\nA-->B";
            for _ in 0..100 {
                let mut value = xfmd_mermaid_parse_v1(1, text.as_ptr(), text.len() as u64);
                assert_eq!(value.status, 0);
                assert!(value.size > 0);
                xfmd_mermaid_parse_free_v1(&mut value);
            }
        }
    }
    #[test]
    fn panic_is_contained() {
        fn fail(_: &[u8]) -> Result<Vec<u8>, String> {
            panic!("injected panic")
        }
        unsafe {
            let mut value = invoke(1, std::ptr::null(), 0, fail);
            assert_eq!(value.status, 7);
            xfmd_mermaid_parse_free_v1(&mut value);
        }
    }
    #[test]
    fn malformed_layout_records() {
        let mut seed = 7u64;
        for n in 0..512 {
            let mut bytes = Vec::new();
            for _ in 0..n {
                seed = seed.wrapping_mul(6364136223846793005).wrapping_add(1);
                bytes.push((seed >> 32) as u8);
            }
            unsafe {
                let mut value = xfmd_diagram_layout_v1(1, bytes.as_ptr(), bytes.len() as u64);
                assert_ne!(value.status, 0);
                xfmd_diagram_layout_free_v1(&mut value);
            }
        }
    }
}
