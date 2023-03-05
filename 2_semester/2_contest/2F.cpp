#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

static constexpr int64_t kNegInf = std::numeric_limits<int64_t>::min();
static constexpr size_t kInf = std::numeric_limits<size_t>::max();

struct Edge {
  size_t from;
  size_t to;
  int64_t weight;
  Edge(size_t from, size_t to, int64_t weight)
      : from(from), to(to), weight(weight) {}
};

std::pair<std::vector<std::vector<size_t>>, std::vector<std::vector<int64_t>>>
FloydWarshall(std::vector<Edge>& edges, size_t vertex_count) {
  std::vector<std::vector<int64_t>> result(
      vertex_count, std::vector<int64_t>(vertex_count, kNegInf));
  std::vector<std::vector<size_t>> path(
      vertex_count, std::vector<size_t>(vertex_count, kInf));
  for (size_t i = 0; i < vertex_count; i++) {
    result[i][i] = 0;
    path[i][i] = i;
  }
  for (size_t i = 0; i < edges.size(); i++) {
    if (result[edges[i].from][edges[i].to] < edges[i].weight) {
      result[edges[i].from][edges[i].to] = edges[i].weight;
      path[edges[i].from][edges[i].to] = i;
    }
  }
  for (size_t k = 0; k < vertex_count; k++) {
    for (size_t i = 0; i < vertex_count; i++) {
      for (size_t j = 0; j < vertex_count; j++) {
        if (result[i][k] > kNegInf && result[k][j] > kNegInf) {
          if (result[i][j] < result[i][k] + result[k][j]) {
            result[i][j] = result[i][k] + result[k][j];
            path[i][j] = path[i][k];
          }
        }
      }
    }
  }
  return {path, result};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  size_t path_count;
  std::cin >> vertex_count >> edge_count >> path_count;
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
  std::vector<size_t> destinations(path_count);
  for (size_t i = 0; i < path_count; i++) {
    std::cin >> destinations[i];
    --destinations[i];
  }
  auto[paths, distances] = FloydWarshall(edges, vertex_count);
  for (size_t i = 0; i < vertex_count; i++) {
    if (distances[i][i] > 0) {
      for (size_t j = 0; j < path_count - 1; j++) {
        if (distances[destinations[j]][i] > kNegInf &&
            distances[i][destinations[j + 1]] > kNegInf) {
          std::cout << "infinitely kind";
          return 0;
        }
      }
    }
  }
  std::vector<size_t> result;
  size_t current = destinations[0];
  for (size_t i = 1; i < path_count; i++) {
    size_t next = destinations[i];
    while (current != next) {
      size_t edge = paths[current][next];
      current = edges[edge].to;
      result.push_back(edge + 1);
    }
  }
  std::cout << result.size() << std::endl;
  for (size_t edge : result) {
    std::cout << edge << " ";
  }
  return 0;
}