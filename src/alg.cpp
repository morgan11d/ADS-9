#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include "tree.h"

namespace {
std::uint64_t fact(std::size_t n) {
  std::uint64_t result = 1;
  for (std::size_t i = 2; i <= n; ++i) {
    if (result > std::numeric_limits<std::uint64_t>::max() / i) {
      return std::numeric_limits<std::uint64_t>::max();
    }
    result *= i;
  }
  return result;
}

void collectAll(PMNode* node,
                std::vector<char>* current,
                std::vector<std::vector<char>>* result) {
  current->push_back(node->value);
  if (node->children.empty()) {
    result->push_back(*current);
  } else {
    for (PMNode* child : node->children) {
      collectAll(child, current, result);
    }
  }
  current->pop_back();
}

bool collectNumber(PMNode* node,
                   int num,
                   int* currentNumber,
                   std::vector<char>* current,
                   std::vector<char>* result) {
  current->push_back(node->value);
  if (node->children.empty()) {
    ++(*currentNumber);
    if (*currentNumber == num) {
      *result = *current;
      current->pop_back();
      return true;
    }
  } else {
    for (PMNode* child : node->children) {
      if (collectNumber(child, num, currentNumber, current, result)) {
        current->pop_back();
        return true;
      }
    }
  }
  current->pop_back();
  return false;
}
}  // namespace

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> result;
  std::vector<char> current;
  for (PMNode* child : tree.root()->children) {
    collectAll(child, &current, &result);
  }
  return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  if (num <= 0) {
    return {};
  }
  std::vector<char> current;
  std::vector<char> result;
  int currentNumber = 0;
  for (PMNode* child : tree.root()->children) {
    if (collectNumber(child, num, &currentNumber, &current, &result)) {
      return result;
    }
  }
  return {};
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  if (num <= 0) {
    return {};
  }
  std::uint64_t total = fact(tree.size());
  if (static_cast<std::uint64_t>(num) > total) {
    return {};
  }
  std::uint64_t index = static_cast<std::uint64_t>(num - 1);
  std::vector<char> result;
  PMNode* node = tree.root();
  while (!node->children.empty()) {
    std::uint64_t block = fact(node->children.size() - 1);
    std::size_t childIndex = static_cast<std::size_t>(index / block);
    if (childIndex >= node->children.size()) {
      return {};
    }
    node = node->children[childIndex];
    result.push_back(node->value);
    index %= block;
  }
  return result;
}
