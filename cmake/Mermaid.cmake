find_package(Python3 3.11 REQUIRED COMPONENTS Interpreter)
find_program(CARGO_EXECUTABLE cargo REQUIRED)
find_program(RUSTC_EXECUTABLE rustc REQUIRED)
execute_process(COMMAND ${RUSTC_EXECUTABLE} --version OUTPUT_VARIABLE XFMD_RUST_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ERROR_IS_FATAL ANY)
if(NOT XFMD_RUST_VERSION MATCHES "^rustc 1\\.92\\.0 ")
  message(FATAL_ERROR "xfmd requires Rust 1.92.0 (see rust-toolchain.toml); found ${XFMD_RUST_VERSION}")
endif()
execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/bootstrap_mermaid.py COMMAND_ERROR_IS_FATAL ANY)
set(XFMD_CARGO_TARGET "${CMAKE_BINARY_DIR}/cargo" CACHE PATH "Cargo build artifacts")
file(GLOB_RECURSE XFMD_RUST_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.rs" "${CMAKE_SOURCE_DIR}/src/*/Cargo.toml")
set(XFMD_RUST_ARCHIVE "${XFMD_CARGO_TARGET}/release/libxfmd_mermaid_ffi.a")
add_custom_command(OUTPUT "${XFMD_RUST_ARCHIVE}"
  COMMAND ${CARGO_EXECUTABLE} build --release --locked -p xfmd-mermaid-ffi --target-dir "${XFMD_CARGO_TARGET}"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  DEPENDS ${XFMD_RUST_SOURCES} "${CMAKE_SOURCE_DIR}/Cargo.toml" "${CMAKE_SOURCE_DIR}/Cargo.lock" "${CMAKE_SOURCE_DIR}/cmake/patches/mermaid-measurements.patch"
  VERBATIM)
add_custom_target(xfmd_rust_build DEPENDS "${XFMD_RUST_ARCHIVE}")
add_library(xfmd_rust STATIC IMPORTED GLOBAL)
set_target_properties(xfmd_rust PROPERTIES IMPORTED_LOCATION "${XFMD_RUST_ARCHIVE}" INTERFACE_LINK_LIBRARIES "Threads::Threads;${CMAKE_DL_LIBS};m")
add_dependencies(xfmd_rust xfmd_rust_build)
