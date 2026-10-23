# SDP transport decoding belongs to application, independently of Mermaid.
set(XFMD_SDP_ARCHIVE "${XFMD_CARGO_TARGET}/release/libxfmd_sdp_json.a")
add_custom_command(OUTPUT "${XFMD_SDP_ARCHIVE}"
  COMMAND ${CARGO_EXECUTABLE} build --release --locked --offline -p xfmd-sdp-json --target-dir "${XFMD_CARGO_TARGET}"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  DEPENDS src/application/sdp/rust/src/lib.rs src/application/sdp/rust/Cargo.toml Cargo.lock
  VERBATIM)
add_custom_target(xfmd_sdp_rust_build DEPENDS "${XFMD_SDP_ARCHIVE}")
add_library(xfmd_sdp_json STATIC IMPORTED GLOBAL)
set_target_properties(xfmd_sdp_json PROPERTIES IMPORTED_LOCATION "${XFMD_SDP_ARCHIVE}" INTERFACE_LINK_LIBRARIES "Threads::Threads;${CMAKE_DL_LIBS};m")
add_dependencies(xfmd_sdp_json xfmd_sdp_rust_build)
add_library(xfmd_sdp src/application/sdp/SduiTools.cpp src/application/sdp/SdpDisplay.cpp src/application/sdp/SdpRuntime.cpp src/application/sdp/SdpProtocol.cpp src/application/sdp/SdpExpansion.cpp src/application/sdp/SdpSources.cpp src/application/sdp/SdpToolJob.cpp src/application/sdp/SdpWatch.cpp)
target_include_directories(xfmd_sdp PUBLIC src)
target_link_libraries(xfmd_sdp PRIVATE xfmd_sdp_json PUBLIC Threads::Threads)

# Optional at development time; release 0.7 supplies both SDUI-owned binaries.
set(XFMD_SDUI_TOOLS_DIR "${CMAKE_CURRENT_BINARY_DIR}/libexec/xfmd" CACHE PATH "Built SDUI preview and Fyne helpers")
install(PROGRAMS "${XFMD_SDUI_TOOLS_DIR}/sdui-preview" "${XFMD_SDUI_TOOLS_DIR}/sdui-fyne" DESTINATION libexec/xfmd OPTIONAL)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/LICENSES/go/" DESTINATION "${CMAKE_INSTALL_DOCDIR}/licenses/go")

# Optional for ordinary development; local 0.9 delivery includes its exact producer.
install(PROGRAMS "${XFMD_SDUI_TOOLS_DIR}/sdptool" DESTINATION libexec/xfmd OPTIONAL)
