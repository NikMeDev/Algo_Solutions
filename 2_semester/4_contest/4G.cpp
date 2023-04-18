#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

static constexpr int64_t kInfinity = std::numeric_limits<int64_t>::max();

struct Edge {
  int64_t from;
  int64_t to;
  int64_t flow;
  int64_t capacity;

  Edge(int64_t from, int64_t to, int64_t capacity)
      : from(from), to(to), flow(0), capacity(capacity) {}

  Edge(int64_t from, int64_t to, int64_t flow, int64_t capacity)
      : from(from), to(to), flow(flow), capacity(capacity) {}
};

class Graph {
 public:
  Graph(size_t size) : graph_(size, std::vector<size_t>()) {}

  void AddEdge(int64_t from, int64_t to, int64_t capacity) {
    edges_.emplace_back(from, to, capacity);
    size_t edge_id = edges_.size() - 1;
    ids_.push_back((int64_t)edge_id);
    edges_.emplace_back(to, from, 0);
    size_t back_edge_id = edges_.size() - 1;
    graph_[from].push_back(edge_id);
    graph_[to].push_back(back_edge_id);
  }

  bool BFS(size_t start, size_t end) {
    dist_.assign(graph_.size(), kInfinity);
    std::queue<size_t> queue;
    dist_[start] = 0;
    queue.push(start);
    while (!queue.empty() && dist_[end] == kInfinity) {
      size_t vertex = queue.front();
      queue.pop();
      for (unsigned long id : graph_[vertex]) {
        if (dist_[edges_[id].to] == kInfinity &&
            edges_[id].flow < edges_[id].capacity) {
          queue.push(edges_[id].to);
          dist_[edges_[id].to] = dist_[edges_[id].from] + 1;
        }
      }
    }
    return dist_[end] != kInfinity;  // увы ниче не нашли!
  }

  int64_t DFS(size_t vertex, size_t end, int64_t current_flow,
              std::vector<int64_t>& ptr) {
    if (current_flow <= 0) {
      return 0;
    }
    if (vertex == end) {
      return current_flow;
    }
    for (int64_t i = ptr[vertex]; i < (int64_t)graph_[vertex].size(); i++) {
      auto to_id = (int64_t)graph_[vertex][i];
      size_t to = edges_[to_id].to;
      if (dist_[to] == dist_[vertex] + 1) {
        int64_t flow = DFS(
            to, end,
            std::min(current_flow, edges_[to_id].capacity - edges_[to_id].flow),
            ptr);
        if (flow > 0) {
          edges_[to_id].flow += flow;
          edges_[to_id ^ 1].flow -= flow;
          return flow;
        }
      }
      ++ptr[vertex];
    }
    return 0;
  }

  int64_t GetFlow(size_t edge_id) { return edges_[ids_[edge_id]].flow; }

  int64_t DinicAlgo(size_t start, size_t end) {
    int64_t flow = 0;
    while (BFS(start, end)) {
      std::vector<int64_t> ptr(graph_.size(), 0);
      int64_t delta = 0;
      do {
        delta = DFS(start, end, kInfinity, ptr);
        flow += delta;
      } while (delta > 0);
    }
    return flow;
  }

 private:
  std::vector<Edge> edges_;
  std::vector<std::vector<size_t>> graph_;
  std::vector<int64_t> dist_;
  std::vector<int64_t> ids_;
};

void Main2() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  Graph gr(vertex_count);
  for (size_t i = 0; i < edge_count; ++i) {
    int64_t from;
    int64_t to;
    int64_t capacity;
    std::cin >> from >> to >> capacity;
    --from;
    --to;
    gr.AddEdge(from, to, capacity);
  }
  std::cout << gr.DinicAlgo(0, vertex_count - 1) << '\n';
  for (size_t i = 0; i < edge_count; i++) {
    std::cout << gr.GetFlow(i) << '\n';
  }
}

int main() {
  Main2();
  return 0;
}