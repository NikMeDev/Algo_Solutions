#include <iostream>
#include <vector>

int64_t PrefixFunction(std::string& str) {
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
  return prefix_function[prefix_function.size() - 1];
}

void Concat(std::string& left, std::string& right) {
  std::string concat;
  if (left.length() >= right.length()) {
    concat = right + '#' + left.substr(left.length() - right.length());
  } else {
    concat = right.substr(0, left.length()) + '#' + left;
  }
  int64_t prefix_function = PrefixFunction(concat);
  left += right.substr(prefix_function);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t count;
  std::cin >> count;
  std::string result;
  std::cin >> result;
  std::string temp;
  for (size_t i = 1; i < count; i++) {
    std::cin >> temp;
    Concat(result, temp);
  }
  std::cout << result;
  return 0;
}