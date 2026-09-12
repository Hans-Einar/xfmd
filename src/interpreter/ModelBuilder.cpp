#include "ModelBuilder.h"
namespace xfmd {
ModelBuilder::ModelBuilder(const SourceSnapshot& source) : mapping(source.text) {
  model.token = source.token; model.sourceSize = source.text.size();
}
void ModelBuilder::beginBlock(cmark_node* node) {
  SemanticBlock block;
  block.source = mapping.record(node);
  switch (cmark_node_get_type(node)) {
  case CMARK_NODE_HEADING: block.kind = BlockKind::Heading; block.level = cmark_node_get_heading_level(node); break;
  case CMARK_NODE_CODE_BLOCK: block.kind = BlockKind::Code; break;
  case CMARK_NODE_HTML_BLOCK: block.kind = BlockKind::Html; break;
  case CMARK_NODE_THEMATIC_BREAK: block.kind = BlockKind::Rule; break;
  default: break;
  }
  int depth = 0;
  for (auto* parent = cmark_node_parent(node); parent; parent = cmark_node_parent(parent)) {
    if (++depth > 128) throw Error(ErrorCode::TooLarge, "Markdown nesting exceeds 128 levels.");
    if (cmark_node_get_type(parent) == CMARK_NODE_BLOCK_QUOTE) ++block.quoteDepth;
    if (cmark_node_get_type(parent) == CMARK_NODE_ITEM) {
      ++block.indent;
      if (cmark_node_first_child(parent) == node) {
        auto* list = cmark_node_parent(parent);
        block.marker = cmark_node_get_list_type(list) == CMARK_ORDERED_LIST ? std::to_string(listCounts[list]) + "." : "•";
      }
    }
  }
  model.blocks.push_back(std::move(block));
  active = &model.blocks.back();
}
void ModelBuilder::appendNode(cmark_node* node, cmark_event_type event) {
  auto type = cmark_node_get_type(node);
  bool block = type == CMARK_NODE_PARAGRAPH || type == CMARK_NODE_HEADING || type == CMARK_NODE_CODE_BLOCK || type == CMARK_NODE_HTML_BLOCK || type == CMARK_NODE_THEMATIC_BREAK;
  if (event == CMARK_EVENT_EXIT) {
    if (type == CMARK_NODE_IMAGE && active) active->runs.push_back({"]", mapping.record(node), false, true, false, {}});
    if (block) active = nullptr;
    return;
  }
  if (type == CMARK_NODE_LIST) listCounts[node] = cmark_node_get_list_start(node) - 1;
  if (type == CMARK_NODE_ITEM) ++listCounts[cmark_node_parent(node)];
  if (block) beginBlock(node);
  if (!active) return;
  InlineRun run;
  run.source = mapping.record(node);
  if (type == CMARK_NODE_TEXT || type == CMARK_NODE_CODE || type == CMARK_NODE_CODE_BLOCK || type == CMARK_NODE_HTML_BLOCK || type == CMARK_NODE_HTML_INLINE) {
    const char* literal = cmark_node_get_literal(node);
    run.text = literal ? literal : "";
    run.code = type == CMARK_NODE_CODE || type == CMARK_NODE_CODE_BLOCK;
    if (!mapping.matches(run.text, run.source)) run.source.quality = MappingQuality::Approximate;
  } else if (type == CMARK_NODE_SOFTBREAK) { run.text = " "; run.source.quality = MappingQuality::Approximate; }
  else if (type == CMARK_NODE_LINEBREAK) { run.text = "\n"; run.source.quality = MappingQuality::Approximate; }
  else if (type == CMARK_NODE_IMAGE) { run.text = "[image: "; run.italic = true; run.source.quality = MappingQuality::Approximate; }
  else return;
  for (auto* parent = cmark_node_parent(node); parent; parent = cmark_node_parent(parent)) {
    auto parentType = cmark_node_get_type(parent);
    if (parentType == CMARK_NODE_STRONG) run.bold = true;
    if (parentType == CMARK_NODE_EMPH || parentType == CMARK_NODE_IMAGE) run.italic = true;
    if (parentType == CMARK_NODE_LINK) { const char* url = cmark_node_get_url(parent); run.link = url ? url : ""; }
  }
  if (run.source.quality == MappingQuality::Unavailable) {
    run.source = active->source; run.source.quality = MappingQuality::Approximate;
  }
  active->runs.push_back(std::move(run));
}
SemanticDocument ModelBuilder::finish() { return std::move(model); }
}
