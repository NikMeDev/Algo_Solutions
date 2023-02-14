#include <iostream>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <vector>

template <class VType = size_t, class EType = std::pair<VType, VType>>
class Graph {
 public:
  virtual std::vector<VType>& GetNeighbors(const VType&) = 0;
  virtual size_t VertexCount() = 0;
  virtual size_t EdgeCount() = 0;
  virtual const std::vector<VType>& Vertices() = 0;
  virtual typename std::vector<VType>::iterator Begin(const VType&) = 0;
  virtual typename std::vector<VType>::iterator End(const VType&) = 0;
  ~Graph() = default;
};

template <class VType = size_t, class EType = std::pair<VType, VType>,
          bool ordered = false>
class ListGraph : public Graph<VType, EType> {
 public:
  ListGraph(const std::vector<VType>& vertices, const std::vector<EType>& edges)
      : kVertexCount(vertices.size()),
        kEdgeCount(edges.size()),
        vertices_(vertices) {
    for (auto edge : edges) {
      data_[edge.first].push_back(edge.second);
      if constexpr (!ordered) {
        data_[edge.second].push_back(edge.first);
      }
    }
  }
  std::vector<VType>& GetNeighbors(const VType& vertex) override {
    return data_[vertex];
  }
  size_t VertexCount() override { return kVertexCount; }
  size_t EdgeCount() override { return kEdgeCount; }
  const std::vector<VType>& Vertices() override { return vertices_; }
  typename std::vector<VType>::iterator Begin(const VType& vertex) override {
    return data_[vertex].begin();
  }
  typename std::vector<VType>::iterator End(const VType& vertex) override {
    return data_[vertex].end();
  }
  ~ListGraph() = default;

 private:
  std::unordered_map<VType, std::vector<VType>> data_;
  const size_t kVertexCount;
  const size_t kEdgeCount;
  std::vector<VType> vertices_;
};

template <class VType, class EType>
class Visitor {
 public:
  virtual void Visit(const EType&) = 0;
  ~Visitor() = default;
};

template <class VType, class EType = std::pair<VType, VType>>
class BFSVisitor : public Visitor<VType, EType> {
 public:
  void Visit(const EType& edge) override {
    if (map_.find(edge.second) == map_.end()) {
      map_[edge.second] = edge.first;
    }
  };
  std::unordered_map<VType, VType> GetMap() { return map_; }
  ~BFSVisitor() = default;

 private:
  std::unordered_map<VType, VType> map_;
};

template <class VType, class EType>
void BFS(Graph<VType>& graph, VType start, Visitor<VType, EType>& visitor) {
  std::unordered_map<VType, bool> visited;
  std::queue<VType> queue;
  queue.push(start);
  while (!queue.empty()) {
    VType current = queue.front();
    for (VType vertex : graph.GetNeighbors(current)) {
      if (!visited[vertex]) {
        visitor.Visit({current, vertex});
        queue.push(vertex);
      }
    }
    visited[current] = true;
    queue.pop();
  }
}

int main() {
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  size_t start;
  size_t end;
  std::cin >> start >> end;
  if (start == end) {
    std::cout << 0 << '\n' << start;
    return 0;
  }
  std::vector<size_t> vertices(vertex_count);
  std::iota(vertices.begin(), vertices.end(), 1);
  std::vector<std::pair<size_t, size_t>> edges;
  for (size_t i = 0; i < edge_count; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    edges.emplace_back(first, second);
  }
  ListGraph<size_t> graph(vertices, edges);
  BFSVisitor<size_t> visitor;
  BFS(graph, start, visitor);
  auto map = visitor.GetMap();
  std::vector<size_t> result;
  if (map.find(end) == map.end()) {
    std::cout << -1;
    return 0;
  }
  while (map.find(end) != map.end()) {
    result.push_back(end);
    end = map[end];
  }
  result.push_back(end);
  std::cout << result.size() - 1 << std::endl;
  for (size_t i = 1; i <= result.size(); i++) {
    std::cout << result[result.size() - i] << ' ';
  }
}