#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include "../User.h"
#include "../customGenerator.h"
#include "../Transaction.h"
using std::string;

const int TRANSACTION_AMOUNT = 10000;

std::vector<std::string> loadUsersFromFile(const std::string& filename) {
    std::vector<std::string> userPublicKeys;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return userPublicKeys;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name, publicKey;
        int balance;
        iss >> name >> publicKey >> balance;
        
        userPublicKeys.push_back(publicKey);
    }
    file.close();
    
    std::cout << "Loaded " << userPublicKeys.size() << " users from file" << std::endl;
    return userPublicKeys;
}

std::vector<Transaction> generateTransactions(const std::vector<std::string>& userPublicKeys, int count) {
    std::vector<Transaction> transactions;
    
    std::mt19937 gen(time(NULL));
    std::uniform_int_distribution<> userDist(0, userPublicKeys.size() - 1);
    std::uniform_real_distribution<> amountDist(100.0, 500000.0);
    
    for (int i = 0; i < count; ++i) {
        int senderIdx = userDist(gen);
        int receiverIdx;
        do {
            receiverIdx = userDist(gen);
        } while (receiverIdx == senderIdx);
        
        std::string sender = userPublicKeys[senderIdx];
        std::string receiver = userPublicKeys[receiverIdx];
        double amount = amountDist(gen);
        
        Transaction tx(sender, receiver, amount);
        transactions.push_back(tx);
    }
    
    return transactions;
}

void saveToFile(const std::vector<Transaction>& transactions, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }
    
    for (const auto& tx : transactions) {
          file << std::left 
            << std::setw(70) << tx.getSender() 
            << std::setw(70) << tx.getReceiver() 
            << std::setw(20) << tx.getAmount() 
            << std::endl;
    }
    
    file.close();
}

int main() {
    auto userPublicKeys = loadUsersFromFile("users.txt");
    auto transactions = generateTransactions(userPublicKeys, TRANSACTION_AMOUNT);
    saveToFile(transactions, "transactions.txt");
    return 0;
}