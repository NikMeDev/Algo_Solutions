#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

void DFS(size_t current, size_t root,
         std::vector<std::set<size_t>>& trans_graph, std::vector<bool>& used,
         std::vector<std::vector<size_t>>& graph) {
  used[current] = true;
  for (size_t vertex : graph[current]) {
    if (!used[vertex]) {
      DFS(vertex, root, trans_graph, used, graph);
    }
  }
  trans_graph[root].insert(current);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  std::cin >> vertex_count;
  std::vector<std::vector<size_t>> graph(vertex_count, std::vector<size_t>());
  for (size_t i = 0; i < vertex_count; i++) {
    for (size_t j = 0; j < vertex_count; j++) {
      short edge;
      std::cin >> edge;
      if (edge == 1) {
        graph[i].push_back(j);
      }
    }
  }
  std::vector<std::set<size_t>> trans_graph(vertex_count);
  std::vector<bool> used(vertex_count, false);
  for (size_t i = 0; i < vertex_count; i++) {
    DFS(i, i, trans_graph, used, graph);
    used.assign(vertex_count, false);
  }
  for (size_t i = 0; i < vertex_count; i++) {
    size_t current = 0;
    for (auto vertex : trans_graph[i]) {
      while (current < vertex) {
        std::cout << 0 << ' ';
        current++;
      }
      std::cout << 1 << ' ';
      current++;
    }
    while (current < vertex_count) {
      std::cout << 0 << ' ';
      current++;
    }
    std::cout << '\n';
  }
  return 0;
}