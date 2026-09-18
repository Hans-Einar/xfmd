find_package(Python3 3.11 REQUIRED COMPONENTS Interpreter)
set(XFMD_IDENTITY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
# Run on explicit builds, including after checkout/commit without reconfigure.
# The generator preserves timestamps when the identity has not changed.
add_custom_target(xfmd_build_identity
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_identity.py"
    --source "${CMAKE_CURRENT_SOURCE_DIR}" --output "${XFMD_IDENTITY_DIR}"
  BYPRODUCTS "${XFMD_IDENTITY_DIR}/VersionGenerated.h" "${XFMD_IDENTITY_DIR}/xfmd-build.json"
  VERBATIM)
add_library(xfmd_version STATIC src/application/build/BuildVersion.cpp)
add_dependencies(xfmd_version xfmd_build_identity)
target_include_directories(xfmd_version PRIVATE "${XFMD_IDENTITY_DIR}")
configure_file(packaging/xfmd.1.in "${XFMD_IDENTITY_DIR}/xfmd.1" @ONLY)
install(FILES "${XFMD_IDENTITY_DIR}/xfmd-build.json" DESTINATION "${CMAKE_INSTALL_DOCDIR}")
