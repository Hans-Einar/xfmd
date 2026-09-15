cmake_minimum_required(VERSION 3.20)
project(xfmd VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(PREPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.deps")
include(CTest)
include(GNUInstallDirs)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED COMPONENTS Xrandr)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FOX REQUIRED IMPORTED_TARGET fox>=1.6.57)
pkg_check_modules(PANGO REQUIRED IMPORTED_TARGET pangocairo pangofc)
pkg_check_modules(CMARK REQUIRED libcmark-gfm=0.29.0.gfm.13)
find_library(CMARK_CORE NAMES cmark-gfm HINTS ${CMARK_LIBRARY_DIRS} REQUIRED)
find_library(CMARK_EXTENSIONS NAMES cmark-gfm-extensions HINTS ${CMARK_LIBRARY_DIRS} REQUIRED)
option(XFMD_SANITIZERS "Enable Address/UndefinedBehavior sanitizers" OFF)
if(XFMD_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

include(cmake/MicroTeX.cmake)
include(cmake/Mermaid.cmake)
pkg_check_modules(PIXBUF REQUIRED IMPORTED_TARGET gdk-pixbuf-2.0)
add_library(xfmd_contracts INTERFACE)
target_include_directories(xfmd_contracts INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_library(xfmd_interpreter
  src/interpreter/mermaid/MermaidBlockBuilder.cpp src/interpreter/mermaid/MermaidInterpreter.cpp src/interpreter/MathSyntax.cpp src/interpreter/CmarkInterpreter.cpp src/interpreter/ModelBuilder.cpp
  src/interpreter/SourceMapBuilder.cpp src/interpreter/TableModelBuilder.cpp)
target_link_libraries(xfmd_interpreter PUBLIC xfmd_contracts PRIVATE xfmd_rust ${CMARK_EXTENSIONS} ${CMARK_CORE})
target_include_directories(xfmd_interpreter PRIVATE ${CMARK_INCLUDE_DIRS})
add_library(xfmd_renderer
  src/renderer/diagram/DiagramPlacement.cpp src/renderer/diagram/DiagramTextLayout.cpp src/renderer/diagram/MermaidDiagramLayout.cpp
  src/renderer/MarkdownRenderer.cpp src/renderer/BlockLayout.cpp src/renderer/TableLayout.cpp
  src/renderer/PageBreaker.cpp src/renderer/PageComposer.cpp src/renderer/PageAnchorIndex.cpp
  src/renderer/InlineLayout.cpp src/renderer/HitTester.cpp src/renderer/LinkMarker.cpp)
target_link_libraries(xfmd_renderer PUBLIC xfmd_contracts PRIVATE xfmd_rust)
add_library(xfmd_document
  src/application/io/InputPolicy.cpp src/application/io/LocalFileStore.cpp
  src/application/document/DocumentSession.cpp src/application/document/DocumentCoordinator.cpp
  src/application/document/TextProjection.cpp src/application/document/EditController.cpp)
target_link_libraries(xfmd_document PUBLIC xfmd_contracts)
add_library(xfmd_preview
  src/application/preview/PreviewSelection.cpp src/application/preview/PreviewCoordinator.cpp src/application/preview/ParserWorker.cpp)
target_link_libraries(xfmd_preview PUBLIC xfmd_document Threads::Threads)
add_library(xfmd_scroll
  src/application/scroll/AnchorMapper.cpp src/application/scroll/ScrollCoordinator.cpp
  src/application/scroll/ScrollDynamics.cpp)
target_link_libraries(xfmd_scroll PUBLIC xfmd_contracts)
add_library(xfmd_navigation
  src/application/navigation/LinkResolver.cpp src/application/navigation/HistoryStore.cpp
  src/application/navigation/NavigationCoordinator.cpp)
target_link_libraries(xfmd_navigation PUBLIC xfmd_document xfmd_scroll)
add_library(xfmd_index src/application/index/DocumentIndex.cpp src/application/index/ReferenceWorker.cpp)
target_link_libraries(xfmd_index PUBLIC xfmd_navigation Threads::Threads)
add_library(xfmd_workspace
  src/application/workspace/WorkPathHistory.cpp
  src/application/workspace/FileNameFilter.cpp
  src/application/workspace/DirectoryScanner.cpp)
target_link_libraries(xfmd_workspace PUBLIC xfmd_contracts Threads::Threads)
add_library(xfmd_media src/application/media/EmbeddedVisuals.cpp
  src/application/media/ImageDecoder.cpp src/application/media/MathTypesetter.cpp)
target_link_libraries(xfmd_media PUBLIC xfmd_contracts PRIVATE xfmd_navigation xfmd_math_engine PkgConfig::PIXBUF)
target_compile_definitions(xfmd_media PRIVATE XFMD_MATH_RESOURCES="${microtex_SOURCE_DIR}/res")
add_library(xfmd_typography
  src/application/adapters/FontCatalog.cpp src/application/adapters/SharedTextMetrics.cpp
  src/application/adapters/DiagramPainter.cpp src/application/adapters/DisplayListPainter.cpp src/application/preferences/ReadingColors.cpp)
target_link_libraries(xfmd_typography PUBLIC xfmd_contracts PkgConfig::PANGO)
add_library(xfmd_diagrams src/application/diagrams/DiagramCache.cpp src/application/diagrams/DiagramPreparation.cpp src/application/composition/DiagramServices.cpp)
target_link_libraries(xfmd_diagrams PUBLIC xfmd_typography xfmd_media xfmd_renderer xfmd_interpreter)
add_library(xfmd_export src/application/export/ExportCoordinator.cpp
  src/application/export/ExportPipeline.cpp src/application/io/PdfFilePublisher.cpp
  src/application/adapters/PdfOutput.cpp)
target_link_libraries(xfmd_export PUBLIC xfmd_typography xfmd_media xfmd_diagrams Threads::Threads)
add_library(xfmd_application
  src/application/ui/controls/CompactToolbar.cpp src/application/ui/controls/PreviewControls.cpp src/application/ui/controls/PreviewColorControls.cpp
  src/application/ui/style/ThemeProfiles.cpp
  src/application/ui/style/IconCatalog.cpp src/application/ui/style/UiContext.cpp
  src/application/ui/style/UiStyling.cpp src/application/ui/controls/ButtonPainter.cpp
  src/application/ui/controls/UiButton.cpp src/application/ui/controls/UiLayout.cpp
  src/application/preferences/PreferencesService.cpp
  src/application/adapters/ExternalBrowser.cpp
  src/application/adapters/FoxPreferencesStore.cpp src/application/ui/PreferencesDialog.cpp src/application/ui/PreferencesPages.cpp
  src/application/ApplicationTree.cpp src/application/adapters/DesktopFileOpener.cpp
  src/application/ApplicationAppearance.cpp src/application/ui/XfmdToolbar.cpp
  src/application/ApplicationIndex.cpp src/application/Application.cpp src/application/ApplicationCommands.cpp src/application/ApplicationExport.cpp
  src/application/commands/CommandRouter.cpp src/application/ui/XfmdWindow.cpp src/application/ui/IconResources.cpp
  src/application/ui/EditorWidget.cpp src/application/ui/EditorPresentation.cpp src/application/ui/SidebarWidget.cpp src/application/ui/WorkspacePanel.cpp src/application/ui/RecentFilesPanel.cpp
  src/application/ui/NavigationTree.cpp src/application/ui/IndexPanel.cpp
  src/application/ui/ViewModeController.cpp
  src/application/adapters/FoxCairoCanvas.cpp src/application/adapters/ViewTransform.cpp src/application/adapters/FoxWindowMode.cpp
  src/application/adapters/FoxPreviewInput.cpp src/application/adapters/FoxRenderHost.cpp src/application/adapters/FoxScheduler.cpp
  src/application/adapters/FoxWheelScrollBar.cpp)
target_link_libraries(xfmd_application PUBLIC
  xfmd_index xfmd_export xfmd_typography xfmd_workspace xfmd_document xfmd_preview xfmd_scroll xfmd_navigation xfmd_interpreter xfmd_renderer PkgConfig::FOX X11::X11 X11::Xrandr)
add_executable(xfmd src/application/main.cpp)
target_link_libraries(xfmd PRIVATE xfmd_application)

if(BUILD_TESTING)
  include(cmake/Tests.cmake)
endif()
install(TARGETS xfmd RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES packaging/xfmd.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES packaging/xfmd.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
install(FILES packaging/xfmd.1 DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
install(FILES README.md packaging/appearance.ini LICENSES/cmark-gfm.txt DESTINATION ${CMAKE_INSTALL_DOCDIR})
set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_VENDOR "Hans-Einar")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Native FOX Markdown viewer and editor")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)

foreach(size 16 24 32 48 64 128)
  install(FILES packaging/icons/xfmd-${size}.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/${size}x${size}/apps RENAME xfmd.png)
endforeach()
