#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

static constexpr uint64_t kGoal =
    0b0001'0010'0011'0100'0101'0110'0111'1000'0000ull;
static constexpr uint64_t kInf = std::numeric_limits<uint64_t>::max();

uint64_t Hash(std::vector<uint64_t>& field) {
  uint64_t hash = 0;
  for (uint64_t number : field) {
    hash <<= 4;
    hash += number;
  }
  return hash;
}

template <size_t field_size>
uint64_t Get(uint64_t element, uint64_t index) {
  return (element >> ((field_size * field_size - index - 1) * 4)) & 0b1111ull;
}

template <size_t field_size>
void Set(uint64_t& element, uint64_t index, uint64_t what) {
  element =
      (element & ~(0b1111ull << ((field_size * field_size - index - 1) * 4))) +
      (what << ((field_size * field_size - index - 1) * 4));
}

template <size_t field_size>
void Swap(uint64_t& element, uint64_t first, uint64_t second) {
  uint64_t temp = Get<field_size>(element, first);
  Set<field_size>(element, first, Get<field_size>(element, second));
  Set<field_size>(element, second, temp);
}

template <size_t field_size>
uint64_t GetZeroIndex(uint64_t field) {
  for (size_t i = 0; i < field_size * field_size; i++) {
    if (Get<field_size>(field, i) == 0) {
      return i;
    }
  }
  return 0;
}

template <size_t field_size>
bool CanGetUp(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  return zero_index / field_size > 0;
}

template <size_t field_size>
bool CanGetDown(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  return zero_index / field_size + 1 != field_size;
}

template <size_t field_size>
bool CanGetLeft(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  return zero_index % field_size > 0;
}

template <size_t field_size>
bool CanGetRight(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  return zero_index % field_size + 1 != field_size;
}

template <size_t field_size>
uint64_t MoveUp(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  uint64_t new_index = zero_index - field_size;
  Swap<field_size>(field, zero_index, new_index);
  return field;
}

template <size_t field_size>
uint64_t MoveDown(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  uint64_t new_index = zero_index + field_size;
  Swap<field_size>(field, zero_index, new_index);
  return field;
}

template <size_t field_size>
uint64_t MoveLeft(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  uint64_t new_index = zero_index - 1;
  Swap<field_size>(field, zero_index, new_index);
  return field;
}

template <size_t field_size>
uint64_t MoveRight(uint64_t field) {
  uint64_t zero_index = GetZeroIndex<field_size>(field);
  uint64_t new_index = zero_index + 1;
  Swap<field_size>(field, zero_index, new_index);
  return field;
}

template <size_t field_size>
uint64_t ManhattanDistance(uint64_t field) {
  uint64_t result = 0;
  for (size_t i = 0; i < field_size * field_size; i++) {
    uint64_t number = Get<field_size>(field, i);
    if (number != 0) {
      result += std::max((number - 1) / field_size, i / field_size) -
                std::min((number - 1) / field_size, i / field_size);
      result += std::max((number - 1) % field_size, i % field_size) -
                std::min((number - 1) % field_size, i % field_size);
    }
  }
  return result;
}

template <size_t field_size>
uint64_t NotInOrder(uint64_t field) {
  uint64_t result = 0;
  for (size_t i = 0; i < field_size * field_size; i++) {
    uint64_t number = Get<field_size>(field, i);
    if (number != 0) {
      result += (number - 1) != i;
    }
  }
  return result;
}

template <size_t field_size>
uint64_t Heuristic(uint64_t field) {
  return NotInOrder<field_size>(field);
}

template <size_t field_size>
std::optional<std::unordered_map<uint64_t, std::pair<uint64_t, char>>> AStar(
    uint64_t start) {
  typedef std::pair<uint64_t, uint64_t> Ullpair;
  std::priority_queue<Ullpair, std::vector<Ullpair>, std::greater<>> queue;
  std::set<uint64_t> visited;
  std::unordered_map<uint64_t, uint64_t> results;
  queue.push({Heuristic<field_size>(start), start});
  results[start] = 0;
  std::unordered_map<uint64_t, std::pair<uint64_t, char>> parents;
  while (!queue.empty()) {
    auto[priority, current] = queue.top();
    if (current == kGoal) {
      return parents;
    }
    queue.pop();
    if (priority == results[current] + Heuristic<field_size>(current)) {
      visited.insert(current);
      if (CanGetUp<field_size>(current)) {
        uint64_t neighbour = MoveUp<field_size>(current);
        if (visited.find(neighbour) == visited.end() ||
            results[neighbour] > results[current] + 1) {
          results[neighbour] = results[current] + 1;
          parents[neighbour] = {current, 'U'};
          queue.push({results[neighbour] + Heuristic<field_size>(neighbour),
                      neighbour});
        }
      }
      if (CanGetDown<field_size>(current)) {
        uint64_t neighbour = MoveDown<field_size>(current);
        if (visited.find(neighbour) == visited.end() ||
            results[neighbour] > results[current] + 1) {
          results[neighbour] = results[current] + 1;
          parents[neighbour] = {current, 'D'};
          queue.push({results[neighbour] + Heuristic<field_size>(neighbour),
                      neighbour});
        }
      }
      if (CanGetLeft<field_size>(current)) {
        uint64_t neighbour = MoveLeft<field_size>(current);
        if (visited.find(neighbour) == visited.end() ||
            results[neighbour] > results[current] + 1) {
          results[neighbour] = results[current] + 1;
          parents[neighbour] = {current, 'L'};
          queue.push({results[neighbour] + Heuristic<field_size>(neighbour),
                      neighbour});
        }
      }
      if (CanGetRight<field_size>(current)) {
        uint64_t neighbour = MoveRight<field_size>(current);
        if (visited.find(neighbour) == visited.end() ||
            results[neighbour] > results[current] + 1) {
          results[neighbour] = results[current] + 1;
          parents[neighbour] = {current, 'R'};
          queue.push({results[neighbour] + Heuristic<field_size>(neighbour),
                      neighbour});
        }
      }
    }
  }
  return std::nullopt;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::vector<size_t> field(9);
  for (size_t i = 0; i < 9; i++) {
    std::cin >> field[i];
  }
  uint64_t begin = Hash(field);
  auto result = AStar<3>(begin);
  if (result.has_value()) {
    std::string answer;
    uint64_t current = kGoal;
    while (current != begin) {
      auto[next, new_char] = result.value()[current];
      current = next;
      answer += new_char;
    }
    std::reverse(answer.begin(), answer.end());
    std::cout << answer.length() << '\n' << answer;
  } else {
    std::cout << -1;
  }
  return 0;
}