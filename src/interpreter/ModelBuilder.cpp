#include "ModelBuilder.h"
#include "boxui/BoxUiBlockBuilder.h"
#include <map>
#include "mermaid/MermaidBlockBuilder.h"
namespace xfmd {
ModelBuilder::ModelBuilder(const SourceSnapshot& source, const MathSyntax* syntax,
                           IDiagramInterpreter* parser, IBoxUiInterpreter* boxes)
    : math(syntax), diagrams(parser), boxUi(boxes), mapping(source.text, syntax ? &syntax->masked : nullptr) {
  model.token = source.token;
  model.sourceSize = source.text.size();
}
void ModelBuilder::beginBlock(cmark_node* node) {
  SemanticBlock block;
  block.source = mapping.record(node);
  switch (cmark_node_get_type(node)) {
  case CMARK_NODE_HEADING:
    block.kind = BlockKind::Heading;
    block.level = cmark_node_get_heading_level(node);
    break;
  case CMARK_NODE_CODE_BLOCK:
    block.kind = BlockKind::Code;
    break;
  case CMARK_NODE_HTML_BLOCK:
    block.kind = BlockKind::Html;
    break;
  case CMARK_NODE_THEMATIC_BREAK:
    block.kind = BlockKind::Rule;
    break;
  default:
    break;
  }
  int depth = 0;
  for (auto* parent = cmark_node_parent(node); parent; parent = cmark_node_parent(parent)) {
    if (++depth > 128)
      throw Error(ErrorCode::TooLarge, "Markdown nesting exceeds 128 levels.");
    if (cmark_node_get_type(parent) == CMARK_NODE_BLOCK_QUOTE)
      ++block.quoteDepth;
    if (cmark_node_get_type(parent) == CMARK_NODE_ITEM) {
      ++block.indent;
      if (cmark_node_first_child(parent) == node) {
        auto* list = cmark_node_parent(parent);
        block.marker = cmark_node_get_list_type(list) == CMARK_ORDERED_LIST
                           ? std::to_string(listCounts[list]) + "."
                           : "•";
      }
    }
  }
  model.blocks.push_back(std::move(block));
  active = &model.blocks.back();
}
void ModelBuilder::appendNode(cmark_node* node, cmark_event_type event) {
  for (auto* parent = cmark_node_parent(node); parent; parent = cmark_node_parent(parent))
    if (cmark_node_get_type(parent) == CMARK_NODE_IMAGE)
      return;
  if (appendTable(node, event))
    return;
  auto type = cmark_node_get_type(node);
  bool block = type == CMARK_NODE_PARAGRAPH || type == CMARK_NODE_HEADING ||
               type == CMARK_NODE_CODE_BLOCK || type == CMARK_NODE_HTML_BLOCK ||
               type == CMARK_NODE_THEMATIC_BREAK;
  if (event == CMARK_EVENT_EXIT) {
    if (block)
      active = nullptr;
    return;
  }
  if (type == CMARK_NODE_LIST)
    listCounts[node] = cmark_node_get_list_start(node) - 1;
  if (type == CMARK_NODE_ITEM)
    ++listCounts[cmark_node_parent(node)];
  if (block)
    beginBlock(node);
  if (!active)
    return;
  if (type == CMARK_NODE_CODE_BLOCK) {
    const char* literal = cmark_node_get_literal(node);
    const char* info = cmark_node_get_fence_info(node);
    if (BoxUiBlockBuilder::build(*active, info, literal, boxUi, diagramCount))
      return;
    if (MermaidBlockBuilder::build(*active, info, literal, diagrams, diagramCount))
      return;
    if (info && (std::string(info) == "math" || std::string(info) == "latex")) {
      active->kind = BlockKind::Paragraph;
      InlineRun formula;
      formula.source = mapping.record(node);
      formula.text = literal ? literal : "";
      formula.embedded = {EmbeddedKind::Math, formula.text, true, {}};
      active->runs.push_back(std::move(formula));
    } else
      active->runs = mapping.codeLines(node, literal ? literal : "");
    return;
  }
  InlineRun run;
  run.source = mapping.record(node);
  const auto* formula = math && type == CMARK_NODE_CODE ? math->at(run.source) : nullptr;
  if (formula) {
    run.text = formula->formula;
    run.source = formula->range;
    run.embedded = {EmbeddedKind::Math, formula->formula, formula->display, {}};
  } else if (type == CMARK_NODE_TEXT || type == CMARK_NODE_CODE || type == CMARK_NODE_CODE_BLOCK ||
             type == CMARK_NODE_HTML_BLOCK || type == CMARK_NODE_HTML_INLINE) {
    const char* literal = cmark_node_get_literal(node);
    run.text = literal ? literal : "";
    run.code = type == CMARK_NODE_CODE || type == CMARK_NODE_CODE_BLOCK;
    if (!mapping.matches(run.text, run.source))
      run.source.quality = MappingQuality::Approximate;
  } else if (type == CMARK_NODE_SOFTBREAK) {
    run.text = " ";
    run.source.quality = MappingQuality::Approximate;
  } else if (type == CMARK_NODE_LINEBREAK) {
    run.text = "\n";
    run.source.quality = MappingQuality::Approximate;
  } else if (type == CMARK_NODE_IMAGE) {
    auto* iter = cmark_iter_new(node);
    while (cmark_iter_next(iter) != CMARK_EVENT_DONE) {
      auto* child = cmark_iter_get_node(iter);
      if (cmark_iter_get_event_type(iter) == CMARK_EVENT_ENTER &&
          (cmark_node_get_type(child) == CMARK_NODE_TEXT ||
           cmark_node_get_type(child) == CMARK_NODE_CODE)) {
        const char* text = cmark_node_get_literal(child);
        if (text)
          run.text += text;
      }
    }
    cmark_iter_free(iter);
    const char* url = cmark_node_get_url(node);
    run.embedded = {EmbeddedKind::Image, url ? url : "", false, {}};
    run.source.quality = MappingQuality::Approximate;
  } else
    return;
  for (auto* parent = cmark_node_parent(node); parent; parent = cmark_node_parent(parent)) {
    auto parentType = cmark_node_get_type(parent);
    if (parentType == CMARK_NODE_STRONG)
      run.bold = true;
    if (parentType == CMARK_NODE_EMPH || parentType == CMARK_NODE_IMAGE)
      run.italic = true;
    if (parentType == CMARK_NODE_LINK) {
      const char* url = cmark_node_get_url(parent);
      run.link = url ? url : "";
      run.linkId = mapping.record(parent).begin + 1;
    }
  }
  if (run.source.quality == MappingQuality::Unavailable) {
    run.source = active->source;
    run.source.quality = MappingQuality::Approximate;
  }
  active->runs.push_back(std::move(run));
}
SemanticDocument ModelBuilder::finish() {
  std::map<std::string, unsigned> counts;
  for(const auto& b:model.blocks) if(b.boxUi) ++counts[b.boxUi->documentId];
  for(auto& b:model.blocks) if(b.boxUi && counts[b.boxUi->documentId]>1) {
    b.boxUi.reset(); b.kind=BlockKind::Code;
    b.runs.push_back({"BoxUI: duplicate documentId in Markdown document\n"+b.diagramSource,b.source,false,false,true,{}});
  }
  return std::move(model);
}
} // namespace xfmd
