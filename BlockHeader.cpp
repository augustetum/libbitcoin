#include "BlockHeader.h"

BlockHeader::BlockHeader(std::string prev, int diff, int ver)
    : prevBlockHash(prev), difficulty(diff), version(ver), nonce(0), merkle_root("") {
    timestamp = time(nullptr);
}

void BlockHeader::setVersion(int v) { version = v; }
void BlockHeader::setMerkleRoot(std::string mr) { merkle_root = mr; }
void BlockHeader::incrementNonce() { nonce++; }
void BlockHeader::resetNonce() { nonce = 0; }
void BlockHeader::setNonce(int nc) { nonce = nc; }