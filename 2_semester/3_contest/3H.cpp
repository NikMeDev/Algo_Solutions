#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

template <typename EType>
class DSU {
 public:
  void Unite(EType first, EType second) {
    EType first_set = FindSet(first);
    EType second_set = FindSet(second);
    if (first_set != second_set) {
      if (ranks_[first_set] < ranks_[second_set]) {
        std::swap(first_set, second_set);
      }
      ancestors_[second_set] = first_set;
      if (ranks_[first_set] == ranks_[second_set]) {
        ++ranks_[first_set];
      }
    }
  }
  bool AreSame(EType first, EType second) {
    return FindSet(first) == FindSet(second);
  }
  DSU(std::vector<EType>& elements) : ancestors_(elements) {
    ranks_.assign(elements.size(), 0);
  }

 private:
  EType FindSet(EType element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }
  std::unordered_map<EType, EType> ancestors_;
  std::unordered_map<EType, size_t> ranks_;
};

template <>
class DSU<size_t> {
 public:
  void Unite(size_t first, size_t second) {
    size_t first_set = FindSet(first);
    size_t second_set = FindSet(second);
    if (first_set != second_set) {
      if (ranks_[first_set] < ranks_[second_set]) {
        std::swap(first_set, second_set);
      }
      ancestors_[second_set] = first_set;
      if (ranks_[first_set] == ranks_[second_set]) {
        ++ranks_[first_set];
      }
    }
  }
  bool AreSame(size_t first, size_t second) {
    return FindSet(first) == FindSet(second);
  }
  DSU(size_t size) {
    ancestors_.resize(size);
    ranks_.assign(size, 0);
    for (size_t i = 0; i < size; i++) {
      ancestors_[i] = i;
    }
  }

 private:
  size_t FindSet(size_t element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }
  std::vector<size_t> ancestors_;
  std::vector<size_t> ranks_;
};

template <typename EType, typename WType>
class WeightedDSU {
 public:
  void Unite(const EType& first, const EType& second, const WType& weight) {
    EType first_set = FindSet(first);
    EType second_set = FindSet(second);
    if (first_set != second_set) {
      if (ranks_[first_set] < ranks_[second_set]) {
        std::swap(first_set, second_set);
      }
      weights_[first_set] += weights_[second_set] + weight;
      ancestors_[second_set] = first_set;
      if (ranks_[first_set] == ranks_[second_set]) {
        ++ranks_[first_set];
      }
    } else {
      weights_[first_set] += weight;
    }
  }
  bool AreSame(const EType& first, const EType& second) {
    return FindSet(first) == FindSet(second);
  }
  WType GetWeight(const EType& element) { return weights_[FindSet(element)]; }
  WeightedDSU(const std::vector<EType>& elements) : ancestors_(elements) {
    ranks_.assign(elements.size(), 0);
    weights_.assign(elements.size(), 0);
  }

 private:
  EType FindSet(const EType& element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }
  std::unordered_map<EType, EType> ancestors_;
  std::unordered_map<EType, size_t> ranks_;
  std::unordered_map<EType, WType> weights_;
};

template <typename WType>
class WeightedDSU<size_t, WType> {
 public:
  void Unite(const size_t& first, const size_t& second, const WType& weight) {
    size_t first_set = FindSet(first);
    size_t second_set = FindSet(second);
    if (first_set != second_set) {
      if (ranks_[first_set] < ranks_[second_set]) {
        std::swap(first_set, second_set);
      }
      weights_[first_set] += weights_[second_set] + weight;
      ancestors_[second_set] = first_set;
      if (ranks_[first_set] == ranks_[second_set]) {
        ++ranks_[first_set];
      }
    } else {
      weights_[first_set] += weight;
    }
  }
  bool AreSame(const size_t& first, const size_t& second) {
    return FindSet(first) == FindSet(second);
  }
  WType GetWeight(const size_t& element) { return weights_[FindSet(element)]; }
  WeightedDSU(size_t size) {
    weights_.assign(size, 0);
    ancestors_.assign(size, 0);
    ranks_.assign(size, 0);
    for (size_t i = 0; i < size; i++) {
      ancestors_[i] = i;
    }
  }

 private:
  size_t FindSet(const size_t& element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }
  std::vector<size_t> ancestors_;
  std::vector<size_t> ranks_;
  std::vector<WType> weights_;
};

////////////////////////////////////////////////////

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

struct State {
  std::vector<int> result;
  std::vector<bool> used;
  int count;
  State(size_t vertex_count)
      : result(std::vector<int>(vertex_count, -1)),
        used(std::vector<bool>(vertex_count, false)),
        count(0) {}
};

constexpr size_t kInf = std::numeric_limits<size_t>::max();

template <typename T>
class SparseTable {
 public:
  SparseTable(const std::vector<T>& vector)
      : logs_(vector.size() + 1, 0), n_(vector.size()) {
    std::vector<std::pair<T, size_t>> data(vector.size());
    for (size_t i = 0; i < vector.size(); i++) {
      data[i] = {vector[i], i};
    }
    PrecalcLogs(data.size());
    log_n_ = logs_[n_];
    sparse_.assign(log_n_ + 1, std::vector<std::pair<T, size_t>>(
                                   n_ + 1, std::pair<T, size_t>()));
    BuildSparse(data);
  }

