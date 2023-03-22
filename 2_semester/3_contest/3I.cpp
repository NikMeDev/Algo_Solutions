#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t n;
  size_t m;
  std::cin >> n >> m;
  std::vector<size_t> a(n);
  for(size_t i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  size_t result = 0;
  std::sort(a.begin(), a.end());
  size_t first_element = a[0];
  auto begin = a.begin();
  auto end = a.end();
  while(begin != end) {
    if(begin != a.begin()) {
      if(end != a.end()) {
        result += std::min(first_element + *begin, (*begin + *end) % m);
      } else {
        result += first_element + *begin;
      }
    }
    auto separator = std::lower_bound(begin + 1, end, m - *begin);
    for(auto it = separator; it != end; ++it) {
      result += (*begin + *it) % m;
    }
    end = separator;
    ++begin;
  }
  std::cout << result;
  return 0;
}