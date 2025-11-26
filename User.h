#include <iostream>
using std::string;

class User {
private:
    string name;
    string publicKey;
    int balance;
public:

    User() : name(""), publicKey(""), balance(0) {}
    User(string name_, string publicKey_, int balance_) {name = name_, publicKey = publicKey_, balance = balance_; }
    void setName(string name_) {name = name_; }
    void setPublicKey(string publicKey_) {publicKey = publicKey_; }
    void setBalance(int balance_) {balance = balance_; }

    string getName() const {return name; }
    string getPublicKey() const {return publicKey; }
    int getBalance() const {return balance; }
};
