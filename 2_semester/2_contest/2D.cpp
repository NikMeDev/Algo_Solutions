#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

static constexpr int64_t kInf = 30000;

struct Edge {
  size_t from;
  size_t to;
  int64_t weight;
  Edge(size_t from, size_t to, int64_t weight)
      : from(from), to(to), weight(weight) {}
};

std::vector<int64_t> FordBellman(std::vector<Edge>& edges, size_t vertex_count,
                                 size_t start) {
  std::vector<int64_t> result(vertex_count, kInf);
  result[start] = 0;
  for (size_t k = 1; k < vertex_count - 1; ++k) {
    for (auto edge : edges) {
      if (result[edge.from] != kInf &&
          result[edge.to] > result[edge.from] + edge.weight) {
        result[edge.to] = result[edge.from] + edge.weight;
      }
    }
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  std::vector<Edge> edges;
  for (size_t i = 0; i < edge_count; i++) {
    size_t from;
    size_t to;
    int64_t weight;
    std::cin >> from >> to >> weight;
    from--;
    to--;
    edges.emplace_back(from, to, weight);
  }
  auto result = FordBellman(edges, vertex_count, 0);
  for (auto weight : result) {
    std::cout << weight << " ";
  }
  return 0;
}