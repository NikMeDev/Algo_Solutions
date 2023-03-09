#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

constexpr size_t kInf = std::numeric_limits<size_t>::max();

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

struct Edge {
  size_t from;
  size_t to;
  size_t weight;
  Edge(size_t from, size_t to, size_t weight)
      : from(from), to(to), weight(weight) {}
  bool operator<(const Edge& rhs) const { return weight > rhs.weight; }
};

void DFS(size_t begin,
         std::vector<std::vector<std::pair<size_t, size_t>>>& edges,
         std::vector<size_t>& result, size_t current_min = kInf) {
  for (auto edge : edges[begin]) {
    if(result[edge.first] == kInf) {
      size_t new_min = std::min(current_min, edge.second);
      result[edge.first] = new_min;
      DFS(edge.first, edges, result, new_min);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  size_t query_count;
  std::cin >> vertex_count >> edge_count >> query_count;
  std::vector<Edge> edges;
  std::vector<std::vector<std::pair<size_t, size_t>>> spanning_tree(
      vertex_count, std::vector<std::pair<size_t, size_t>>());
  for (size_t i = 0; i < edge_count; i++) {
    size_t from;
    size_t to;
    size_t weight;
    std::cin >> from >> to >> weight;
    --from;
    --to;
    edges.emplace_back(from, to, weight);
  }
  std::sort(edges.begin(), edges.end());
  WeightedDSU<size_t, size_t> dsu(vertex_count);
  for (Edge edge : edges) {
    if (!dsu.AreSame(edge.from, edge.to)) {
      dsu.Unite(edge.from, edge.to, edge.weight);
      spanning_tree[edge.from].emplace_back(edge.to, edge.weight);
      spanning_tree[edge.to].emplace_back(edge.from, edge.weight);
    }
  }
  std::vector<std::vector<size_t>> results(
      vertex_count, std::vector<size_t>(vertex_count, kInf));
  for(size_t i = 0; i < vertex_count; i++) {
    DFS(i, spanning_tree, results[i]);
  }
  for(size_t i = 0; i < query_count; i++) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    --first;
    --second;
    std::cout << results[first][second] << '\n';
  }
  return 0;
}