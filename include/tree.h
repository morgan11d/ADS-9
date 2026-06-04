// Copyright 2022 NNTU-CS
#ifndef TREE_H
#define TREE_H

#include <vector>
#include <algorithm>
#include <cstddef>

struct PMNode {
    char value;
    std::vector<PMNode*> children;

    explicit PMNode(char value = 0) : value(value) {}

    ~PMNode() {
        for (PMNode* child : children) {
            delete child;
        }
    }

    PMNode(const PMNode&) = delete;
    PMNode& operator=(const PMNode&) = delete;
};

class PMTree {
private:
    PMNode* root_;
    std::vector<char> alphabet_;

    void build(PMNode* node, const std::vector<char>& values) {
        for (std::size_t i = 0; i < values.size(); ++i) {
            PMNode* child = new PMNode(values[i]);
            node->children.push_back(child);
            std::vector<char> rest;
            rest.reserve(values.size() - 1);
            for (std::size_t j = 0; j < values.size(); ++j) {
                if (i != j) {
                    rest.push_back(values[j]);
                }
            }
            build(child, rest);
        }
    }

public:
    explicit PMTree(std::vector<char> values) : root_(new PMNode()), alphabet_(std::move(values)) {
        std::sort(alphabet_.begin(), alphabet_.end());
        build(root_, alphabet_);
    }

    ~PMTree() {
        delete root_;
    }

    PMTree(const PMTree&) = delete;
    PMTree& operator=(const PMTree&) = delete;

    PMTree(PMTree&& other) noexcept : root_(other.root_), alphabet_(std::move(other.alphabet_)) {
        other.root_ = nullptr;
    }

    PMTree& operator=(PMTree&& other) noexcept {
        if (this != &other) {
            delete root_;
            root_ = other.root_;
            alphabet_ = std::move(other.alphabet_);
            other.root_ = nullptr;
        }
        return *this;
    }

    PMNode* root() {
        return root_;
    }

    const PMNode* root() const {
        return root_;
    }

    std::size_t size() const {
        return alphabet_.size();
    }

    const std::vector<char>& alphabet() const {
        return alphabet_;
    }
};

using Tree = PMTree;

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif

  // INCLUDE_TREE_H_
