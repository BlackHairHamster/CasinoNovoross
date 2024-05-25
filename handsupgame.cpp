#include "handsupgame.h"
#include <iostream>
#include <random>
#include <iomanip>
#include <ctime>

HandsUpGame::HandsUpGame() {
    std::srand(std::time(nullptr));
}

int HandsUpGame::random(int min, int max) {
    return min + rand() % (( max + 1 ) - min);
}



void HandsUpGame::startNewGame() {
    std::vector<card> deck;
    for (size_t i = 0; i < 4; i++) {
        for (size_t j = 0; j < 13; j++) {
            deck.push_back(std::make_pair(j + 1, i));
        }
    }

    int rand = random(0, deck.size()-1);
    playerHand.first = deck[rand];
    deck.erase(deck.begin() + rand);
    rand = random(0, deck.size()-1);
    playerHand.second = deck[rand];
    deck.erase(deck.begin() + rand);
    rand = random(0, deck.size()-1);
    dealerHand.first = deck[rand];
    deck.erase(deck.begin() + rand);
    rand = random(0, deck.size()-1);
    dealerHand.second = deck[rand];
    deck.erase(deck.begin() + rand);
    tableCards.clear();

    for (size_t i = 0; i < 5; i++) {
        rand = random(0, deck.size()-1);
        tableCards.push_back(deck[rand]);
        deck.erase(deck.begin() + rand);
    }
}

void HandsUpGame::printAll() {
    double comb1 = findComb(playerHand, tableCards);
    double comb2 = findComb(dealerHand, tableCards);
    winnerDecider(comb1, comb2);
}

double HandsUpGame::findComb(hand& playerHand, table& tableCards) {
    double rez = 0;
    std::vector<int> suitCount(4, 0);
    std::vector<int> nominalCount(14, 0);
    for (size_t i = 0; i < tableCards.size(); i++) {
        suitCount[tableCards[i].second]++;
        nominalCount[tableCards[i].first]++;
    }
    suitCount[playerHand.first.second]++;
    suitCount[playerHand.second.second]++;
    nominalCount[playerHand.first.first]++;
    nominalCount[playerHand.second.first]++;

    nominalCount[0] = nominalCount[13];

    highestCard(nominalCount, rez);
    isPair(nominalCount, rez);
    isTwoPair(nominalCount, rez);
    isSet(nominalCount, rez);
    isStreet(nominalCount, rez);
    isFlash(suitCount, rez, tableCards, playerHand);
    isBoat(nominalCount, rez);
    isQuads(nominalCount, rez);
    isStreetFlash(suitCount, rez, tableCards, playerHand);
    return rez;
}

std::string HandsUpGame::winningCombPrint(double& rez) {

    std::map<int, std::string> nominalSingular{ // расшифровка номинала
                                               {1, "Deuce"},
                                               {2, "Three"},
                                               {3, "Four"},
                                               {4, "Five"},
                                               {5, "Six"},
                                               {6, "Seven"},
                                               {7, "Eight"},
                                               {8, "Nine"},
                                               {9, "Ten"},
                                               {10, "Jack"},
                                               {11, "Queen"},
                                               {12, "King"},
                                               {13, "Ace"},
                                               };

    std::map<int, std::string> nominalPlural{ // расшифровка номинала
                                             {1, "Deuces"},
                                             {2, "Threes"},
                                             {3, "Fours"},
                                             {4, "Fives"},
                                             {5, "Sixes"},
                                             {6, "Sevens"},
                                             {7, "Eights"},
                                             {8, "Nines"},
                                             {9, "Tens"},
                                             {10, "Jacks"},
                                             {11, "Queens"},
                                             {12, "Kings"},
                                             {13, "Aces"},
                                             };

    std::string stringRez = std::to_string(rez);
    if (rez < 1) {
        return nominalSingular[std::stoi(stringRez.substr(2, 2))] + " High";
    }
    if (rez < 2) {
        return "Pair of " + nominalPlural[std::stoi(stringRez.substr(2, 2))];
    }
    if (rez < 3) {
        return "Two pair: " + nominalPlural[std::stoi(stringRez.substr(2, 2))] + " and " + nominalPlural[std::stoi(stringRez.substr(4, 2))];
    }
    if (rez < 4) {
        return "Three of a kind: " + nominalPlural[std::stoi(stringRez.substr(2, 2))];
    }
    if (rez < 5) {
        if (std::stoi(stringRez.substr(2, 2)) == 4) return "Straight: from Ace to Five";
        else return "Straight: from " + nominalSingular[std::stoi(stringRez.substr(2, 2)) - 4] + " to " + \
                   nominalSingular[std::stoi(stringRez.substr(2, 2))];
    }
    if (rez < 6) {
        return "Flash: " + nominalSingular[std::stoi(stringRez.substr(2, 2))] + " High";
    }
    if (rez < 7) {
        return "Full house: " + nominalPlural[std::stoi(stringRez.substr(2, 2))] + " full of " + \
               nominalPlural[std::stoi(stringRez.substr(4, 2))];
    }
    if (rez < 8) {
        return "Quad " + nominalPlural[std::stoi(stringRez.substr(2, 2))];
    }
    if (std::stoi(stringRez.substr(2, 2)) == 4) return "Straight Flash: from Ace to Five";
    else return "Straight Flash: from " + nominalSingular[std::stoi(stringRez.substr(2, 2)) - 4] + " to " + \
               nominalSingular[std::stoi(stringRez.substr(2, 2))];

}

