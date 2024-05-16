// cool dsba poker
#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <iomanip>

int random(int, int);
int minn(int, int);
long long power(int, int);
typedef std::pair<int, int> card;
typedef std::pair<card, card> hand;
typedef std::vector<card> table;
void cardPrint(card&);
void printAll(hand&, hand&, table&);
double findComb(hand&, table&);
void winnerDecider(double&, double&);
void highestCard(std::vector<int>&, double&);
void isPair(std::vector<int>&, double&);
void isTwoPair(std::vector<int>&, double&);
void isSet(std::vector<int>&, double&);
void isStreet(std::vector<int>&, double&);
void isFlash(std::vector<int>&, double&, table&, hand&);
void isBoat(std::vector<int>&, double&);
void isQuads(std::vector<int>&, double&);
void isStreetFlash(std::vector<int>&, double&, table&, hand&);

int main() {
    std::cout << std::setprecision(10) << std::fixed;
    std::vector<card> deck;
    for (size_t i = 0; i < 4; i++) { // заполнение колоды картами, расшифровку масти и номинала можно чекнуть в функции cardPrint
        for (size_t j = 0; j < 13; j++) {
            deck.push_back(std::make_pair(j+1, i));
        }
    }
    // здесь реализована раздача карт игрокам и генерация флопа, терна и ривера
    hand p1;
    int rand = random(0, deck.size());
    p1.first = deck[rand];
    deck.erase(deck.begin() + rand);
    rand = random(0, deck.size());
    p1.second = deck[rand];
    deck.erase(deck.begin() + rand);
    hand p2;
    rand = random(0, deck.size());
    p2.first = deck[rand];
    deck.erase(deck.begin() + rand);
    rand = random(0, deck.size());
    p2.second = deck[rand];
    deck.erase(deck.begin() + rand);
    table tableRand;
    for (size_t i = 0; i < 5; i++) {
        rand = random(0, deck.size());
        tableRand.push_back(deck[rand]);
        deck.erase(deck.begin() + rand);
    }
    table tableTest = { // можно выбрать свой расклад на столе, но карты которые вы выберите не пропадут из колоды!
        std::make_pair(8, 0), 
        std::make_pair(12, 0), 
        std::make_pair(11, 0), 
        std::make_pair(10, 0), 
        std::make_pair(9, 0)
    };
    printAll(p1, p2, tableRand); // здесь можно заменить на tableTest
    return 0;
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

int minn(int a, int b) {
    if (a < b) return a;
    else if (b < a) return b;
    else return a;
}

long long power(int a, int b) {
    long long rez = 1;
    while (b > 0) {
        rez *= a;
        b--;
    }
    return rez;
}

void cardPrint(card& card) {
    std::map<int, std::string> suit{ // расшифровка масти
        {0, "c"},
        {1, "d"},
        {2, "h"},
        {3, "s"},
    };
    std::map<int, std::string> nominal{ // расшифровка номинала
        {1, "2"},
        {2, "3"},
        {3, "4"},
        {4, "5"},
        {5, "6"},
        {6, "7"},
        {7, "8"},
        {8, "9"},
        {9, "T"},
        {10, "J"},
        {11, "Q"},
        {12, "K"},
        {13, "A"},
    };
    std::cout << nominal[card.first] << suit[card.second] << ' ';
}

void printAll(hand& h1, hand& h2, table& table) { // функция выводит руки игроков и стол, чтобы можно было вручную проверить правоту программы
    std::cout << "A777MP has: ";
    cardPrint(h1.first);
    cardPrint(h1.second);
    double comb1 = findComb(h1, table);
    std::cout << comb1 << '\n';
    for (size_t i = 0; i < 5; i++) {
        cardPrint(table[i]);
    }
    std::cout << '\n';
    std::cout << "B777OP has: ";
    cardPrint(h2.first);
    cardPrint(h2.second);
    double comb2 = findComb(h2, table);
    std::cout << comb2 << '\n';
    winnerDecider(comb1, comb2);
}

double findComb(hand& hand, table& table) { // функция высчитвает вес комбинации, чем больше - тем сильнее
    double rez = 0;
    std::vector<int> suitCount(4, 0);
    std::vector<int> nominalCount(14, 0);
    for (size_t i = 0; i < 5; i++) {
        suitCount[table[i].second] += 1;
    }
    suitCount[hand.first.second] += 1;
    suitCount[hand.second.second] += 1;
    for (size_t i = 0; i < 5; i++) {
        nominalCount[table[i].first] += 1;
    }
    nominalCount[hand.first.first] += 1;
    nominalCount[hand.second.first] += 1;
    nominalCount[0] = nominalCount[13];

    highestCard(nominalCount, rez);
    isPair(nominalCount, rez);
    isTwoPair(nominalCount, rez);
    isSet(nominalCount, rez);
    isStreet(nominalCount, rez);
    isFlash(suitCount, rez, table, hand);
    isBoat(nominalCount, rez);
    isQuads(nominalCount, rez);
    isStreetFlash(suitCount, rez, table, hand);
    return rez;
}

// функция определяющая победителя
void winnerDecider(double& comb1, double& comb2) { 
    if (comb1 > comb2) std::cout << "A777MP wins.";
    else if (comb2 > comb1) std::cout << "B777OP wins.";
    else std::cout << "Friendship wins.";
}

// функции проверяющие наличие каждой комбинации
void highestCard(std::vector<int>& nominalCount, double& rez) {
    int pow = 1;
    int cnt = 0;
    for (size_t i = 0; i < 13; i++) {
        if (nominalCount[13-i] == 1 and cnt != 5) {
            rez += (double)(13-i)/power(100, pow);
            pow ++;
            cnt ++;
        }
    }
}

void isPair(std::vector<int>& nominalCount, double& rez) {
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
                rez += (double)(13-i)/power(100, pow);
                pow ++;
                cnt ++;
            }
        }
    }
}

void isTwoPair(std::vector<int>& nominalCount, double& rez) {
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
                rez += (double)(13-i)/power(100, pow);
                pow ++;
                cnt ++;
            }
        }
        for (size_t i = 0; i < 13; i++) {
            if (nominalCount[13-i] == 1) {
                rez += (double)(13-i)/power(100, pow);
                break;
            }
        }
    }
}

void isSet(std::vector<int>& nominalCount, double& rez) {
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
                rez += (double)(13-i)/power(100, pow);
                pow ++;
                cnt ++;
            }
        } 
    }
}

void isStreet(std::vector<int>& nominalCount, double& rez) {
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

void isFlash(std::vector<int>& suitCount, double& rez, table& table, hand& hand) {
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

void isBoat(std::vector<int>& nominalCount, double& rez) {
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

void isQuads(std::vector<int>& nominalCount, double& rez) {
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

void isStreetFlash(std::vector<int>& suitCount, double& rez, table& table, hand& hand) {
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
