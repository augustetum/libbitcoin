#include "merkleTree.h"
#include "customGenerator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <openssl/sha.h>

MerkleTree::MerkleTree(const std::vector<std::string> &data)
{
    if (data.empty())
    {
        throw std::invalid_argument("Data cannot be empty");
    }

    for (const auto &item : data)
    {
        leaves.push_back(hash(item));
    }
    root = buildTree(leaves);
}

std::string MerkleTree::hash(const std::string &data) const
{
    HashGenerator hash;
    std::string hashed = hash.generateHash(data);
    return hashed;
}

std::string MerkleTree::combineHash(const std::string &left, const std::string &right) const
{
    return hash(left + right);
}

std::shared_ptr<MerkleTree::Node> MerkleTree::buildTree(const std::vector<std::string> &hashes)
{
    if (hashes.empty())
    {
        return nullptr;
    }

    if (hashes.size() == 1)
    {
        return std::make_shared<Node>(hashes[0]);
    }

    std::vector<std::shared_ptr<Node>> level;

    // Create leaf nodes
    for (const auto &h : hashes)
    {
        level.push_back(std::make_shared<Node>(h));
    }

    while (level.size() > 1)
    {
        std::vector<std::shared_ptr<Node>> nextLevel;

        for (size_t i = 0; i < level.size(); i += 2)
        {
            if (i + 1 < level.size())
            {
                std::string combinedHash = combineHash(level[i]->hash, level[i + 1]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i + 1]);
                nextLevel.push_back(parent);
            }
            else
            {
                std::string combinedHash = combineHash(level[i]->hash, level[i]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i]);
                nextLevel.push_back(parent);
            }
        }

        level = nextLevel;
    }

    return level[0];
}

std::string MerkleTree::getRootHash() const
{
    return root ? root->hash : "";
}

void MerkleTree::getProofHelper(const std::shared_ptr<Node> &node, size_t index,
                                size_t start, size_t end, std::vector<std::string> &proof) const
{
    if (!node || start == end)
    {
        return;
    }

    size_t mid = (start + end) / 2;

    if (index <= mid)
    {
        if (node->right)
        {
            proof.push_back(node->right->hash);
        }
        getProofHelper(node->left, index, start, mid, proof);
    }
    else
    {
        if (node->left)
        {
            proof.push_back(node->left->hash);
        }
        getProofHelper(node->right, index, mid + 1, end, proof);
    }
}

std::vector<std::string> MerkleTree::getProof(size_t index) const
{
    if (index >= leaves.size())
    {
        throw std::out_of_range("Index out of range");
    }

    std::vector<std::string> proof;
    getProofHelper(root, index, 0, leaves.size() - 1, proof);
    return proof;
}

bool MerkleTree::verify(const std::string &data, const std::vector<std::string> &proof,
                        const std::string &rootHash) const
{
    std::string currentHash = hash(data);

    for (const auto &proofHash : proof)
    {
        std::string hash1 = combineHash(currentHash, proofHash);
        std::string hash2 = combineHash(proofHash, currentHash);

        currentHash = hash1;
    }

    return currentHash == rootHash;
}

void MerkleTree::printNode(const std::shared_ptr<Node> &node, int depth) const
{
    if (!node)
        return;

    std::string indent(depth * 2, ' ');
    std::cout << indent << node->hash << " (depth: " << depth << ")" << std::endl;
    std::cout << std::endl;
    if (node->left)
        printNode(node->left, depth + 1);
    if (node->right && node->right != node->left)
        printNode(node->right, depth + 1);
}

void MerkleTree::printTree() const
{
    std::cout << "Merkle Tree Structure:" << std::endl;
    printNode(root, 0);
}