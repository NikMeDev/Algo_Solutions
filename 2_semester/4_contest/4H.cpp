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

void Main2() {
  int64_t vertex_count;
  std::cin >> vertex_count;
  std::vector<Edge> edges;
  std::vector<std::vector<int64_t>> graph(vertex_count + 2,
                                          std::vector<int64_t>());
  std::vector<int64_t> starting(vertex_count);
  for (int64_t i = 0; i < vertex_count; i++) {
    std::cin >> starting[i];
  }
  std::vector<int64_t> will_be_played(vertex_count);
  for (int64_t i = 0; i < vertex_count; i++) {
    std::cin >> will_be_played[i];
  }
  std::vector<std::vector<int64_t>> matrix_graph(
      vertex_count, std::vector<int64_t>(vertex_count));
  for (int64_t i = 0; i < vertex_count; i++) {
    for (int64_t j = 0; j < vertex_count; j++) {
      std::cin >> matrix_graph[i][j];
    }
  }
  for (int64_t i = 1; i < vertex_count; i++) {
    for (int64_t j = i + 1; j < vertex_count; j++) {
      starting[i] += matrix_graph[i][j];
      AddEdge(graph, edges, i, j, matrix_graph[i][j]);
    }
  }
  int64_t score_to_beat = starting[0] + will_be_played[0];
  int64_t must_score = 0;
  for (int64_t i = 1; i < vertex_count; i++) {
    must_score += starting[i];
    AddEdge(graph, edges, vertex_count, i, starting[i]);
    AddEdge(graph, edges, i, vertex_count + 1, score_to_beat);
  }
  int64_t result = MaxFlow(graph, edges, vertex_count, vertex_count + 1);
  if (result < must_score) {
    std::cout << "NO";
  } else {
    std::cout << "YES";
  }
}

int main() {
  Main2();
  return 0;
}
