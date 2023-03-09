#include <algorithm>
#include <iostream>
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
      costs_[first_set] = std::min(GetCost(first_set), GetCost(second_set));
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
  WType GetCost(const size_t& element) { return costs_[FindSet(element)]; }
  WeightedDSU(size_t size, std::vector<WType> costs)
      : costs_(std::move(costs)) {
    weights_.assign(size, 0);
    ancestors_.assign(size, 0);
    ranks_.assign(size, 0);
    for (size_t i = 0; i < size; i++) {
      ancestors_[i] = i;
    }
  }

  size_t FindSet(const size_t& element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }

 private:
  std::vector<size_t> ancestors_;
  std::vector<size_t> ranks_;
  std::vector<WType> weights_;
  std::vector<WType> costs_;
};

struct Edge {
  size_t from;
  size_t to;
  size_t weight;
  Edge(size_t from, size_t to, size_t weight)
      : from(from), to(to), weight(weight) {}
  bool operator<(const Edge& rhs) const { return weight < rhs.weight; }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  std::cin >> vertex_count;
  std::vector<Edge> edges;
  for (size_t i = 0; i < vertex_count; i++) {
    for (size_t j = 0; j < vertex_count; j++) {
      size_t weight;
      std::cin >> weight;
      if (j > i) {
        edges.emplace_back(i, j, weight);
      }
    }
  }
  std::sort(edges.begin(), edges.end());
  std::vector<size_t> costs(vertex_count);
  for (size_t i = 0; i < vertex_count; i++) {
    std::cin >> costs[i];
  }
  WeightedDSU<size_t, size_t> dsu(vertex_count, costs);
  for (Edge edge : edges) {
    if (!dsu.AreSame(edge.from, edge.to)) {
      size_t first_cost = dsu.GetCost(edge.from);
      size_t second_cost = dsu.GetCost(edge.to);
      if (std::max(first_cost, second_cost) >= edge.weight) {
        dsu.Unite(edge.from, edge.to, edge.weight);
      }
    }
  }
  size_t result = 0;
  std::set<size_t> used;
  for (size_t i = 0; i < vertex_count; i++) {
    size_t set = dsu.FindSet(i);
    if (!used.contains(set)) {
      used.insert(set);
      result += dsu.GetCost(set) + dsu.GetWeight(set);
    }
  }
  std::cout << result;
  return 0;
}