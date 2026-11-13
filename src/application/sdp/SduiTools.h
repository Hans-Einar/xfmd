#pragma once
#include <string>
#include <vector>
namespace xfmd {
// Shared registered runtime; explicit absolute helper overrides for development.
std::string sduiExecutable(const std::string& name);
std::vector<std::string> sduiCommand(const std::string& operation);
} // namespace xfmd
