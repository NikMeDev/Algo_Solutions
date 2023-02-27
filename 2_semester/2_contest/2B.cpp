#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

std::vector<int64_t> DijkstraViruses(
    std::vector<std::vector<std::pair<size_t, int64_t>>>& edges,
    size_t vertex_count, std::vector<size_t> start) {
  std::vector<int64_t> results(vertex_count, -1);
  std::priority_queue<std::pair<int64_t, size_t>,
                      std::vector<std::pair<int64_t, size_t>>, std::greater<>>
      queue;
  for (size_t vertex : start) {
    queue.push({0, vertex});
    results[vertex] = 0;
  }
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();
    if (current.first == results[current.second]) {
      for (auto edge : edges[current.second]) {
        if (results[edge.first] == -1 ||
            results[edge.first] > results[current.second] + edge.second) {
          results[edge.first] = results[current.second] + edge.second;
          queue.push({results[edge.first], edge.first});
        }
      }
    }
  }
  return results;
}

std::vector<int64_t> DijkstraHumans(
    std::vector<std::vector<std::pair<size_t, int64_t>>>& edges,
    size_t vertex_count, size_t start, std::vector<int64_t>& virus_times) {
  std::vector<int64_t> results(vertex_count, -1);
  std::priority_queue<std::pair<int64_t, size_t>,
                      std::vector<std::pair<int64_t, size_t>>, std::greater<>>
      queue;

  queue.push({0, start});
  results[start] = 0;
  while (!queue.empty()) {
    auto current = queue.top();
    queue.pop();
    if (current.first == results[current.second]) {
      for (auto edge : edges[current.second]) {
        if ((results[edge.first] == -1 ||
             results[edge.first] > results[current.second] + edge.second) &&
            results[current.second] + edge.second < virus_times[edge.first]) {
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
  size_t vertex_count;
  size_t edge_count;
  size_t virus_count;
  std::cin >> vertex_count >> edge_count >> virus_count;

  std::vector<size_t> viruses(virus_count);
  for (size_t i = 0; i < virus_count; i++) {
    std::cin >> viruses[i];
    --viruses[i];
  }

  std::vector<std::vector<std::pair<size_t, int64_t>>> edges(
      vertex_count, std::vector<std::pair<size_t, int64_t>>());
  for (size_t i = 0; i < edge_count; i++) {
    size_t start_edge;
    size_t end_edge;
    int64_t weight;
    std::cin >> start_edge >> end_edge >> weight;
    --start_edge;
    --end_edge;
    edges[start_edge].emplace_back(end_edge, weight);
    edges[end_edge].emplace_back(start_edge, weight);
  }

  size_t start;
  size_t end;

  std::cin >> start >> end;
  --start;
  --end;

  auto virus_times = DijkstraViruses(edges, vertex_count, viruses);
  auto results = DijkstraHumans(edges, vertex_count, start, virus_times);
  std::cout << results[end];
  return 0;
}