#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

constexpr size_t kInf = std::numeric_limits<size_t>::max();

template <typename T>
class SparseTable {
 public:
  SparseTable(const std::vector<T>& vector)
      : logs_(vector.size() + 1, 0), n_(vector.size()) {
    std::vector<std::pair<T, size_t>> data(vector.size());
    for (size_t i = 0; i < vector.size(); i++) {
      data[i] = {vector[i], i};
    }
    PrecalcLogs(data.size());
    log_n_ = logs_[n_];
    sparse_.assign(log_n_ + 1, std::vector<std::pair<T, size_t>>(
                                   n_ + 1, std::pair<T, size_t>()));
    BuildSparse(data);
  }

  SparseTable() = default;

  std::pair<T, size_t> Min(size_t l, size_t r) {
    if (l == r) {
      return {kInf, -1};
    }
    size_t lg = logs_[r - l];
    std::pair<T, size_t> left = sparse_[lg][l];
    std::pair<T, size_t> right = sparse_[lg][r - (1 << lg)];
    return std::min(left, right);
  }

 private:
  std::vector<size_t> logs_;
  size_t n_;
  size_t log_n_;
  std::vector<std::vector<std::pair<T, size_t>>> sparse_;
  void BuildSparse(const std::vector<std::pair<T, size_t>>& data) {
    sparse_[0] = data;
    for (size_t k = 1; k <= log_n_; ++k) {
      for (size_t i = 0; i + (1 << k) <= n_; ++i) {
        sparse_[k][i] =
            std::min(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  void PrecalcLogs(size_t n) {
    logs_[0] = 0;
    logs_[1] = 0;
    for (size_t i = 2; i <= n; ++i) {
      logs_[i] = logs_[i / 2] + 1;
    }
  }
};

class Solution {
 public:
  void StartDFS(size_t current, size_t height = 0) {
    order.push_back(current);
    heights.push_back(height);
    depths[current] = height;
    first[current] = order.size() - 1;
    for (size_t vertex : edges[current]) {
      if (first[vertex] == kInf) {
        StartDFS(vertex, height + 1);
        order.push_back(current);
        heights.push_back(height);
      }
    }
  }

  size_t Query(size_t first_person, size_t second_person) {
    auto [depth, index] =
        table.Min(std::min(first[first_person], first[second_person]),
                  std::max(first[first_person], first[second_person]) + 1);
    return depths[first_person] + depths[second_person] - 2 * depth;
  }

  explicit Solution(size_t vertex_count) {
    edges.assign(vertex_count, std::vector<size_t>());
    first.assign(vertex_count, kInf);
    depths.assign(vertex_count, kInf);
    for (size_t i = 0; i < vertex_count - 1; i++) {
      size_t begin;
      size_t end;
      std::cin >> begin >> end;
      --begin;
      --end;
      edges[begin].push_back(end);
      edges[end].push_back(begin);
    }
    StartDFS(0);
    table = SparseTable(heights);
  }

 private:
  std::vector<std::vector<size_t>> edges;
  std::vector<size_t> first;
  std::vector<size_t> depths;
  std::vector<size_t> order;
  std::vector<size_t> heights;
  SparseTable<size_t> table;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t vertex_count;
  std::cin >> vertex_count;
  Solution solution(vertex_count);
  size_t query_count;
  std::cin >> query_count;
  for (size_t i = 0; i < query_count; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    --first;
    --second;
    std::cout << solution.Query(first, second) << '\n';
  }
  return 0;
}