#include "blackj.h"
#include "ui_blackj.h"
#include "mainwindow.h"
#include <random>
#include <vector>
#include <map>
#include <string>

int random(int, int);
int handSum(std::map<std::string, std::string>, const std::vector<std::string> );
void basicLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);
void doubleLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);
void splitLine(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, bool&);

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

std::vector<std::string> dcards;
std::vector<std::string> pcards;

blackj::blackj(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::blackj)
{
    ui->setupUi(this);
    ui->blackjDealerContent->hide();
    ui->blackjPlayerContent->hide();
    ui->blackjResultBar->hide();
    ui->blackjHitButton->hide();
    ui->blackjStayButton->hide();
    ui->blackjDoubleButton->hide();
    ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
            this, &blackj::updateDisplayedBalance);
}

blackj::~blackj()
{
    delete ui;
}

void blackj::on_blackjDepositButton_clicked()
{
    BalanceManager::balanceInstance().depositBalance(1000);
    ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
}

void blackj::on_blackjBackButton_clicked()
{
    hide();
    MainWindow *hub = new MainWindow(this);
    hub->show();
    emit balanceChanged(BalanceManager::balanceInstance().getBalance());
}

void blackj::updateDisplayedBalance(int newBalance)
{
    ui->blackjBalanceNum->setText(QString::number(newBalance));
}




