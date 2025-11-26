#include "Block.h"
#include "customGenerator.h"
#include <iostream>
#include <iomanip>
#include <atomic>

Block::Block(const string& prevHash, int difficulty, const vector<Transaction>& txs)
    : header(prevHash, difficulty), transactions(txs), attempts(0) {
    buildMerkleTree();
}

void Block::buildMerkleTree() {
    if (transactions.empty()) {
        header.setMerkleRoot("0000000000000000000000000000000000000000000000000000000000000000");
        return;
    }

    vector<string> txData;
    for (const auto& tx : transactions) {
        txData.push_back(tx.getTransactionId());
    }

    MerkleTree tree(txData);
    string merkleRoot = tree.getRootHash();

    header.setMerkleRoot(merkleRoot);
}

string Block::calculateBlockHash() {
    HashGenerator hasher;
    string data = header.getPrevBlockHash() +
                  to_string(header.getTimestamp()) +
                  to_string(header.getVersion()) +
                  header.getMerkleRoot() +
                  to_string(header.getNonce()) +
                  to_string(header.getDifficulty());
    return hasher.generateHash(data);
}

bool Block::mineBlockParallel(atomic<bool>& stopFlag, int maxAttempts) {
    int attemptsThisRound = 0;
    while (!stopFlag.load() && attemptsThisRound < maxAttempts) {
        blockHash = calculateBlockHash();
        attempts++;
        attemptsThisRound++;

        if (isHashValid(blockHash, header.getDifficulty())) {
            return true;  
        }

        header.incrementNonce();
    }
    return false;  
}

void Block::mineBlock() {
    header.resetNonce();
    attempts = 0;
    
    cout << "Mining block..." << endl;

    while (true) {
        blockHash = calculateBlockHash();
        attempts++;

        if (isHashValid(blockHash, header.getDifficulty())) {
            cout << "Block mined! Nonce: " << header.getNonce()
                 << " (after " << attempts << " attempts)" << endl;
            break;
        }

        header.incrementNonce();
    }
}

bool Block::isHashValid(const string& hash, int difficulty) const {
    if (difficulty <= 0 || difficulty > hash.length()) {
        return false;
    }

    for (int i = 0; i < difficulty; i++) {
        if (hash[i] != '0') {
            return false;
        }
    }
    return true;
}

void Block::print() const {
    cout << "\n================================" << endl;
    cout << "Block Hash: " << blockHash << endl;
    cout << "Previous Hash: " << header.getPrevBlockHash() << endl;
    cout << "Merkle Root: " << header.getMerkleRoot() << endl;
    cout << "Timestamp: " << header.getTimestamp() << endl;
    cout << "Difficulty: " << header.getDifficulty() << endl;
    cout << "Nonce: " << header.getNonce() << endl;
    cout << "Attempts: " << attempts << endl;
    cout << "Transactions: " << transactions.size() << endl;
    cout << "================================\n" << endl;
}