// 123
#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <set>
#include <unordered_map>
#include <vector>

template <class VType = size_t, class EType = std::pair<VType, VType>>
class Graph {
 public:
  virtual std::vector<VType>& GetNeighbors(const VType&) = 0;
  virtual size_t VertexCount() = 0;
  virtual size_t EdgeCount() = 0;
  virtual const std::vector<VType>& Vertices() = 0;
  // NOLINTNEXTLINE
  virtual typename std::vector<VType>::iterator begin(const VType&) = 0;
  // NOLINTNEXTLINE
  virtual typename std::vector<VType>::iterator end(const VType&) = 0;
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
  // NOLINTNEXTLINE
  typename std::vector<VType>::iterator begin(const VType& vertex) override {
    return data_[vertex].begin();
  }
  // NOLINTNEXTLINE
  typename std::vector<VType>::iterator end(const VType& vertex) override {
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
  virtual void Visit(const VType&) = 0;
  virtual void Visit(const EType&) = 0;
  virtual void Update(const EType&) = 0;
  virtual bool IsVisited(const VType& vertex) = 0;
  ~Visitor() = default;
};

template <class VType, class EType = std::pair<VType, VType>>
class BridgesVisitor : public Visitor<VType, EType> {
 public:
  void Visit(const VType& vertex) override {
    time_in_[vertex] = time_++;
    time_up_[vertex] = time_in_[vertex];
    visited_[vertex] = true;
  };
  void Visit(const EType& edge) override {
    time_up_[edge.first] =
        std::min(time_up_[edge.first], time_in_[edge.second]);
    if (time_up_[edge.second] > time_in_[edge.first]) {
      bridges_.push_back(edge);
    }
  }
  void Update(const EType& edge) override {
    time_up_[edge.first] =
        std::min(time_up_[edge.first], time_up_[edge.second]);
    if (time_up_[edge.second] > time_in_[edge.first]) {
      bridges_.push_back(edge);
    }
  }
  bool IsVisited(const VType& vertex) override { return visited_[vertex]; }
  std::vector<EType> GetBridges() { return bridges_; };
  BridgesVisitor(size_t vertex_count) {}
  ~BridgesVisitor() = default;

 private:
  std::unordered_map<VType, size_t> time_up_;
  std::unordered_map<VType, size_t> time_in_;
  std::unordered_map<VType, bool> visited_;
  std::vector<EType> bridges_;
  size_t time_ = 0;
};

template <>
class BridgesVisitor<int, std::pair<int, int>>
    : public Visitor<int, std::pair<int, int>> {
 public:
  void Visit(const int& vertex) override {
    time_in_[vertex] = time_++;
    time_up_[vertex] = time_in_[vertex];
    visited_[vertex] = true;
  };
  void Visit(const std::pair<int, int>& edge) override {
    time_up_[edge.first] =
        std::min(time_up_[edge.first], time_in_[edge.second]);
    if (time_up_[edge.second] > time_in_[edge.first]) {
      bridges_.push_back(edge);
    }
  }
  void Update(const std::pair<int, int>& edge) override {
    time_up_[edge.first] =
        std::min(time_up_[edge.first], time_up_[edge.second]);
    if (time_up_[edge.second] > time_in_[edge.first]) {
      bridges_.push_back(edge);
    }
  }
  bool IsVisited(const int& vertex) override { return visited_[vertex]; }
  std::vector<std::pair<int, int>> GetBridges() { return bridges_; };
  BridgesVisitor(size_t vertex_count) {
    time_up_.assign(vertex_count, 0);
    time_in_.assign(vertex_count, 0);
    visited_.assign(vertex_count, false);
  }
  ~BridgesVisitor() = default;

 private:
  std::vector<int> time_up_;
  std::vector<int> time_in_;
  std::vector<bool> visited_;
  std::vector<std::pair<int, int>> bridges_;
  int time_ = 0;
};

template <class VType, class EType>
void BridgesDFS(VType& current, VType& parent, Graph<VType, EType>& graph,
                Visitor<VType, EType>& visitor) {
  visitor.Visit(current);
  for (VType neighbour : graph.GetNeighbors(current)) {
    if (parent != neighbour) {
      if (!visitor.IsVisited(neighbour)) {
        BridgesDFS(neighbour, current, graph, visitor);
        visitor.Update(std::make_pair(current, neighbour));
      } else {
        visitor.Visit(std::make_pair(current, neighbour));
      }
    }
  }
}

template <class VType, class EType>
std::vector<EType> GetBridges(Graph<VType, EType>& graph) {
  BridgesVisitor<VType, EType> visitor(graph.VertexCount());
  for (VType vertex : graph.Vertices()) {
    if (!visitor.IsVisited(vertex)) {
      BridgesDFS(vertex, vertex, graph, visitor);
    }
  }
  return visitor.GetBridges();
}

int main() {
  int vertex_count;
  int edge_count;
  std::cin >> vertex_count >> edge_count;
  std::vector<int> vertices(vertex_count);
  std::iota(vertices.begin(), vertices.end(), 0);
  std::vector<std::pair<int, int>> edges;
  std::unordered_map<int, std::unordered_map<int, std::pair<int, int>>> dict;
  for (int i = 1; i <= edge_count; i++) {
    int first;
    int second;
    std::cin >> first >> second;
    first--;
    second--;
    if (first != second) {
      edges.emplace_back(first, second);
    }
    dict[first][second].first = i;
    dict[second][first].first = i;
    dict[first][second].second++;
    dict[second][first].second++;
  }
  ListGraph<int> graph(vertices, edges);
  std::vector<std::pair<int, int>> bridges = GetBridges(graph);
  std::vector<int> result;
  std::set<int> set;
  for (auto edge : bridges) {
    if (dict[edge.first][edge.second].second == 1) {
      set.insert(dict[edge.first][edge.second].first);
    }
  }
  std::cout << set.size() << std::endl;
  for (int bridge : set) {
    std::cout << bridge << ' ';
  }
  return 0;
}