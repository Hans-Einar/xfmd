//! Application-local JSON decoding. Borrowed values live until the root is freed.
use serde_json::Value;
use std::{ffi::c_char, ptr, slice};

#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_parse(data: *const u8, len: usize) -> *mut Value {
    if data.is_null() || len > 32 * 1024 * 1024 {
        return ptr::null_mut();
    }
    std::panic::catch_unwind(|| {
        let bytes = unsafe { slice::from_raw_parts(data, len) };
        serde_json::from_slice::<Value>(bytes)
            .ok()
            .map_or(ptr::null_mut(), |v| Box::into_raw(Box::new(v)))
    })
    .unwrap_or(ptr::null_mut())
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_free(value: *mut Value) {
    if !value.is_null() {
        drop(unsafe { Box::from_raw(value) });
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_get(
    value: *const Value,
    key: *const u8,
    len: usize,
) -> *const Value {
    let Some(v) = (unsafe { value.as_ref() }) else {
        return ptr::null();
    };
    if key.is_null() {
        return ptr::null();
    }
    let Ok(key) = std::str::from_utf8(unsafe { slice::from_raw_parts(key, len) }) else {
        return ptr::null();
    };
    v.get(key).map_or(ptr::null(), |v| v)
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_at(value: *const Value, index: usize) -> *const Value {
    (unsafe { value.as_ref() })
        .and_then(|v| v.as_array())
        .and_then(|a| a.get(index))
        .map_or(ptr::null(), |v| v)
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_size(value: *const Value) -> usize {
    (unsafe { value.as_ref() })
        .and_then(|v| v.as_array())
        .map_or(usize::MAX, |v| v.len())
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_unsigned(value: *const Value, result: *mut u64) -> bool {
    if result.is_null() {
        return false;
    }
    let Some(number) = (unsafe { value.as_ref() }).and_then(|v| v.as_u64()) else {
        return false;
    };
    unsafe {
        *result = number;
    }
    true
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_text(value: *const Value, len: *mut usize) -> *const c_char {
    if len.is_null() {
        return ptr::null();
    }
    let Some(v) = (unsafe { value.as_ref() }).and_then(|v| v.as_str()) else {
        return ptr::null();
    };
    unsafe {
        *len = v.len();
    }
    v.as_ptr().cast()
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_sdp_json_boolean(value: *const Value, result: *mut bool) -> bool {
    if result.is_null() { return false; }
    let Some(v) = (unsafe { value.as_ref() }).and_then(|v| v.as_bool()) else { return false; };
    unsafe { *result = v; }
    true
}
