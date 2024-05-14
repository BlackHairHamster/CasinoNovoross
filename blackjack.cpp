#include <iostream>
#include <random>
#include <vector>
#include <map>

int random(int, int);
int handSum(std::map<std::string, std::string>, const std::vector<std::string> );



int main(int, char**){
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
    else {
        if (handSum(weights, dcards) == 21) {
            std::cout << "Dealer has Blackjack, you lose!";
            gameEnded = 1;
        }
        else {
            while (handSum(weights, pcards) < 21) {
                std::cout << "hit ot stay?" << '\n';
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
                        std::cout << "you bust haha";
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
    }
    if (gameEnded == 0) {
        std::cout << "Dealer has: " << dc1 << ' ' << dc2 << " ; sum - " << handSum(weights, dcards) << '\n';
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
            else std::cout << "Tie. Both of you have " << handSum(weights, dcards);
        }
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
    for (std::string card: cards) {
        sum += std::stoi(weights[card]);
    }
    return sum;
}
