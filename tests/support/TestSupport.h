#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#define CHECK(...) do { if (!(__VA_ARGS__)) throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " #__VA_ARGS__); } while (false)
#define TEST_MAIN(body) int main() { try { body(); std::cout << "PASS\n"; return 0; } catch (const std::exception& e) { std::cerr << e.what() << '\n'; return 1; } }
