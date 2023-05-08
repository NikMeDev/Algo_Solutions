#include <iostream>
#include <vector>

template <int64_t x, int64_t q>
std::vector<bool> Hash(const std::string& str, size_t length) {
  std::vector<bool> table(q, false);
  int64_t current_hash = 0;
  int64_t pow = 1;
  for (size_t i = 0; i < length; i++) {
    current_hash += ((str[length - i - 1] - 'a' + 1) * pow) % q;
    current_hash %= q;
    if (i != length - 1) {
      pow *= x;
      pow %= q;
    }
  }
  table[current_hash] = true;
  for (size_t i = 0; i + length < str.size(); i++) {
    current_hash += q;
    current_hash -= (pow * (str[i] - 'a' + 1)) % q;
    current_hash %= q;
    current_hash *= x;
    current_hash %= q;
    current_hash += (str[i + length] - 'a' + 1);
    current_hash %= q;
    table[current_hash] = true;
  }
  return table;
}

template <int64_t x, int64_t q, int64_t x_two, int64_t q_two>
size_t FindCountOfMatchesOfCyclicPattern(std::string& text,
                                         std::string& pattern) {
  size_t result = 0;
  auto hashes = Hash<x, q>(pattern + pattern, pattern.length());
  auto hashes_two = Hash<x_two, q_two>(pattern + pattern, pattern.length());
  int64_t current_hash = 0;
  int64_t current_hash_two = 0;
  int64_t pow = 1;
  int64_t pow_two = 1;
  for (size_t i = 0; i < pattern.length(); i++) {
    current_hash += ((text[pattern.length() - i - 1] - 'a' + 1) * pow) % q;
    current_hash %= q;
    current_hash_two +=
        ((text[pattern.length() - i - 1] - 'a' + 1) * pow_two) % q_two;
    current_hash_two %= q_two;
    if (i != pattern.length() - 1) {
      pow *= x;
      pow %= q;
      pow_two *= x_two;
      pow_two %= q_two;
    }
  }
  if (hashes[current_hash] && hashes_two[current_hash_two]) {
    ++result;
  }
  for (size_t i = 0; i + pattern.length() < text.size(); i++) {
    current_hash += q;
    current_hash -= (pow * (text[i] - 'a' + 1)) % q;
    current_hash %= q;
    current_hash *= x;
    current_hash %= q;
    current_hash += (text[i + pattern.length()] - 'a' + 1);
    current_hash %= q;

    current_hash_two += q_two;
    current_hash_two -= (pow_two * (text[i] - 'a' + 1)) % q_two;
    current_hash_two %= q_two;
    current_hash_two *= x_two;
    current_hash_two %= q_two;
    current_hash_two += (text[i + pattern.length()] - 'a' + 1);
    current_hash_two %= q_two;
    if (hashes[current_hash] && hashes_two[current_hash_two]) {
      ++result;
    }
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string pattern;
  std::string text;
  std::cin >> pattern >> text;
  if (pattern.length() > text.length()) {
    std::cout << 0;
    return 0;
  }
  std::cout
      << FindCountOfMatchesOfCyclicPattern<1223, 1073704213, 11, 10000019>(
          text,
          pattern);
  return 0;
}