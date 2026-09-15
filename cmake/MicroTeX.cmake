# Pin the embeddable math engine; build only its Cairo backend, without GTK widgets/demo.
include(FetchContent)
FetchContent_Declare(microtex
  URL https://codeload.github.com/NanoMichael/MicroTeX/tar.gz/0e3707f6dafebb121d98b53c64364d16fefe481d
  URL_HASH SHA256=47476269d29c41df322bce6bdd2daa7017cc50b9eda841b2bec1767dba28daa6
  SOURCE_SUBDIR xfmd-no-upstream-cmake)
FetchContent_MakeAvailable(microtex)
# Fix upstream allocation/deallocation mismatch and modern fontconfig declaration.
file(READ "${microtex_SOURCE_DIR}/src/latex.cpp" latex_source)
string(REPLACE "delete userdata_fallback;" "free(userdata_fallback);" latex_source "${latex_source}")
file(READ "${microtex_SOURCE_DIR}/src/latex.cpp" original_latex)
if(NOT original_latex STREQUAL latex_source)
  file(WRITE "${microtex_SOURCE_DIR}/src/latex.cpp" "${latex_source}")
endif()
file(READ "${microtex_SOURCE_DIR}/src/platform/cairo/graphic_cairo.cpp" graphics_source)
if(NOT graphics_source MATCHES "fontconfig/fcfreetype.h")
  string(REPLACE "#include <fontconfig/fontconfig.h>" "#include <fontconfig/fontconfig.h>\n#include <fontconfig/fcfreetype.h>" graphics_source "${graphics_source}")
  file(WRITE "${microtex_SOURCE_DIR}/src/platform/cairo/graphic_cairo.cpp" "${graphics_source}")
endif()
file(GLOB_RECURSE microtex_sources CONFIGURE_DEPENDS
  "${microtex_SOURCE_DIR}/src/atom/*.cpp" "${microtex_SOURCE_DIR}/src/box/*.cpp"
  "${microtex_SOURCE_DIR}/src/core/*.cpp" "${microtex_SOURCE_DIR}/src/fonts/*.cpp"
  "${microtex_SOURCE_DIR}/src/res/*.cpp" "${microtex_SOURCE_DIR}/src/utils/*.cpp")
add_library(xfmd_math_engine STATIC ${microtex_sources}
  "${microtex_SOURCE_DIR}/src/latex.cpp" "${microtex_SOURCE_DIR}/src/render.cpp"
  "${microtex_SOURCE_DIR}/src/platform/cairo/graphic_cairo.cpp")
pkg_check_modules(MATHTYPE REQUIRED IMPORTED_TARGET cairomm-1.0 pangomm-1.4 tinyxml2 fontconfig)
target_include_directories(xfmd_math_engine SYSTEM PUBLIC "${microtex_SOURCE_DIR}/src")
target_compile_definitions(xfmd_math_engine PUBLIC BUILD_GTK)
target_compile_options(xfmd_math_engine PRIVATE -w)
target_link_libraries(xfmd_math_engine PUBLIC PkgConfig::MATHTYPE)
install(DIRECTORY "${microtex_SOURCE_DIR}/res/" DESTINATION ${CMAKE_INSTALL_DATADIR}/xfmd/math)
install(FILES "${microtex_SOURCE_DIR}/LICENSE" DESTINATION ${CMAKE_INSTALL_DOCDIR} RENAME MicroTeX-LICENSE)

# Upstream allocates builtin macros at static initialization, even if init() is
# never called. Always release them; make release idempotent for the normal path.
file(READ "${microtex_SOURCE_DIR}/src/core/macro.cpp" macro_source)
if(NOT macro_source MATCHES "_commands.clear\\(\\)")
  string(REPLACE "for (const auto& i : _commands) delete i.second;"
    "for (const auto& i : _commands) delete i.second;\n  _commands.clear();" macro_source "${macro_source}")
  file(WRITE "${microtex_SOURCE_DIR}/src/core/macro.cpp" "${macro_source}")
endif()
file(READ "${microtex_SOURCE_DIR}/src/core/macro_def.cpp" definitions_source)
if(NOT definitions_source MATCHES "XfmdMacroCleanup")
  file(APPEND "${microtex_SOURCE_DIR}/src/core/macro_def.cpp"
    "\nnamespace { struct XfmdMacroCleanup { ~XfmdMacroCleanup() { tex::MacroInfo::_free_(); } }; XfmdMacroCleanup xfmdMacroCleanup; }\n")
endif()

# Duplicate symbol registrations replace an owned CharFont in upstream's raw map.
file(READ "${microtex_SOURCE_DIR}/src/fonts/fonts.cpp" fonts_source)
if(NOT fonts_source MATCHES "delete _symbolMappings\\[c.name\\]")
  string(REPLACE "_symbolMappings[c.name] = new CharFont(c.code, c.font);"
    "delete _symbolMappings[c.name];\n    _symbolMappings[c.name] = new CharFont(c.code, c.font);" fonts_source "${fonts_source}")
  file(WRITE "${microtex_SOURCE_DIR}/src/fonts/fonts.cpp" "${fonts_source}")
endif()

# Inclusive binary search must stop at rows-1 (accent/next-larger lookup).
file(READ "${microtex_SOURCE_DIR}/src/utils/indexed_arr.h" indexed_source)
if(NOT indexed_source MATCHES "h = int\\(_rows\\) - 1")
  string(REPLACE "int     l = 0, h = _rows;" "int     l = 0, h = int(_rows) - 1;" indexed_source "${indexed_source}")
  file(WRITE "${microtex_SOURCE_DIR}/src/utils/indexed_arr.h" "${indexed_source}")
endif()
