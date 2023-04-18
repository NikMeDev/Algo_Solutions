#include <iostream>
#include <limits>
#include <set>
#include <unordered_map>
#include <vector>

static constexpr int64_t kInfinity = std::numeric_limits<int64_t>::max();

struct Edge {
  int64_t from;
  int64_t to_vertex;
  int64_t capacity;
  int64_t flow;

  int64_t CurrentCapacity() const { return capacity - flow; }

  Edge(int64_t to_vertex, int64_t capacity, int64_t from)
      : from(from), to_vertex(to_vertex), capacity(capacity), flow(0) {}
};

void AddEdge(std::vector<std::vector<int64_t>>& graph, std::vector<Edge>& edges,
             int64_t from, int64_t to, int64_t capacity) {
  edges.emplace_back(to, capacity, from);
  graph[from].emplace_back(edges.size() - 1);
  edges.emplace_back(from, 0, to);
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

void FindConnected(std::vector<std::vector<int64_t>>& graph,
                   std::vector<Edge>& edges, int64_t current,
                   std::vector<bool>& used) {
  used[current] = true;
  for (int64_t edge_id : graph[current]) {
    if (!used[edges[edge_id].to_vertex] &&
        (edges[edge_id].capacity - edges[edge_id].flow) > 0) {
      FindConnected(graph, edges, edges[edge_id].to_vertex, used);
    }
  }
}

void Main2() {  // Прошёл всего год, ~~а у меня борода~~, встречайте Мейн 2!
  std::vector<Edge> edges;
  int64_t vertex_count;
  int64_t edge_count;
  std::cin >> vertex_count >> edge_count;
  std::unordered_map<int64_t, std::unordered_map<int64_t, int64_t>> edge_ids;
  std::vector<std::vector<int64_t>> graph(vertex_count + edge_count,
                                          std::vector<int64_t>());
  for (int64_t i = 0; i < edge_count; ++i) {
    int64_t from;
    int64_t to;
    int64_t capacity;
    std::cin >> from >> to >> capacity;
    --from;
    --to;
    AddEdge(graph, edges, from, to, capacity);
    AddEdge(graph, edges, to, vertex_count + i, capacity);
    AddEdge(graph, edges, vertex_count + i, from, capacity);
    edge_ids[from][to] = i + 1;
    edge_ids[to][from] = i + 1;
    edge_ids[to][vertex_count + i] = i + 1;
    edge_ids[vertex_count + i][to] = i + 1;
    edge_ids[vertex_count + i][from] = i + 1;
    edge_ids[from][vertex_count + i] = i + 1;
  }

  int64_t max_flow = MaxFlow(graph, edges, 0, vertex_count - 1);
  std::vector<bool> used(graph.size(), false);
  FindConnected(graph, edges, 0, used);
  std::set<int64_t> answer;
  for (Edge edge : edges) {
    if (used[edge.from] != used[edge.to_vertex]) {
      answer.insert(edge_ids[edge.from][edge.to_vertex]);
    }
  }
  std::cout << answer.size() << ' ' << max_flow << '\n';
  for (int64_t ans : answer) {
    std::cout << ans << ' ';
  }
};

int main() {
  Main2();
  return 0;
}
