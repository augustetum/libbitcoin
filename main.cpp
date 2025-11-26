#include "customGenerator.h"
#include "merkleTree.h"
#include <iostream>
#include <vector>

int main() {
    try {
        std::vector<std::string> data = {
            "Transaction 1",
            "Transaction 2",
            "Transaction 3",
            "Transaction 4",
            "Transaction 5"
        };
        
        std::cout << "Creating Merkle Tree with " << data.size() << " transactions" << std::endl;
        
        // Create Merkle Tree
        MerkleTree tree(data);
        std::cout << std::endl;
        std::cout << "Root Hash: " << tree.getRootHash() << std::endl;
        std::cout << std::endl;

        tree.printTree();
    
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}