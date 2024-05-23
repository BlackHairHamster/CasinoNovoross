#ifndef HANDSUPGAME_H
#define HANDSUPGAME_H

#include <vector>
#include <map>
#include <utility>

typedef std::pair<int, int> card;
typedef std::pair<card, card> hand;
typedef std::vector<card> table;

class HandsUpGame {
public:
    HandsUpGame();

    void startNewGame();
    void printAll();
    double findComb(hand& playerHand, table& tableCards);
    void winnerDecider(double& comb1, double& comb2);

    const hand& getPlayerHand() const { return playerHand; }
    const hand& getDealerHand() const { return dealerHand; }
    const table& getTableCards() const { return tableCards; }
    const std::string& getWinner() const { return winner; }

private:
    hand playerHand;
    hand dealerHand;
    table tableCards;
    std::string winner;

    int random(int min, int max);
    void highestCard(std::vector<int>& nominalCount, double& rez);
    void isPair(std::vector<int>& nominalCount, double& rez);
    void isTwoPair(std::vector<int>& nominalCount, double& rez);
    void isSet(std::vector<int>& nominalCount, double& rez);
    void isStreet(std::vector<int>& nominalCount, double& rez);
    void isFlash(std::vector<int>& suitCount, double& rez, table& tableCards, hand& playerHand);
    void isBoat(std::vector<int>& nominalCount, double& rez);
    void isQuads(std::vector<int>& nominalCount, double& rez);
    void isStreetFlash(std::vector<int>& suitCount, double& rez, table& tableCards, hand& playerHand);
};

#endif // HANDSUPGAME_H
