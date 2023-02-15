#include <iostream>
#include <numeric>
#include <optional>
#include <queue>
#include <stack>
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
      if (!ordered) {
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

enum Color { White = 0, Gray, Black };

template <class VType, class EType>
std::optional<std::pair<size_t, size_t>> DFS(Graph<VType>& graph,
                                             Visitor<VType, EType>& visitor) {
  std::unordered_map<VType, Color> visited;
  for (VType vertex : graph.Vertices()) {
    if (visited[vertex] == White) {
      auto res = DFS(graph, visitor, vertex, visited);
      if (res.has_value()) {
        return res;
      }
    }
  }
  return std::nullopt;
}

template <class VType, class EType>
std::optional<std::pair<size_t, size_t>> DFS(
    Graph<VType>& graph, Visitor<VType, EType>& visitor, VType current,
    std::unordered_map<VType, Color>& visited) {
  visited[current] = Gray;
  for (VType neighbour : graph.GetNeighbors(current)) {
    if (visited[neighbour] == Gray) {
      return std::make_pair(neighbour, current);
    }
    if (visited[neighbour] != Black) {
      visitor.Visit({current, neighbour});
      auto res = DFS(graph, visitor, neighbour, visited);
      if (res.has_value()) {
        return res;
      }
    }
  }
  visited[current] = Black;
  return std::nullopt;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  std::vector<size_t> vertices(vertex_count);
  std::iota(vertices.begin(), vertices.end(), 1);
  std::vector<std::pair<size_t, size_t>> edges;
  for (size_t i = 0; i < edge_count; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    edges.emplace_back(first, second);
  }
  ListGraph<size_t, std::pair<size_t, size_t>, true> graph(vertices, edges);
  BFSVisitor<size_t> visitor;
  std::optional<std::pair<size_t, size_t>> points = DFS(graph, visitor);
  auto map = visitor.GetMap();
  std::vector<size_t> result;
  if (!points.has_value()) {
    std::cout << "NO";
    return 0;
  }
  size_t start = points->first;
  size_t end = points->second;
  int counter = 0;
  while (end != start) {
    result.push_back(end);
    end = map[end];
    counter++;
    if (counter > 100000) {
      return 0;
    }
  }
  result.push_back(end);
  std::cout << "YES" << std::endl;
  for (size_t i = 1; i <= result.size(); i++) {
    std::cout << result[result.size() - i] << ' ';
  }
}