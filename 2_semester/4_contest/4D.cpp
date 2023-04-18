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
  int64_t height;
  int64_t wight;
  std::cin >> height >> wight;
  int64_t double_price;
  int64_t single_price;
  std::cin >> double_price >> single_price;
  int64_t valid = 0;
  std::vector<std::vector<bool>> field(height, std::vector<bool>(wight, false));
  for (int64_t i = 0; i < height; i++) {
    for (int64_t j = 0; j < wight; j++) {
      char field_piece;
      std::cin >> field_piece;
      if (field_piece == '*') {
        field[i][j] = true;
        ++valid;
      }
    }
  }
  std::vector<Edge> edges;
  std::vector<std::vector<int64_t>> graph(height * wight + 2,
                                          std::vector<int64_t>());
  for (int64_t i = 0; i < height; i++) {
    for (int64_t j = 0; j < wight; j++) {
      if (field[i][j]) {
        if ((i + j) % 2) {
          AddEdge(graph, edges, height * wight, i * wight + j, 1);
        } else {
          AddEdge(graph, edges, i * wight + j, height * wight + 1, 1);
        }
        if (i < height - 1 && field[i + 1][j]) {
          if ((i + j) % 2) {
            AddEdge(graph, edges, i * wight + j, (i + 1) * wight + j, 1);
          } else {
            AddEdge(graph, edges, (i + 1) * wight + j, i * wight + j, 1);
          }
        }
        if (j < wight - 1 && field[i][j + 1]) {
          if ((i + j) % 2) {
            AddEdge(graph, edges, i * wight + j, i * wight + j + 1, 1);
          } else {
            AddEdge(graph, edges, i * wight + j + 1, i * wight + j, 1);
          }
        }
      }
    }
  }
  int64_t doubles = MaxFlow(graph, edges, height * wight, height * wight + 1);
  if (2 * single_price > double_price) {
    std::cout << doubles * double_price + (valid - doubles * 2) * single_price;
  } else {
    std::cout << valid * single_price;
  }
  return 0;
}
