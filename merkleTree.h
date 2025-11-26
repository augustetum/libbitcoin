#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <vector>
#include <string>
#include <memory>
#include <optional>

class MerkleTree
{
public:
    struct Node
    {
        std::string hash;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(const std::string &h) : hash(h), left(nullptr), right(nullptr) {}
        Node(const std::string &h, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
            : hash(h), left(l), right(r) {}
    };

    MerkleTree(const std::vector<std::string> &data);

    std::string getRootHash() const;
    std::vector<std::string> getProof(size_t index) const;
    bool verify(const std::string &data, const std::vector<std::string> &proof, const std::string &rootHash) const;
    void printTree() const;

private:
    std::shared_ptr<Node> root;
    std::vector<std::string> leaves;

    std::string hash(const std::string &data) const;
    std::string combineHash(const std::string &left, const std::string &right) const;
    std::shared_ptr<Node> buildTree(const std::vector<std::string> &hashes);
    void printNode(const std::shared_ptr<Node> &node, int depth) const;
    void getProofHelper(const std::shared_ptr<Node> &node, size_t index, size_t start, size_t end,
                        std::vector<std::string> &proof) const;
};

#endif
