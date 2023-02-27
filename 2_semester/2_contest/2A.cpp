#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

std::vector<int> Dijkstra(
    std::vector<std::vector<std::pair<size_t, int>>>& edges,
    size_t vertex_count, size_t start) {
  std::vector<int> results(vertex_count, 2009000999);
  std::priority_queue<std::pair<int, size_t>,
                      std::vector<std::pair<int, size_t>>, std::greater<>>
      queue;
  queue.push({0, start});
  results[start] = 0;
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();
    if (current.first == results[current.second]) {
      for (auto edge : edges[current.second]) {
        if (results[edge.first] == 2009000999 ||
            results[edge.first] > results[current.second] + edge.second) {
          results[edge.first] = results[current.second] + edge.second;
          queue.push({results[edge.first], edge.first});
        }
      }
    }
  }
  return results;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t count;
  std::cin >> count;
  for (size_t count_no = 0; count_no < count; count_no++) {
    size_t vertex_count;
    size_t edge_count;
    std::cin >> vertex_count >> edge_count;
    std::vector<std::vector<std::pair<size_t, int>>> edges(
        vertex_count, std::vector<std::pair<size_t, int>>());
    for (size_t i = 0; i < edge_count; i++) {
      size_t start_edge;
      size_t end_edge;
      int weight;
      std::cin >> start_edge >> end_edge >> weight;
      edges[start_edge].emplace_back(end_edge, weight);
      edges[end_edge].emplace_back(start_edge, weight);
    }
    size_t start;
    std::cin >> start;
    auto results = Dijkstra(edges, vertex_count, start);
    for (auto result : results) {
      std::cout << result << " ";
    }
    std::cout << std::endl;
  }
  return 0;
}