#pragma once
#include "contracts/diagram/DiagramScene.h"
#include <list>
namespace xfmd {
class DiagramCache {
  struct Entry {
    std::string key;
    std::shared_ptr<const DiagramScene> scene;
  };
  std::list<Entry> entries;
  std::size_t size = 0;

public:
  std::shared_ptr<const DiagramScene> find(const std::string&);
  void insert(std::string, std::shared_ptr<const DiagramScene>);
  std::size_t bytes() const { return size; }
};
} // namespace xfmd