void blackj::on_blackjPlayButton_clicked()
{
    ui->blackjPlayButton->hide();
    ui->blackjDealerContent->show();
    ui->blackjPlayerContent->show();
    ui->blackjPrizeNum->setText(" ");
    ui->blackjResultBar->setText(QString::fromStdString(""));
    ui->blackjResultBar->show();

    QString bet = ui->blackjBetNum->text();
    blackjBet = bet.toInt();
    ui->blackjBetNum->setDisabled(true);
    BalanceManager::balanceInstance().withdrawBalance(blackjBet);

    deck = {};
    pcards = {};
    dcards = {};
    for (std::pair<std::string, std::string> pair: weights) {
        for (size_t i = 0; i < 32; i ++) {
            deck.push_back(pair.first);
        }
    }
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
    QString dealer = QString::fromStdString("Dealer has: " + dc1 + " ? ; sum - " + weights[dc1]);
    QString player = QString::fromStdString("You have: " + pc1 + " "+ pc2 + " ; sum - " + std::to_string(handSum(weights, pcards)));
    ui->blackjDealerContent->setText(dealer);
    ui->blackjPlayerContent->setText(player);
    if (handSum(weights, pcards) == 21) {
        ui->blackjResultBar->setText(QString::fromStdString("Blackjack, you win!"));
        ui->blackjPlayButton->show();
        BalanceManager::balanceInstance().depositBalance(blackjBet*3);
        ui->blackjPrizeNum->setText(QString::number(blackjBet*3));
        ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    }
    else if (dc1 == "A" and handSum(weights, dcards) == 21) {
        ui->blackjResultBar->setText(QString::fromStdString("Dealer has Blackjack, you lose!"));
        ui->blackjPlayButton->show();
        ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->blackjPrizeNum->setText("0");

    }

    else {
        ui->blackjHitButton->show();
        ui->blackjStayButton->show();
        ui->blackjDoubleButton->show();


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









void blackj::on_blackjHitButton_clicked()
{
    int rand = random(0, deck.size() - 1);
    std::string pc = deck[rand];
    pcards.push_back(pc);
    deck.erase(deck.begin() + rand);
    std::string penis = "";
    for (size_t i = 0; i < pcards.size(); i++) {
        penis = penis + pcards[i] + " ";
    }
    penis = penis + "; sum - " + std::to_string(handSum(weights, pcards));
    ui->blackjPlayerContent->setText(QString::fromStdString(penis));
    if (handSum(weights, pcards) > 21) {
        ui->blackjResultBar->setText(QString::fromStdString("you bust haha"));
        ui->blackjPrizeNum->setText("0");
        ui->blackjHitButton->hide();
        ui->blackjStayButton->hide();
        ui->blackjDoubleButton->hide();
        ui->blackjPlayButton->show();

    }
    else if (handSum(weights, pcards) < 21) {
        ui->blackjDoubleButton->hide();
    }
    else {
        while (handSum(weights, dcards) < 17) {
            int rand = random(0, deck.size() - 1);
            std::string dc = deck[rand];
            dcards.push_back(dc);
            deck.erase(deck.begin() + rand);
        }
        std::string penis = "";
        for (size_t i = 0; i < dcards.size(); i++) {
            penis = penis + dcards[i] + " ";
        }
        penis = penis + "; sum - " + std::to_string(handSum(weights, dcards));
        ui->blackjDealerContent->setText(QString::fromStdString(penis));
        if (handSum(weights, dcards) > 21) {
            ui->blackjResultBar->setText(QString::fromStdString("Dealer loh, you win"));
            BalanceManager::balanceInstance().depositBalance(2*blackjBet);
            ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
            ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
            ui->blackjHitButton->hide();
            ui->blackjStayButton->hide();
            ui->blackjDoubleButton->hide();
            ui->blackjPlayButton->show();

        }
        else {
            if (handSum(weights, dcards) > handSum(weights, pcards)) {
                std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
                ui->blackjPrizeNum->setText("0");
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();
            }
            else if (handSum(weights, dcards) < handSum(weights, pcards)) {
                std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                BalanceManager::balanceInstance().depositBalance(2*blackjBet);
                ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
                ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();
            }
            else {
                std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();
                ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
            }
        }
    }
}

void blackj::on_blackjStayButton_clicked()
{
    while (handSum(weights, dcards) < 17) {
        int rand = random(0, deck.size() - 1);
        std::string dc = deck[rand];
        dcards.push_back(dc);
        deck.erase(deck.begin() + rand);
    }
    std::string penis = "";
    for (size_t i = 0; i < dcards.size(); i++) {
        penis = penis + dcards[i] + " ";
    }
    penis = penis + "; sum - " + std::to_string(handSum(weights, dcards));
    ui->blackjDealerContent->setText(QString::fromStdString(penis));
    if (handSum(weights, dcards) > 21) {
        ui->blackjResultBar->setText(QString::fromStdString("Dealer loh, you win"));
        BalanceManager::balanceInstance().depositBalance(2*blackjBet);
        ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
        ui->blackjHitButton->hide();
        ui->blackjStayButton->hide();
        ui->blackjDoubleButton->hide();
        ui->blackjPlayButton->show();

    }
    else {
        if (handSum(weights, dcards) > handSum(weights, pcards)) {
            std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
            ui->blackjResultBar->setText(QString::fromStdString(rez));
            ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
            ui->blackjPrizeNum->setText("0");
            ui->blackjHitButton->hide();
            ui->blackjStayButton->hide();
            ui->blackjDoubleButton->hide();
            ui->blackjPlayButton->show();
        }
        else if (handSum(weights, dcards) < handSum(weights, pcards)) {
            std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
            ui->blackjResultBar->setText(QString::fromStdString(rez));
            BalanceManager::balanceInstance().depositBalance(blackjBet*2);
            ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
            ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
            ui->blackjHitButton->hide();
            ui->blackjStayButton->hide();
            ui->blackjDoubleButton->hide();
            ui->blackjPlayButton->show();

        }
        else {
            std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
            ui->blackjResultBar->setText(QString::fromStdString(rez));
            ui->blackjHitButton->hide();
            ui->blackjStayButton->hide();
            ui->blackjDoubleButton->hide();
            ui->blackjPlayButton->show();
            ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        }
    }
}




void blackj::on_blackjDoubleButton_clicked()
{
    BalanceManager::balanceInstance().withdrawBalance(blackjBet);
    blackjBet = 2*blackjBet;
    ui->blackjBetNum->setText(QString::number(blackjBet));
    int rand = random(0, deck.size() - 1);
    std::string pc = deck[rand];
    pcards.push_back(pc);
    deck.erase(deck.begin() + rand);
    std::string penis = "";
    for (size_t i = 0; i < pcards.size(); i++) {
        penis = penis + pcards[i] + " ";
    }
    penis = penis + "; sum - " + std::to_string(handSum(weights, pcards));
    ui->blackjPlayerContent->setText(QString::fromStdString(penis));
    if (handSum(weights, pcards) > 21) {
        ui->blackjResultBar->setText(QString::fromStdString("you bust haha"));
        ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->blackjPrizeNum->setText("0");
        ui->blackjHitButton->hide();
        ui->blackjStayButton->hide();
        ui->blackjDoubleButton->hide();
        ui->blackjPlayButton->show();
    }
    else {
        while (handSum(weights, dcards) < 17) {
            int rand = random(0, deck.size() - 1);
            std::string dc = deck[rand];
            dcards.push_back(dc);
            deck.erase(deck.begin() + rand);
        }
        std::string penis = "";
        for (size_t i = 0; i < dcards.size(); i++) {
            penis = penis + dcards[i] + " ";
        }
        penis = penis + "; sum - " + std::to_string(handSum(weights, dcards));
        ui->blackjDealerContent->setText(QString::fromStdString(penis));
        if (handSum(weights, dcards) > 21) {
            ui->blackjResultBar->setText(QString::fromStdString("Dealer loh, you win"));
            BalanceManager::balanceInstance().depositBalance(2*blackjBet);
            ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
            ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
            ui->blackjHitButton->hide();
            ui->blackjStayButton->hide();
            ui->blackjDoubleButton->hide();
            ui->blackjPlayButton->show();

        }
        else {
            if (handSum(weights, dcards) > handSum(weights, pcards)) {
                std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
                ui->blackjPrizeNum->setText("0");
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();

            }
            else if (handSum(weights, dcards) < handSum(weights, pcards)) {
                std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                BalanceManager::balanceInstance().depositBalance(2*blackjBet);
                ui->blackjPrizeNum->setText(QString::number(blackjBet*2));
                ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();

            }
            else {
                std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
                ui->blackjResultBar->setText(QString::fromStdString(rez));
                ui->blackjHitButton->hide();
                ui->blackjStayButton->hide();
                ui->blackjDoubleButton->hide();
                ui->blackjPlayButton->show();
            }
        }
    }
}



