#include <iostream>
#include <limits>
#include <vector>

static constexpr int64_t kInfinity = std::numeric_limits<int64_t>::max();

struct Edge {
  int64_t to_vertex;
  int64_t capacity;
  int64_t flow;

  int64_t CurrentCapacity() const { return capacity - flow; }

  Edge(int64_t to_vertex, int64_t capacity)
      : to_vertex(to_vertex), capacity(capacity), flow(0) {}
};

void AddEdge(std::vector<std::vector<int64_t>>& graph, std::vector<Edge>& edges,
             int64_t from, int64_t to, int64_t capacity) {
  edges.emplace_back(to, capacity);
  graph[from].emplace_back(edges.size() - 1);
  edges.emplace_back(from, 0);
  graph[to].emplace_back(edges.size() - 1);
}

struct State {
  size_t phase;
  int64_t current;
  int64_t end;
  int64_t min;
};

int64_t FindIncreasingPath(std::vector<std::vector<int64_t>>& graph,
                           std::vector<Edge>& edges, std::vector<size_t>& used,
                           State state) {
  if (state.end == state.current) {
    return state.min;
  }
  used[state.current] = state.phase;
  for (int64_t edge_id : graph[state.current]) {
    if (edges[edge_id].CurrentCapacity() > 0 &&
        used[edges[edge_id].to_vertex] != state.phase) {
      int64_t delta = FindIncreasingPath(
          graph, edges, used,
          {state.phase, edges[edge_id].to_vertex, state.end,
           std::min(state.min, edges[edge_id].CurrentCapacity())});
      if (delta > 0) {
        edges[edge_id].flow += delta;
        edges[edge_id ^ 1].flow -= delta;
        return delta;
      }
    }
  }
  return 0;
}

int64_t MaxFlow(std::vector<std::vector<int64_t>>& graph,
                std::vector<Edge>& edges, int64_t start, int64_t end) {
  std::vector<size_t> used(graph.size(), 0);
  size_t phase = 1;
  while (FindIncreasingPath(graph, edges, used,
                            {phase, start, end, kInfinity}) != 0) {
    ++phase;
  }

  int64_t result = 0;
  for (int64_t edge_id : graph[start]) {
    result += edges[edge_id].flow;
  }
  return result;
}

int main() {
  int64_t left_size;
  int64_t right_size;
  std::cin >> left_size >> right_size;
  std::vector<Edge> edges;
  int64_t vertex_count = left_size + right_size + 2;
  std::vector<std::vector<int64_t>> graph(vertex_count, std::vector<int64_t>());
  for (int64_t i = 1; i <= left_size; i++) {
    AddEdge(graph, edges, 0, i, 1);
  }
  for (int64_t i = 1; i <= right_size; i++) {
    AddEdge(graph, edges, left_size + i, vertex_count - 1, 1);
  }
  for (int64_t i = 1; i <= left_size; i++) {
    int64_t right_vertex;
    std::cin >> right_vertex;
    while (right_vertex != 0) {
      AddEdge(graph, edges, i, left_size + right_vertex, 1);
      std::cin >> right_vertex;
    }
  }
  std::cout << MaxFlow(graph, edges, 0, vertex_count - 1) << std::endl;
  for (int64_t i = 1; i <= left_size; i++) {
    for (int64_t edge_id : graph[i]) {
      if (edges[edge_id].flow != 0 && edges[edge_id].to_vertex != 0) {
        std::cout << i << ' ' << edges[edge_id].to_vertex - left_size
                  << std::endl;
        break;
      }
    }
  }
  return 0;
}
