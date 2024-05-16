#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <string>

int random(int, int);
int handSum(std::map<std::string, std::string>, const std::vector<std::string> );
void basicLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);
void doubleLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);
void splitLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);
void dealerMove(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);
void dealerSplit(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);



int casino(int, char**){
    std::vector<std::string> deck;
    std::map<std::string, std::string> weights {
        {"2", "2"},
        {"3", "3"},
        {"4", "4"},
        {"5", "5"},
        {"6", "6"},
        {"7", "7"},
        {"8", "8"},
        {"9", "9"},
        {"T", "10"},
        {"J", "10"},
        {"Q", "10"},
        {"K", "10"},
        {"A", "11"},
    };
    for (std::pair<std::string, std::string> pair: weights) {
        for (size_t i = 0; i < 32; i ++) {
            deck.push_back(pair.first);
        }
    }
    std::vector<std::string> dcards;
    std::vector<std::string> pcards;
    int rand1 = random(0, 415);
    std::string dc1 = deck[rand1];
    dcards.push_back(dc1);
    deck.erase(deck.begin() + rand1);
    int rand2 = random(0, 414);
    std::string pc1 = deck[rand2];
    pcards.push_back(pc1);
    deck.erase(deck.begin() + rand2);
    int rand3 = random(0, 413);
    std::string dc2 = deck[rand3];
    dcards.push_back(dc2);
    deck.erase(deck.begin() + rand3);
    int rand4 = random(0, 412);
    std::string pc2 = deck[rand4];
    pcards.push_back(pc2);
    deck.erase(deck.begin() + rand4);
    std::cout << "Dealer has: " << dc1 << " ? ; sum - " << weights[dc1] << '\n';
    std::cout << "You have: " << pc1 << ' ' << pc2 << " ; sum - " << handSum(weights, pcards) << '\n';
    bool gameEnded = 0;
    if (handSum(weights, pcards) == 21) {
        std::cout << "Blackjack, you win!";
        gameEnded = 1;
    }
    else if (dc1 == "A" and handSum(weights, dcards) == 21) {
        std::cout << "Dealer has Blackjack, you lose!";
        gameEnded = 1;
    }
    else {
        if (weights[pc1] == weights[pc2]) {
            std::cout << "Do you want to split? (type 'yes' or 'no')" << '\n';
            std::string command;
            std::getline(std::cin, command);
            if (command == "yes") {
                std::vector<std::string> pcards1, pcards2;
                pcards1.push_back(pc1);
                pcards2.push_back(pc2);
                std::cout<< "First hand:" << '\n';
                basicLine(weights, deck, pcards1, gameEnded);
                std::cout<< "Second hand:" << '\n';
                bool gameEnded2 = 0;
                basicLine(weights, deck, pcards2, gameEnded2);
                if (gameEnded == 0 and gameEnded2 == 1) {
                    dealerMove(weights, deck, dcards, pcards1);
                }
                else if (gameEnded == 1 and gameEnded2 == 0) {
                    dealerMove(weights, deck, dcards, pcards2);
                }
                else if (gameEnded == 0 and gameEnded2 == 0) {
                    dealerSplit(weights, deck, dcards, pcards1, pcards2);
                    gameEnded = 1;
                }
            }
            else {
                doubleLine(weights, deck, pcards, gameEnded);
            }
        }
        else {
            doubleLine(weights, deck, pcards, gameEnded);
        }
              
    }
    if (gameEnded == 0) {
        dealerMove(weights, deck, dcards, pcards);
    }
}


int random(int min, int max)
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) );
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

int handSum (std::map<std::string, std::string> weights, const std::vector<std::string> cards) {
    int sum = 0;
    int aceCount = 0;
    for (std::string card: cards) {
        sum += std::stoi(weights[card]);
        if (card == "A") aceCount += 1;
    }
    while (sum > 21 and aceCount > 0) {
        sum -= 10;
        aceCount--;
    }
    return sum;
}

void basicLine(std::map<std::string, std::string> weights, std::vector<std::string>& deck, std::vector<std::string>& pcards, bool& gameEnded) {
    while (handSum(weights, pcards) < 21) {
        std::cout << "hit or stay?" << '\n';
        std::string command;
        std::getline(std::cin, command);
        if (command == "hit") {
            int rand = random(0, deck.size() - 1);
            std::string pc = deck[rand];
            pcards.push_back(pc);
            deck.erase(deck.begin() + rand);
            for (size_t i = 0; i < pcards.size(); i++) {
                std::cout << pcards[i] << ' ';
            }
            std::cout << "; sum - " << handSum(weights, pcards)<< '\n';
            if (handSum(weights, pcards) == 21) break;
            else if (handSum(weights, pcards) > 21) {
                std::cout << "you bust haha" << '\n';
                gameEnded = 1;
                break;
            }
        }
        else if (command == "stay") break;
        else {
            std::cout << "ты чушпан" << '\n';
            continue;
        }
    }
}

