#include "application/adapters/SharedTextMetrics.h"
#include "interpreter/mermaid/MermaidInterpreter.h"
#include "renderer/diagram/MermaidDiagramLayout.h"
#include "renderer/diagram/DiagramTextLayout.h"
#include <fstream>
#include <iostream>
#include <iomanip>
using namespace xfmd;
int main(int argc, char** argv) {
  if (argc != 3) return 2;
  std::ifstream file(argv[1]);
  if (!file) return 2;
  std::string text((std::istreambuf_iterator<char>(file)), {});
  MermaidInterpreter parser;
  auto parsed=parser.parse({text,{}});
  if (!parsed.model) {std::cerr << "PARSE: " << parsed.error << '\n'; return 1;}
  std::cout << "Parsed nodes=" << parsed.model->nodes.size() << " edges=" << parsed.model->edges.size() << " groups=" << parsed.model->groups.size() << '\n';
  SharedTextMetrics metrics;
  std::ofstream measured(std::string(argv[2])+".metrics.tsv");
  measured << std::setprecision(17);
  for (const auto& [label,value] : DiagramTextLayout::measure(*parsed.model,metrics))
    measured << std::quoted(label) << '\t' << value.width << '\t' << value.height << '\n';
  try {
    MermaidDiagramLayout layout;
    auto scene=layout.layout(*parsed.model,{},metrics);
    std::ofstream(std::string(argv[2])+".svg") << scene->svg;
    std::cout << "Scene nodes=" << scene->nodes.size() << " edges=" << scene->edges.size() << " groups=" << scene->groups.size() << '\n' << scene->diagnostics << '\n';
    for (const auto& box:scene->groups) std::cout << "group " << box.x << ' ' << box.y << ' ' << box.width << ' ' << box.height << '\n';
    return 0;
  } catch (const std::exception& e) {std::cerr << "LAYOUT: " << e.what() << '\n';return 1;}
}
