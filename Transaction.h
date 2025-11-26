#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using std::string;

class Transaction {
private:
    string senderKey;           
    string receiverKey; 
    double amount;        

public:
    string transactionID;

    Transaction(const string& from, const string& to, double amt);
    
    string getSender() const;
    string getReceiver() const;
    double getAmount() const;
    string getTransactionId() const;
    
    string calculateTransactionId() const;

    void print() const;
};

#endif