void doubleLine(std::map<std::string, std::string> weights, std::vector<std::string>& deck, std::vector<std::string>& pcards, bool& gameEnded) {
    std::cout << "Double or hit or stay" << '\n';
    std::string command;
    std::getline(std::cin, command);
    if (command == "double") {
        int rand = random(0, deck.size() - 1);
        std::string pc = deck[rand];
        pcards.push_back(pc);
        deck.erase(deck.begin() + rand);
        for (size_t i = 0; i < pcards.size(); i++) {
            std::cout << pcards[i] << ' ';
        }
        std::cout << "; sum - " << handSum(weights, pcards)<< '\n';
        if (handSum(weights, pcards) > 21) {
            std::cout << "you bust haha";
            gameEnded = 1;
        }  
    }
    else if (command == "hit") {
        int rand = random(0, deck.size() - 1);
        std::string pc = deck[rand];
        pcards.push_back(pc);
        deck.erase(deck.begin() + rand);
        for (size_t i = 0; i < pcards.size(); i++) {
            std::cout << pcards[i] << ' ';
        }
        std::cout << "; sum - " << handSum(weights, pcards)<< '\n';
        if (handSum(weights, pcards) > 21) {
            std::cout << "you bust haha" << '\n';
            gameEnded = 1;
        }
        else if (handSum(weights, pcards) < 21) {
            basicLine(weights, deck, pcards, gameEnded);
        }
    } 
}

void dealerMove(std::map<std::string, std::string> weights, std::vector<std::string>& deck, std::vector<std::string>& dcards, std::vector<std::string>& pcards) {
    std::cout << "Dealer has: " << dcards[0] << ' ' << dcards[1] << " ; sum - " << handSum(weights, dcards) << '\n';
    while (handSum(weights, dcards) < 17) {
        int rand = random(0, deck.size() - 1);
        std::string dc = deck[rand];
        dcards.push_back(dc);
        deck.erase(deck.begin() + rand);
        for (size_t i = 0; i < dcards.size(); i++) {
            std::cout << dcards[i] << ' ';
        }
        std::cout << "; sum - " << handSum(weights, dcards)<< '\n';
    }
    if (handSum(weights, dcards) > 21) {
        std::cout << "Dealer loh, you win";
    }
    else {
        if (handSum(weights, dcards) > handSum(weights, pcards)) {
            std::cout << "Dealer wins: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards);
        }
        else if (handSum(weights, dcards) < handSum(weights, pcards)) {
            std::cout << "You win: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards);
        }
        else {
            std::cout << "Tie. Both of you have " << handSum(weights, dcards);
        } 
    }
}
void dealerSplit(std::map<std::string, std::string> weights, std::vector<std::string>& deck, std::vector<std::string>& dcards, std::vector<std::string>& pcards1, std::vector<std::string>& pcards2) {
    std::cout << "Dealer has: " << dcards[0] << ' ' << dcards[1] << " ; sum - " << handSum(weights, dcards) << '\n';
    while (handSum(weights, dcards) < 17) {
        int rand = random(0, deck.size() - 1);
        std::string dc = deck[rand];
        dcards.push_back(dc);
        deck.erase(deck.begin() + rand);
        for (size_t i = 0; i < dcards.size(); i++) {
            std::cout << dcards[i] << ' ';
        }
        std::cout << "; sum - " << handSum(weights, dcards)<< '\n';
    }
    if (handSum(weights, dcards) > 21) {
        std::cout << "Dealer loh, you win both";
    }
    else {
        if (handSum(weights, dcards) > handSum(weights, pcards1)) {
            std::cout << "Dealer wins against 1st hand: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards1) << '\n';
        }
        else if (handSum(weights, dcards) < handSum(weights, pcards1)) {
            std::cout << "You win 1st hand: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards1) << '\n';
        }
        else {
            std::cout << "Tie for 1st hand. Both of you have " << handSum(weights, dcards) << '\n';
        }
        if (handSum(weights, dcards) > handSum(weights, pcards2)) {
            std::cout << "Dealer wins against 2nd hand: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards2);
        }
        else if (handSum(weights, dcards) < handSum(weights, pcards2)) {
            std::cout << "You win 2nd hand: " << handSum(weights, dcards) << " vs " << handSum(weights, pcards2);
        }
        else {
            std::cout << "Tie for 2nd hand. Both of you have " << handSum(weights, dcards);
        }  
    }
}
