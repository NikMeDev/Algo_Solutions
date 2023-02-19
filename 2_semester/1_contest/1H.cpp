#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

static constexpr size_t kInfty = std::numeric_limits<size_t>::max();

struct State {
  size_t time;
  std::vector<bool> used;
  std::vector<size_t> time_in;
  std::vector<size_t> time_up;
  std::set<size_t> result;
  State(size_t vertex_count)
      : time(0),
        used(std::vector<bool>(vertex_count, false)),
        time_in(std::vector<size_t>(vertex_count, kInfty)),
        time_up(std::vector<size_t>(vertex_count, kInfty)) {}
};

void DFS(size_t current, size_t parent, std::vector<std::vector<size_t>>& graph,
         State& state, bool root = true) {
  state.used[current] = true;
  state.time_in[current] = state.time++;
  state.time_up[current] = state.time_in[current];
  size_t children_count = 0;
  for (size_t vertex : graph[current]) {
    if (root || vertex != parent) {
      if (!state.used[vertex]) {
        DFS(vertex, current, graph, state, false);
        state.time_up[current] =
            std::min(state.time_up[current], state.time_up[vertex]);
        if (state.time_up[vertex] >= state.time_in[current] && !root) {
          state.result.insert(current);
        }
        children_count++;
      } else {
        state.time_up[current] =
            std::min(state.time_up[current], state.time_in[vertex]);
      }
    }
  }
  if (root && children_count >= 2) {
    state.result.insert(current);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t vertex_count;
  size_t edge_count;
  std::cin >> vertex_count >> edge_count;
  std::vector<std::vector<size_t>> graph(vertex_count);
  for (size_t i = 0; i < edge_count; i++) {
    size_t start;
    size_t end;
    std::cin >> start >> end;
    start--;
    end--;
    graph[start].push_back(end);
    graph[end].push_back(start);
  }
  State state(vertex_count);
  for (size_t i = 0; i < vertex_count; i++) {
    if (!state.used[i]) {
      DFS(i, 0, graph, state);
    }
  }
  std::cout << state.result.size() << '\n';
  for (size_t vertex : state.result) {
    std::cout << vertex + 1 << '\n';
  }
  return 0;
}