#include <iostream>
#include <vector>

std::vector<int64_t> PrefixFunction(std::string& str) {
  std::vector<int64_t> prefix_function(str.length());
  prefix_function[0] = 0;
  for (size_t i = 1; i < str.length(); i++) {
    int64_t k = prefix_function[i - 1];
    while (k > 0 && str[i] != str[k]) {
      k = prefix_function[k - 1];
    }
    if (str[i] == str[k]) {
      ++k;
    }
    prefix_function[i] = k;
  }
  return prefix_function;
}

template <char special_symbol = '#'>
std::vector<size_t> KMP(std::string& text, std::string& pattern) {
  std::string str = pattern + special_symbol + text;
  std::vector<int64_t> prefix_function = PrefixFunction(str);
  std::vector<size_t> result;
  for (size_t i = 0; i < prefix_function.size(); i++) {
    if (prefix_function[i] == pattern.length()) {
      result.push_back(i - 2 * pattern.length());
    }
  }
  return result;
}

int main() {
  std::string text;
  std::string pattern;
  std::cin >> text >> pattern;
  std::vector<size_t> result = KMP(text, pattern);
  for (size_t i : result) {
    std::cout << i << '\n';
  }
  return 0;
}