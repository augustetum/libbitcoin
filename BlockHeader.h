#ifndef BLOCKHEADER_H
#define BLOCKHEADER_H

#include <string>
#include <ctime>
using std::string;

class BlockHeader {
private:
    std::string prevBlockHash;
    time_t timestamp;
    int version;
    std::string merkle_root;
    int nonce;
    int difficulty;

public:
    BlockHeader(string prev, int diff, int ver = 1);

    void setVersion(int v);
    void setMerkleRoot(string mr);
    void incrementNonce();
    void resetNonce();
    void setNonce(int nc);

    string getPrevBlockHash() const { return prevBlockHash; }
    time_t getTimestamp() const { return timestamp; }
    int getVersion() const { return version; }
    string getMerkleRoot() const { return merkle_root; }
    int getNonce() const { return nonce; }
    int getDifficulty() const { return difficulty; }
};

#endif
