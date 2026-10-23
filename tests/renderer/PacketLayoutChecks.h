#pragma once
// Packet regression: actual Pango metrics flow through the native measured bridge.
// Fork geometry assertions and inspected SVG/PDF complement these integration checks.
inline void checkPacketLayout(MermaidInterpreter& parser, MermaidDiagramLayout& layout,
                              SharedTextMetrics& metrics) {
  const std::pair<const char*, const char*> fixtures[] = {
      {"packet-narrow", "packet\n0: \"PacketFlag\"\n1-12: \"PacketNumber\"\n13-28: \"PacketPayload\""},
      {"packet-split", "packet\n+1: \"FirstLongFlag\"\n+1: \"SecondLongFlag\"\n+29: \"Body\"\n+2: \"CrossingLongName\"\n+1: \"LastFlag\""},
      {"packet-high", "packet\n4093: \"LongHighFlag\"\n4094: \"NextHighFlag\"\n4095: \"LastHighFlag\""}};
  for (const auto& fixture : fixtures) {
    auto parsed = parser.parse({fixture.second, {}});
    CHECK(parsed.model && parsed.model->semantic);
    const auto scene = layout.layout(*parsed.model, {}, metrics);
    CHECK(scene->svg.find("[1]") != std::string::npos);
    CHECK(scene->svg.find("Bits ") != std::string::npos);
    if (std::string(fixture.first) == "packet-narrow") {
      CHECK(scene->svg.find("Bits 0: PacketFlag") != std::string::npos);
      CHECK(scene->svg.find("PacketNumber") != std::string::npos);
      CHECK(scene->svg.find("PacketPayload") != std::string::npos);
    }
    if (std::string(fixture.first) == "packet-split") {
      CHECK(scene->svg.find("Bits 31: CrossingLongName") != std::string::npos);
      CHECK(scene->svg.find("Bits 32: CrossingLongName") != std::string::npos);
    }
    if (const char* directory = std::getenv("XFMD_DIAGRAM_EVIDENCE"))
      std::ofstream(std::string(directory) + "/" + fixture.first + ".svg") << scene->svg;
  }
}
