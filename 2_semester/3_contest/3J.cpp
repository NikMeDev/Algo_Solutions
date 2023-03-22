#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

constexpr int kInf = std::numeric_limits<int>::max();

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
class DSU<int> {
 public:
  void Unite(int first, int second) {
    int first_set = FindSet(first);
    int second_set = FindSet(second);
    if (first_set != second_set) {
      if (ranks_[first_set] < ranks_[second_set]) {
        std::swap(first_set, second_set);
      }
      ancestors_[second_set] = first_set;
      min_[first_set] = min_[second_set] =
          std::min(min_[first_set], min_[second_set]);
      if (ranks_[first_set] == ranks_[second_set]) {
        ++ranks_[first_set];
      }
    }
  }
  bool AreSame(int first, int second) {
    return FindSet(first) == FindSet(second);
  }
  int Min(int element) { return min_[FindSet(element)]; }
  DSU(int size) {
    ancestors_.resize(size);
    min_.resize(size);
    ranks_.assign(size, 0);
    for (int i = 0; i < size; i++) {
      ancestors_[i] = i;
      min_[i] = i;
    }
  }

 private:
  int FindSet(int element) {
    if (element == ancestors_[element]) {
      return element;
    }
    return ancestors_[element] = FindSet(ancestors_[element]);
  }
  std::vector<int> ancestors_;
  std::vector<int> ranks_;
  std::vector<int> min_;
};

class Solution {
 public:
  enum Types { Add, Remove, Ask };

  struct QueryType {
    Types type;
    int parent;
    int child;
  };

  int LCA(int v, int u) {
    if (d[v] > d[u]) {
      std::swap(v, u);
    }
    for (int i = std::log2(vertex_count); i >= 0; i--) {
      if (d[dp[u][i]] - d[v] >= 0) {
        u = dp[u][i];
      }
    }
    if (v == u) {
      return v;
    }
    for (int i = std::log2(vertex_count); i >= 0; i--) {
      if (dp[v][i] != dp[u][i]) {
        v = dp[v][i];
        u = dp[u][i];
      }
    }
    return p[v];
  }

  explicit Solution() {
    int query_count;
    std::cin >> query_count;
    std::vector<QueryType> queries;
    p[0] = 0;
    d[0] = 0;
    for (int i = 0; i < query_count; i++) {
      char type;
      std::cin >> type;
      if (type == '+') {
        int parent;
        std::cin >> parent;
        --parent;
        queries.push_back({Add, parent, vertex_count});
        p[vertex_count] = parent;
        d[vertex_count] = d[p[vertex_count]] + 1;
        ++vertex_count;
      } else if (type == '-') {
        int child;
        std::cin >> child;
        --child;
        queries.push_back({Remove, p[child], child});
      } else {
        int begin;
        int end;
        std::cin >> begin >> end;
        --begin;
        --end;
        queries.push_back({Ask, begin, end});
      }
    }
    dp.assign(vertex_count, std::vector<int>(std::log2(vertex_count) + 1));
    for (int i = 0; i < vertex_count; i++) {
      dp[i][0] = p[i];
    }
    for (int j = 1; j <= std::log2(vertex_count); j++) {
      for (int i = 0; i < vertex_count; i++) {
        dp[i][j] = dp[dp[i][j - 1]][j - 1];
      }
    }
    DSU<int> dsu(vertex_count);
    for (auto query : queries) {
      if (query.type == Remove) {
        dsu.Unite(query.parent, query.child);
      } else if (query.type == Ask) {
        std::cout << dsu.Min(LCA(query.parent, query.child)) + 1 << std::endl;
      }
    }
  }

 private:
  std::unordered_map<int, int> p;
  std::unordered_map<int, int> d;
  std::vector<std::vector<int>> dp;
  int vertex_count = 1;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  Solution solution;
  return 0;
}