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
    operation: fn(&[u8]) -> Result<Vec<u8>, String>,
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
        Err(_) => owned(7, b"Diagram engine panic".to_vec()),
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_mermaid_parse_v1(
    abi: u32,
    data: *const u8,
    size: u64,
) -> ResultBuffer {
    unsafe { invoke(abi, data, size, xfmd_mermaid_interpreter::parse) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_diagram_layout_v1(
    abi: u32,
    data: *const u8,
    size: u64,
) -> ResultBuffer {
    unsafe { invoke(abi, data, size, xfmd_diagram_layout::layout) }
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
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_mermaid_parse_free_v1(r: *mut ResultBuffer) {
    unsafe { release(r) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn xfmd_diagram_layout_free_v1(r: *mut ResultBuffer) {
    unsafe { release(r) }
}
