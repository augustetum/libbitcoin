#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include "../User.h"
#include "../customGenerator.h"
using std::string;

const int USER_AMOUNT = 1000;

int main() {
    std::ofstream userFile("users.txt");
    std::mt19937 gen(time(NULL)); 
    std::uniform_int_distribution<> distr(100, 1000000);
    std::ostringstream buferis; 
    HashGenerator hasher;

    for(int i = 1; i <= USER_AMOUNT; i++){
        
        string name = "Name" + std::to_string(i);
        string publicKey = hasher.generateHash(std::to_string(i)); 
        int balance = distr(gen);
        User user = User(name, publicKey, balance);
        
        buferis << std::left << std::setw(20) << user.getName() << std::setw(70) << user.getPublicKey() << std::setw(20) << user.getBalance() << std::endl;
    }

    userFile << buferis.str();
    userFile.close();
}