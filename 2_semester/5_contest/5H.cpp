#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Trie {
 public:
  static constexpr int64_t Null = -1;
  struct Node {
    Node() = default;
    Node(int64_t parent, char symbol) : parent(parent), symbol(symbol) {}

    bool HasChild(char symbol) {
      return children.find(symbol) != children.end();
    }

    std::unordered_map<char, int64_t> children;
    int64_t parent{-1};
    char symbol{};
    bool is_terminal{false};
  };

  Trie() : nodes_(1) {}

  int64_t Add(const std::string& key) {
    int64_t now = 0;
    for (auto symbol : key) {
      if (!nodes_[now].HasChild(symbol)) {
        nodes_.emplace_back(now, symbol);
        nodes_[now].children[symbol] = nodes_.size() - 1;
      }
      now = nodes_[now].children[symbol];
    }
    nodes_[now].is_terminal = true;
    return now;
  }

  Node& Get(int64_t index) {
    return nodes_.at(index);
  }

  size_t NodeCount() {
    return nodes_.size();
  }
 private:
  std::vector<Node> nodes_;
};

class AhoKorasikAutomaton {
 public:
  AhoKorasikAutomaton(const std::vector<std::string>& patterns)
      : patterns_(patterns) {
    int32_t i = 0;
    for (auto pattern : patterns) {
      pattern_mapper_[trie_.Add(pattern)] = i++;
    }
    suffix_links_.assign(trie_.NodeCount(), Trie::Null);
    terminal_links_.assign(trie_.NodeCount(), Trie::Null);
  }

  std::vector<std::string> MakeStep(char symbol) {
    state_ = CalcStep(state_, symbol);
    return ExtractMatches();
  }

  void ResetState() {
    state_ = 0;
  }

 private:
  std::vector<std::string> ExtractMatches() {
    std::vector<std::string> ans;
    size_t v = state_;
    while (v != 0) {
      auto& node = trie_.Get(v);
      if (node.is_terminal) {
        ans.push_back(patterns_[pattern_mapper_[v]]);
      }
      v = GetTerminalLink(v);
    }
    return ans;
  }

  int64_t CalcStep(int64_t node, char symbol) {
    auto& v = trie_.Get(node);
    if (v.HasChild(symbol)) {
      return v.children[symbol];
    }
    if (node == 0) {
      return 0;
    }
    return CalcStep(GetSuffixLink(node), symbol);
  }

  int64_t GetTerminalLink(int64_t node) {
    if (terminal_links_[node] != Trie::Null) {
      return terminal_links_[node];
    }
    if (node == 0 || trie_.Get(node).parent == 0) {
      terminal_links_[node] = 0;
      return 0;
    }
    int64_t v = GetSuffixLink(node);
    while (v != 0 && !trie_.Get(v).is_terminal) {
      if (terminal_links_[v] != Trie::Null) {
        v = terminal_links_[v];
        break;
      }
      v = GetSuffixLink(v);
    }
    terminal_links_[node] = v;
    return v;
  }

  int64_t GetSuffixLink(int64_t node) {
    if (suffix_links_[node] != Trie::Null) {
      return suffix_links_[node];
    }
    if (node == 0 || trie_.Get(node).parent == 0) {
      suffix_links_[node] = 0;
      return 0;
    }
    auto& v = trie_.Get(node);
    suffix_links_[node] = CalcStep(GetSuffixLink(v.parent), v.symbol);
    return suffix_links_[node];
  }

  std::vector<int64_t> suffix_links_;
  std::vector<int64_t> terminal_links_;

  std::unordered_map<int64_t, size_t> pattern_mapper_;

  int64_t state_{0};
  Trie trie_;
  std::vector<std::string> patterns_;
};

class Matcher {
 public:
  Matcher(const std::vector<std::string>& patterns)
      : automaton_(patterns) {
  }

  std::vector<std::pair<size_t,
                        std::string>> FindMatches(const std::string& text) {
    automaton_.ResetState();
    std::vector<std::pair<size_t, std::string>> matches;
    for (int64_t i = 0; i < text.size(); ++i) {
      auto step_matches = automaton_.MakeStep(text[i]);
      if (!step_matches.empty()) {
        for (auto& match : step_matches) {
          size_t pos =
              static_cast<size_t>(i - static_cast<int64_t>(match.length()) + 1);
          matches.emplace_back(pos, match);
        }
      }
    }
    return matches;
  }

 private:
  AhoKorasikAutomaton automaton_;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string text;
  std::cin >> text;
  size_t count;
  std::cin >> count;
  std::vector<std::string> patterns(count);
  std::unordered_map<std::string, std::vector<size_t>> map;
  for (size_t i = 0; i < count; i++) {
    std::cin >> patterns[i];
  }
  Matcher matcher(patterns);
  auto matches = matcher.FindMatches(text);
  for (auto& [pos, word] : matches) {
    map[word].push_back(pos + 1);
  }
  for (std::string& pattern : patterns) {
    std::vector<size_t> result = map[pattern];
    std::cout << result.size() << ' ';
    for (size_t i : result) {
      std::cout << i << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}