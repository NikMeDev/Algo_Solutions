#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

static constexpr int64_t kInf = std::numeric_limits<int64_t>::max();

struct Edge {
  size_t from;
  size_t to;
  int64_t weight;
  Edge(size_t from, size_t to, int64_t weight)
      : from(from), to(to), weight(weight) {}
};

bool CheckNegativeCycle(std::vector<Edge>& edges, size_t vertex_count,
                        size_t start) {
  std::vector<size_t> parent(vertex_count, vertex_count + 1);
  std::vector<int64_t> result(vertex_count, kInf);
  result[start] = 0;
  for (size_t k = 0; k < vertex_count; ++k) {
    for (auto edge : edges) {
      if (result[edge.from] != kInf &&
          result[edge.to] > result[edge.from] + edge.weight) {
        result[edge.to] = result[edge.from] + edge.weight;
        parent[edge.to] = edge.from;
      }
    }
  }
  for (auto edge : edges) {
    if (result[edge.from] != kInf &&
        result[edge.to] > result[edge.from] + edge.weight) {
      std::vector<size_t> cycle;
      size_t current = edge.to;
      for (size_t i = 0; i < vertex_count; i++) {
        current = parent[current];
      }
      size_t begin = current;
      do {
        cycle.push_back(current);
        current = parent[current];
      } while (begin != current);
      cycle.push_back(cycle[0]);
      std::reverse(cycle.begin(), cycle.end());
      std::cout << "YES" << std::endl;
      std::cout << cycle.size() << std::endl;
      for (size_t vertex : cycle) {
        std::cout << vertex + 1 << " ";
      }
      return false;
    }
  }
  return true;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  std::cin >> vertex_count;
  std::vector<Edge> edges;
  for (size_t i = 0; i < vertex_count; i++) {
    for (size_t j = 0; j < vertex_count; j++) {
      int64_t weight;
      std::cin >> weight;
      if (weight != 100000) {
        edges.emplace_back(i, j, weight);
      }
    }
  }
  for (size_t i = 0; i < vertex_count; i++) {
    if (!CheckNegativeCycle(edges, vertex_count, i)) {
      return 0;
    }
  }
  std::cout << "NO";
  return 0;
}