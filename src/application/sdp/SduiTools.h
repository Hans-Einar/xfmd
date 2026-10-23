#pragma once
#include <string>
namespace xfmd {
// Private installed/build-tree helpers; explicit environment override for testing.
std::string sduiExecutable(const std::string& name);
} // namespace xfmd