void HandsUpGame::winnerDecider(double& comb1, double& comb2) {
    if (comb1 > comb2) {
        winner = "player";
        winningComb = winningCombPrint(comb1);
    }
    else if (comb2 > comb1) {
        winner = "dealer";
        winningComb = winningCombPrint(comb2);
    }
    else {
        winner = "tie";
        winningComb = winningCombPrint(comb1);
    }
}


void HandsUpGame::highestCard(std::vector<int>& nominalCount, double& rez) {
    int pow = 1;
    int cnt = 0;
    for (size_t i = 0; i < 13; i++) {
        if (nominalCount[13-i] == 1 and cnt != 5) {
            rez += (double)(13-i)/std::pow(100, pow);
            pow ++;
            cnt ++;
        }
    }
}

void HandsUpGame::isPair(std::vector<int>& nominalCount, double& rez) {
    bool flag = 0;
    for (size_t i = 0; i < 13; i++) {
        if (nominalCount[13-i] == 2) {
            rez = 1 + (double)(13-i)/100;
            flag = 1;
            break;
        }
    }
    if (flag) {
        int pow = 2;
        int cnt = 0;
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 1 and cnt != 3) {
                rez += (double)(13-i)/std::pow(100, pow);
                pow ++;
                cnt ++;
            }
        }
    }
}

void HandsUpGame::isTwoPair(std::vector<int>& nominalCount, double& rez) {
    int pairCnt = 0;
    for (size_t i = 1; i < 14; i++) {
        if (nominalCount[i] == 2) {
            pairCnt += 1;
        }
    }
    if (pairCnt >=2) {
        rez = 2;
        int pow = 1;
        int cnt = 0;
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 2 and cnt != 2) {
                rez += (double)(13-i)/std::pow(100, pow);
                pow ++;
                cnt ++;
            }
        }
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 1) {
                rez += (double)(13-i)/std::pow(100, pow);
                break;
            }
        }
    }
}

void HandsUpGame::isSet(std::vector<int>& nominalCount, double& rez) {
    bool flag = 0;
    for (size_t i = 0; i < 13; i++) {
        if (nominalCount[13-i] == 3) {
            rez = 3 + (double)(13-i)/100;
            flag = 1;
            break;
        }
    }
    if (flag) {
        int pow = 2;
        int cnt = 0;
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 1 and cnt != 2) {
                rez += (double)(13-i)/std::pow(100, pow);
                pow ++;
                cnt ++;
            }
        }
    }
}
int minn(int a, int b) {
    if (a < b) return a;
    else if (b < a) return b;
    else return a;
}
void HandsUpGame::isStreet(std::vector<int>& nominalCount, double& rez) {
    int curSum = 0;
    for (size_t i = 0; i < 5; i++) {
        curSum += minn(1, nominalCount[i]);
    }
    if (curSum == 5) rez = 4.04;
    for (size_t i = 5; i < 14; i++) {
        curSum -= minn(1, nominalCount[i-5]);
        curSum += minn(1, nominalCount[i]);
        if (curSum == 5) {
            rez = 4 + (double)i/100;
        }
    }
}

void HandsUpGame::isFlash(std::vector<int>& suitCount, double& rez, table& table, hand& hand) {
    for (size_t i = 0; i < 4; i++) {
        if (suitCount[i] >= 5) {
            rez = 5;
            std::vector<int> suitedNominalCnt(14, 0);
            for (size_t j = 0; j < 5; j++) {
                if (table[j].second == i) {
                    suitedNominalCnt[table[j].first] += 1;
                }
            }
            if (hand.first.second == i) suitedNominalCnt[hand.first.first] += 1;
            if (hand.second.second == i) suitedNominalCnt[hand.second.first] += 1;
            highestCard(suitedNominalCnt, rez);
            break;
        }
    }
}

void HandsUpGame::isBoat(std::vector<int>& nominalCount, double& rez) {
    bool flag2 = 0;
    bool flag3 = 0;
    for (size_t i = 1; i < 14; i++) {
        if (nominalCount[i] == 2) flag2 = 1;
        if (nominalCount[i] == 3) flag3 = 1;
    }
    if (flag2 and flag3) {
        rez = 6;
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 3) {
                rez = 6 + (double)(13-i)/100;
                break;
            }
        }
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 2) {
                rez += (double)(13-i)/10000;
                break;
            }
        }
    }
}

void HandsUpGame::isQuads(std::vector<int>& nominalCount, double& rez) {
    bool flag = 0;
    for (size_t i = 0; i < 13; i++) {
        if (nominalCount[13-i] == 4) {
            rez = 7 + (double)(13-i)/100;
            break;
        }
    }
    if (flag) {
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] > 0 and nominalCount[13-i] != 4) {
                rez += (double)(13-i)/10000;
                break;
            }
        }
    }
}

void HandsUpGame::isStreetFlash(std::vector<int>& suitCount, double& rez, table& table, hand& hand) {
    for (size_t i = 0; i < 4; i++) {
        if (suitCount[i] >= 5) {
            std::vector<int> suitedNominalCnt(14, 0);
            for (size_t j = 0; j < 5; j++) {
                if (table[j].second == i) {
                    suitedNominalCnt[table[j].first] += 1;
                }
            }
            if (hand.first.second == i) suitedNominalCnt[hand.first.first] += 1;
            if (hand.second.second == i) suitedNominalCnt[hand.second.first] += 1;
            double streetCheck = 0;
            isStreet(suitedNominalCnt, streetCheck);
            if (streetCheck >= 4) rez = streetCheck + 4;
            break;
        }
    }
}