  SparseTable() = default;

  std::pair<T, size_t> Min(size_t l, size_t r) {
    if (l == r) {
      return {kInf, -1};
    }
    size_t lg = logs_[r - l];
    std::pair<T, size_t> left = sparse_[lg][l];
    std::pair<T, size_t> right = sparse_[lg][r - (1 << lg)];
    return std::min(left, right);
  }

 private:
  std::vector<size_t> logs_;
  size_t n_;
  size_t log_n_;
  std::vector<std::vector<std::pair<T, size_t>>> sparse_;
  void BuildSparse(const std::vector<std::pair<T, size_t>>& data) {
    sparse_[0] = data;
    for (size_t k = 1; k <= log_n_; ++k) {
      for (size_t i = 0; i + (1 << k) <= n_; ++i) {
        sparse_[k][i] =
            std::min(sparse_[k - 1][i], sparse_[k - 1][i + (1 << (k - 1))]);
      }
    }
  }

  void PrecalcLogs(size_t n) {
    logs_[0] = 0;
    logs_[1] = 0;
    for (size_t i = 2; i <= n; ++i) {
      logs_[i] = logs_[i / 2] + 1;
    }
  }
};

class Solution {
 public:
  void StartDFS(size_t current, size_t height = 0) {
    order.push_back(current);
    heights.push_back(height);
    depths[current] = height;
    first[current] = order.size() - 1;
    for (size_t vertex : edges_[current]) {
      if (first[vertex] == kInf) {
        StartDFS(vertex, height + 1);
        order.push_back(current);
        heights.push_back(height);
      }
    }
  }

  size_t Query(size_t first_person, size_t second_person) {
    auto [depth, index] =
        table.Min(std::min(first[first_person], first[second_person]),
                  std::max(first[first_person], first[second_person]) + 1);
    return depth;
  }

  explicit Solution(std::vector<std::set<size_t>> edges, size_t start)
      : edges_(std::move(edges)) {
    first.assign(edges_.size(), kInf);
    depths.assign(edges_.size(), kInf);
    StartDFS(start);
    table = SparseTable(heights);
  }

 private:
  std::vector<std::set<size_t>> edges_;
  std::vector<size_t> first;
  std::vector<size_t> depths;
  std::vector<size_t> order;
  std::vector<size_t> heights;
  SparseTable<size_t> table;
};

template <class VType, class EType>
void GetNeed(
    Graph<VType, EType>& graph,
    std::unordered_map<
        size_t, std::unordered_map<size_t, std::pair<size_t, size_t>>>& dict,
    std::set<size_t>& avoid, std::vector<std::pair<size_t, size_t>>& edges,
    size_t dest) {
  State state(graph.VertexCount());
  for (VType vertex : graph.Vertices()) {
    if (!state.used[vertex]) {
      DFS(graph, state, vertex, dict, avoid);
      state.count++;
    }
  }
  std::vector<std::set<size_t>> condensed(state.count, std::set<size_t>());
  for (size_t id : avoid) {
    VType start = edges[id].first;
    VType end = edges[id].second;
    condensed[state.result[start]].insert(state.result[end]);
    condensed[state.result[end]].insert(state.result[start]);
  }
  Solution solution(condensed, state.result[dest]);
  size_t q;
  std::cin >> q;
  for (size_t i = 0; i < q; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    --first;
    --second;
    std::cout << solution.Query(state.result[first], state.result[second])
              << '\n';
  }
}

template <class VType, class EType>
void DFS(
    Graph<VType, EType>& graph, State& state, VType current,
    std::unordered_map<
        size_t, std::unordered_map<size_t, std::pair<size_t, size_t>>>& dict,
    std::set<size_t>& avoid) {
  state.result[current] = state.count;
  state.used[current] = true;
  for (VType vertex : graph.GetNeighbors(current)) {
    if (!state.used[vertex]) {
      if (avoid.find(dict[current][vertex].first) == avoid.end()) {
        DFS(graph, state, vertex, dict, avoid);
      }
    }
  }
}

////////////////////////

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  size_t finish;
  std::cin >> vertex_count >> edge_count >> finish;
  --finish;
  std::vector<size_t> vertices(vertex_count);
  std::iota(vertices.begin(), vertices.end(), 0);
  std::vector<std::pair<size_t, size_t>> edges;
  std::unordered_map<size_t,
                     std::unordered_map<size_t, std::pair<size_t, size_t>>>
      dict;
  for (size_t i = 0; i < edge_count; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    --first;
    --second;
    edges.emplace_back(first, second);
    dict[first][second].first = i;
    dict[second][first].first = i;
    dict[first][second].second++;
    dict[second][first].second++;
  }
  ListGraph<size_t> graph(vertices, edges);
  std::vector<std::pair<size_t, size_t>> bridges = GetBridges(graph);
  std::vector<size_t> result;
  std::set<size_t> set;
  for (auto edge : bridges) {
    set.insert(dict[edge.first][edge.second].first);
  }
  GetNeed(graph, dict, set, edges, finish);
  return 0;
}