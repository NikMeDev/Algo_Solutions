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

void Input(int64_t height, int64_t wight,
           std::vector<std::vector<uint8_t>>& field, int64_t& sum_black,
           int64_t& sum_white) {
  for (int64_t i = 0; i < height; i++) {
    for (int64_t j = 0; j < wight; j++) {
      char field_piece;
      std::cin >> field_piece;
      if (field_piece == 'H') {
        field[i][j] = 1;
      } else if (field_piece == 'O') {
        field[i][j] = 2;
      } else if (field_piece == 'N') {
        field[i][j] = 3;
      } else if (field_piece == 'C') {
        field[i][j] = 4;
      }
      if ((i + j) % 2 != 0) {
        sum_white += field[i][j];
      } else {
        sum_black += field[i][j];
      }
    }
  }
}

struct Params {
  int64_t height;
  int64_t wight;
  int64_t x_coord;
  int64_t y_coord;
};

void AddVertexToGraph(std::vector<Edge>& edges,
                      std::vector<std::vector<int64_t>>& graph,
                      std::vector<std::vector<uint8_t>>& field, Params params) {
  int64_t x_coord = params.x_coord;
  int64_t y_coord = params.y_coord;
  int64_t height = params.height;
  int64_t wight = params.wight;
  if (field[x_coord][y_coord] != 0) {
    if ((x_coord + y_coord) % 2 != 0) {
      AddEdge(graph, edges, height * wight, x_coord * wight + y_coord,
              field[x_coord][y_coord]);
      if (x_coord < height - 1 && field[x_coord + 1][y_coord] != 0) {
        AddEdge(graph, edges, x_coord * wight + y_coord,
                (x_coord + 1) * wight + y_coord, 1);
      }
      if (y_coord < wight - 1 && field[x_coord][y_coord + 1] != 0) {
        AddEdge(graph, edges, x_coord * wight + y_coord,
                x_coord * wight + y_coord + 1, 1);
      }
    } else {
      AddEdge(graph, edges, x_coord * wight + y_coord, height * wight + 1,
              field[x_coord][y_coord]);
      if (x_coord < height - 1 && field[x_coord + 1][y_coord] != 0) {
        AddEdge(graph, edges, (x_coord + 1) * wight + y_coord,
                x_coord * wight + y_coord, 1);
      }
      if (y_coord < wight - 1 && field[x_coord][y_coord + 1] != 0) {
        AddEdge(graph, edges, x_coord * wight + y_coord + 1,
                x_coord * wight + y_coord, 1);
      }
    }
  }
}

void MakeGraph(std::vector<Edge>& edges,
               std::vector<std::vector<int64_t>>& graph,
               std::vector<std::vector<uint8_t>>& field, int64_t height,
               int64_t wight) {
  for (int64_t i = 0; i < height; i++) {
    for (int64_t j = 0; j < wight; j++) {
      AddVertexToGraph(edges, graph, field, {height, wight, i, j});
    }
  }
}

void Main2() {
  int64_t height;
  int64_t wight;
  std::cin >> height >> wight;
  std::vector<std::vector<uint8_t>> field(height,
                                          std::vector<uint8_t>(wight, 0));
  int64_t sum_white = 0;
  int64_t sum_black = 0;
  Input(height, wight, field, sum_black, sum_white);
  std::vector<Edge> edges;
  std::vector<std::vector<int64_t>> graph(height * wight + 2,
                                          std::vector<int64_t>());
  MakeGraph(edges, graph, field, height, wight);
  int64_t max_flow = MaxFlow(graph, edges, height * wight, height * wight + 1);
  if (max_flow == sum_black && max_flow == sum_white && max_flow != 0) {
    std::cout << "Valid";
  } else {
    std::cout << "Invalid";
  }
}

int main() {
  Main2();
  return 0;
}
