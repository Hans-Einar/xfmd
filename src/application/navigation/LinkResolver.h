#pragma once
#include <string>
namespace xfmd {
class LinkResolver { public: static std::string resolve(const std::string& document, const std::string& target); };
}
