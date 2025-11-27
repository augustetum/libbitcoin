#include "libbitcoinMerkle.h"
#include "customGenerator.h"
#include <iostream>

LibBitcoinMerkle::LibBitcoinMerkle(const std::vector<std::string> &data)
{
    if (data.empty())
    {
        throw std::invalid_argument("Data cannot be empty");
    }

    HashGenerator hash;
    for (const auto &item : data)
    {
        leaves.push_back(hash.generateHash(item));
    }

    rootHashValue = computeMerkleRoot(leaves);

    root = buildTree(leaves);
}

std::string LibBitcoinMerkle::computeMerkleRoot(const std::vector<std::string> &data)
{
    bc::hash_list merkleList;
    for (const auto &hexStr : data)
    {
        merkleList.push_back(stringToHashDigest(hexStr));
    }

    bc::hash_digest merkleRoot = create_merkle(merkleList);

    return hashDigestToString(merkleRoot);
}

bc::hash_digest LibBitcoinMerkle::create_merkle(bc::hash_list& merkle)
{
    if (merkle.empty())
        return bc::null_hash;
    else if (merkle.size() == 1)
        return merkle[0];

    // While there is more than 1 hash in the list, keep looping...
    while (merkle.size() > 1)
    {
        // If number of hashes is odd, duplicate last hash in the list.
        if (merkle.size() % 2 != 0)
            merkle.push_back(merkle.back());

        // List size is now even.
        assert(merkle.size() % 2 == 0);

        // New hash list.
        bc::hash_list new_merkle;

        // Loop through hashes 2 at a time.
        for (auto it = merkle.begin(); it != merkle.end(); it += 2)
        {
            // Join both current hashes together (concatenate).
            bc::data_chunk concat_data(bc::hash_size * 2);
            auto concat = bc::serializer<decltype(concat_data.begin())>(concat_data.begin());
            concat.write_hash(*it);
            concat.write_hash(*(it + 1));

            // Hash both of the hashes.
            bc::hash_digest new_root = bc::bitcoin_hash(concat_data);

            // Add this to the new list.
            new_merkle.push_back(new_root);
        }

        // This is the new list.
        merkle = new_merkle;

        // DEBUG output
        std::cout << "Current merkle hash list:" << std::endl;
        for (const auto& hash: merkle)
            std::cout << " " << bc::encode_base16(hash) << std::endl;
        std::cout << std::endl;
    }

    // Finally we end up with a single item.
    return merkle[0];
}

bc::hash_digest LibBitcoinMerkle::stringToHashDigest(const std::string &hexStr) const
{
    bc::hash_digest hash;
    bc::decode_base16(hash, hexStr);
    return hash;
}

std::string LibBitcoinMerkle::hashDigestToString(const bc::hash_digest &hash) const
{
    return bc::encode_base16(hash);
}

std::string LibBitcoinMerkle::getRootHash() const
{
    return rootHashValue;
}

std::shared_ptr<LibBitcoinMerkle::Node> LibBitcoinMerkle::buildTree(const std::vector<std::string> &hashes)
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
                HashGenerator hasher;
                std::string combinedHash = hasher.generateHash(level[i]->hash + level[i + 1]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i + 1]);
                nextLevel.push_back(parent);
            }
            else
            {
                HashGenerator hasher;
                std::string combinedHash = hasher.generateHash(level[i]->hash + level[i]->hash);
                auto parent = std::make_shared<Node>(combinedHash, level[i], level[i]);
                nextLevel.push_back(parent);
            }
        }

        level = nextLevel;
    }

    return level[0];
}

void LibBitcoinMerkle::getProofHelper(const std::shared_ptr<Node> &node, size_t index,
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

std::vector<std::string> LibBitcoinMerkle::getProof(size_t index) const
{
    if (index >= leaves.size())
    {
        throw std::out_of_range("Index out of range");
    }

    std::vector<std::string> proof;
    getProofHelper(root, index, 0, leaves.size() - 1, proof);
    return proof;
}

bool LibBitcoinMerkle::verify(const std::string &data, const std::vector<std::string> &proof,
                        const std::string &rootHash) const
{
    HashGenerator hasher;
    std::string currentHash = hasher.generateHash(data);

    for (const auto &proofHash : proof)
    {
        std::string hash1 = hasher.generateHash(currentHash + proofHash);
        std::string hash2 = hasher.generateHash(proofHash + currentHash);

        currentHash = hash1;
    }

    return currentHash == rootHash;
}

void LibBitcoinMerkle::printNode(const std::shared_ptr<Node> &node, int depth) const
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

void LibBitcoinMerkle::printTree() const
{
    std::cout << "Merkle Tree Structure:" << std::endl;
    printNode(root, 0);
}
