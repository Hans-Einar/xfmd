#pragma once
#include "DiagramAbi.h"
#include "DiagramModel.h"
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <string_view>
namespace xfmd::diagramWire {
struct Writer {
  std::vector<std::uint8_t> data;
  void integer(std::uint32_t v) {
    for (int i = 0; i < 4; ++i)
      data.push_back(v >> (8 * i));
  }
  void number(double v) {
    std::uint64_t bits;
    std::memcpy(&bits, &v, 8);
    for (int i = 0; i < 8; ++i)
      data.push_back(bits >> (8 * i));
  }
  void text(std::string_view v) {
    integer(v.size());
    data.insert(data.end(), v.begin(), v.end());
  }
  void model(const DiagramModel& m) {
    integer(2);
    integer(m.sequence ? 1 : 0);
    if (m.sequence) {
      if (!m.nodes.empty() || !m.edges.empty() || !m.groups.empty())
        throw std::runtime_error("Mixed sequence and flowchart model");
      integer(m.sequence->participants.size());
      for (const auto& p : m.sequence->participants) {
        text(p.id);
        text(p.label);
        integer(p.actor);
      }
      integer(m.sequence->events.size());
      for (const auto& e : m.sequence->events) {
        integer(std::uint32_t(e.kind));
        integer(e.first);
        integer(e.second);
        text(e.text);
      }
      return;
    }
    integer(m.direction);
    integer(m.nodes.size());
    for (const auto& n : m.nodes) {
      text(n.id);
      text(n.label);
      integer(std::uint32_t(n.shape));
    }
    integer(m.edges.size());
    for (const auto& e : m.edges) {
      integer(e.from);
      integer(e.to);
      text(e.label);
      integer(e.arrowStart | (e.arrowEnd << 1));
      integer(e.style);
    }
    integer(m.groups.size());
    for (const auto& g : m.groups) {
      text(g.id);
      text(g.label);
      integer(g.direction);
      integer(g.parent);
      integer(g.nodes.size());
      for (auto n : g.nodes)
        integer(n);
    }
  }
};
struct Reader {
  const std::uint8_t* data;
  std::size_t size, at = 0;
  explicit Reader(const XfmdDiagramResult& result) : data(result.data), size(result.size) {
    if (result.abi_version != 1 || result.struct_size != sizeof(result) || size > 8 * 1024 * 1024 ||
        (!data && size))
      throw std::runtime_error("Invalid diagram ABI result");
    if (result.status)
      throw std::runtime_error(size ? std::string(reinterpret_cast<const char*>(data), size)
                                    : "Diagram engine failed");
  }
  void need(std::size_t n) {
    if (n > size - at)
      throw std::runtime_error("Truncated diagram result");
  }
  std::uint32_t integer() {
    need(4);
    std::uint32_t v = 0;
    for (int i = 0; i < 4; ++i)
      v |= std::uint32_t(data[at++]) << (8 * i);
    return v;
  }
  std::uint32_t count(std::uint32_t limit) {
    auto n = integer();
    if (n > limit)
      throw std::runtime_error("Diagram limit exceeded");
    return n;
  }
  double number() {
    need(8);
    std::uint64_t bits = 0;
    for (int i = 0; i < 8; ++i)
      bits |= std::uint64_t(data[at++]) << (8 * i);
    double v;
    std::memcpy(&v, &bits, 8);
    if (!std::isfinite(v) || std::abs(v) > 1e7)
      throw std::runtime_error("Invalid diagram geometry");
    return v;
  }
  std::string text(std::uint32_t limit = 65536) {
    auto n = count(limit);
    need(n);
    std::string s(reinterpret_cast<const char*>(data + at), n);
    at += n;
    return s;
  }
  void finish() {
    if (at != size)
      throw std::runtime_error("Trailing diagram data");
  }
  DiagramModel model() {
    DiagramModel m;
    if (integer() != 2)
      throw std::runtime_error("Unsupported diagram model version");
    if (count(1)) {
      SequenceModel s;
      const auto n = count(16);
      for (unsigned i = 0; i < n; ++i) {
        auto id = text(), label = text();
        auto actor = count(1);
        s.participants.push_back({id, label, bool(actor)});
      }
      const auto events = count(128);
      for (unsigned i = 0; i < events; ++i) {
        auto kind = count(13), first = integer(), second = integer();
        auto value = text();
        if (first >= n || second >= n)
          throw std::runtime_error("Invalid sequence reference");
        s.events.push_back({SequenceEventKind(kind), first, second, value});
      }
      m.sequence = std::move(s);
      return m;
    }
    m.direction = count(3);
    auto nn = count(128);
    for (unsigned i = 0; i < nn; ++i) {
      auto id = text(), label = text();
      auto shape = count(3);
      m.nodes.push_back({id, label, DiagramShape(shape)});
    }
    auto ne = count(512);
    for (unsigned i = 0; i < ne; ++i) {
      auto from = integer(), to = integer();
      auto label = text();
      auto arrows = count(3), style = count(2);
      if (from >= nn || to >= nn)
        throw std::runtime_error("Invalid edge endpoint");
      m.edges.push_back({from, to, label, bool(arrows & 1), bool(arrows & 2), style});
    }
    auto ng = count(32);
    for (unsigned i = 0; i < ng; ++i) {
      DiagramGroup g;
      g.id = text();
      g.label = text();
      g.direction = count(4);
      g.parent = integer();
      if (g.parent != UINT32_MAX && g.parent >= i)
        throw std::runtime_error("Invalid group parent");
      auto n = count(128);
      for (unsigned j = 0; j < n; ++j) {
        auto id = integer();
        if (id >= nn)
          throw std::runtime_error("Invalid member");
        g.nodes.push_back(id);
      }
      m.groups.push_back(std::move(g));
    }
    return m;
  }
};
// Result storage never escapes a bridge call into the semantic model.
struct ResultOwner {
  XfmdDiagramResult result;
  void (*release)(XfmdDiagramResult*);
  ~ResultOwner() { release(&result); }
  ResultOwner(const ResultOwner&) = delete;
  ResultOwner(XfmdDiagramResult r, void (*f)(XfmdDiagramResult*)) : result(r), release(f) {}
};
} // namespace xfmd::diagramWire
