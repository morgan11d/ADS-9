// Copyright 2022 NNTU-CS
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "tree.h"

struct Row {
  int n;
  double allMs;
  double perm1Ms;
  double perm2Ms;
};

std::uint64_t factorial(int n) {
  std::uint64_t result = 1;
  for (int i = 2; i <= n; ++i) {
    result *= static_cast<std::uint64_t>(i);
  }
  return result;
}

std::string toString(const std::vector<char>& value) {
  return std::string(value.begin(), value.end());
}

int main() {
  std::vector<char> in = {'1', '2', '3'};
  PMTree tree(in);
  std::vector<std::vector<char>> perms = getAllPerms(tree);
  std::cout << "All permutations:" << std::endl;
  for (const auto& perm : perms) {
    std::cout << toString(perm) << std::endl;
  }
  std::cout << "getPerm1(1): " << toString(getPerm1(tree, 1)) << std::endl;
  std::cout << "getPerm2(2): " << toString(getPerm2(tree, 2)) << std::endl;

  std::vector<Row> rows;
  std::mt19937 generator(12345);
  volatile std::size_t guard = 0;

  for (int n = 2; n <= 9; ++n) {
    std::vector<char> alphabet;
    for (int i = 0; i < n; ++i) {
      alphabet.push_back(static_cast<char>('1' + i));
    }
    PMTree experimentTree(alphabet);
    int total = static_cast<int>(factorial(n));
    std::uniform_int_distribution<int> distribution(1, total);
    std::vector<int> numbers;
    for (int i = 0; i < 25; ++i) {
      numbers.push_back(distribution(generator));
    }

    auto startAll = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<char>> all = getAllPerms(experimentTree);
    auto finishAll = std::chrono::high_resolution_clock::now();
    guard += all.size();
    double allMs = std::chrono::duration<double, std::milli>(
        finishAll - startAll).count();

    auto startPerm1 = std::chrono::high_resolution_clock::now();
    for (int number : numbers) {
      std::vector<char> value = getPerm1(experimentTree, number);
      guard += value.size();
    }
    auto finishPerm1 = std::chrono::high_resolution_clock::now();
    double perm1Ms = std::chrono::duration<double, std::milli>(
        finishPerm1 - startPerm1).count() / numbers.size();

    int repeats = 2000;
    auto startPerm2 = std::chrono::high_resolution_clock::now();
    for (int repeat = 0; repeat < repeats; ++repeat) {
      for (int number : numbers) {
        std::vector<char> value = getPerm2(experimentTree, number);
        guard += value.size();
      }
    }
    auto finishPerm2 = std::chrono::high_resolution_clock::now();
    double perm2Ms = std::chrono::duration<double, std::milli>(
        finishPerm2 - startPerm2).count() / (numbers.size() * repeats);

    rows.push_back({n, allMs, perm1Ms, perm2Ms});
    std::cout << n << " " << allMs << " " << perm1Ms << " "
              << perm2Ms << std::endl;
  }

  std::ofstream csv("result/data.csv");
  csv << "n,getAllPerms_ms,getPerm1_ms,getPerm2_ms\n";
  for (const Row& row : rows) {
    csv << row.n << "," << row.allMs << "," << row.perm1Ms << ","
        << row.perm2Ms << "\n";
  }
  std::cout << "guard: " << guard << std::endl;
  return 0;
}
