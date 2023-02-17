#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

void DFS(size_t current, std::vector<std::vector<size_t>>& graph,
         std::vector<bool>& used, std::vector<size_t>& order) {
  used[current] = true;
  for (size_t vertex : graph[current]) {
    if (!used[vertex]) {
      DFS(vertex, graph, used, order);
    }
  }
  order.push_back(current);
}

std::vector<size_t> Sort(std::vector<std::vector<size_t>>& graph) {
  std::vector<size_t> order;
  std::vector<bool> used(graph.size(), false);
  for (size_t i = 0; i < graph.size(); i++) {
    if (!used[i]) {
      DFS(i, graph, used, order);
    }
  }
  std::reverse(order.begin(), order.end());
  return order;
}

void ComponentDFS(size_t current, std::vector<std::vector<size_t>>& graph,
                  std::vector<size_t>& results, size_t count) {
  results[current] = count;
  for (size_t vertex : graph[current]) {
    if (results[vertex] == 0) {
      ComponentDFS(vertex, graph, results, count);
    }
  }
}

size_t FindComponents(std::vector<std::vector<size_t>>& transpose,
                      std::vector<size_t>& order,
                      std::vector<size_t>& results) {
  size_t count = 0;
  for (size_t vertex : order) {
    if (results[vertex] == 0) {
      count++;
      ComponentDFS(vertex, transpose, results, count);
    }
  }
  return count;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  std::vector<std::vector<size_t>> graph(vertex_count);
  std::vector<std::vector<size_t>> transpose(vertex_count);
  for (size_t i = 0; i < edge_count; i++) {
    size_t start;
    size_t end;
    std::cin >> start >> end;
    start--;
    end--;
    graph[start].push_back(end);
    transpose[end].push_back(start);
  }
  std::vector<size_t> order = Sort(graph);
  std::vector<size_t> results(vertex_count, 0);
  size_t count = FindComponents(transpose, order, results);
  std::cout << count << std::endl;
  for (size_t i : results) {
    std::cout << i << " ";
  }
  return 0;
}