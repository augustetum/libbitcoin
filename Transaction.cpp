#include "Transaction.h"
#include <iostream>
#include "customGenerator.h"

using namespace std;

Transaction::Transaction(const string& from, const string& to, double amt)
    : senderKey(from), receiverKey(to), amount(amt) {
    transactionID = calculateTransactionId();
}

string Transaction::getSender() const { return senderKey; }
string Transaction::getReceiver() const { return receiverKey; }
double Transaction::getAmount() const { return amount; }
string Transaction::getTransactionId() const { return transactionID; }

string Transaction::calculateTransactionId() const {
    HashGenerator hasher;
    string data = senderKey + receiverKey + to_string(amount);
    string hash = hasher.generateHash(data); 
    return hash;
}


void Transaction::print() const {
    cout << "TX: " << transactionID << endl;
    cout << "  From: " << senderKey << endl;
    cout << "  To:   " << receiverKey << endl;
    cout << "  Amount: " << amount << endl;
}