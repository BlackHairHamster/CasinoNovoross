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
void dealerMove(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);
void dealerSplit(std::map<std::string, std::string>, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&);

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

blackj::blackj(int balance, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::blackj)
{
    ui->setupUi(this);
    curBalance = balance;
    ui->label_2->setText(QString::number(curBalance));
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
    ui->pushButton_5->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
}

blackj::~blackj()
{
    delete ui;
}

void blackj::on_pushButton_clicked()
{
    hide();
    MainWindow *hub = new MainWindow(curBalance, this);
    hub->show();
}


void blackj::on_pushButton_2_clicked()
{
    curBalance += 1000;
    ui->label_2->setText(QString::number(curBalance));
}


void blackj::on_pushButton_3_clicked()
{
    curBalance -= 1000;
    ui->label_2->setText(QString::number(curBalance));
}


void blackj::on_pushButton_4_clicked()
{
    ui->pushButton_4->hide();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->setText(QString::fromStdString(""));
    ui->label_5->show();

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
    ui->label_3->setText(dealer);
    ui->label_4->setText(player);
    if (handSum(weights, pcards) == 21) {
        ui->label_5->setText(QString::fromStdString("Blackjack, you win!"));
        ui->pushButton_4->show();
        curBalance += 1000;
        ui->label_2->setText(QString::number(curBalance));
    }
    else if (dc1 == "A" and handSum(weights, dcards) == 21) {
        ui->label_5->setText(QString::fromStdString("Dealer has Blackjack, you lose!"));
        ui->pushButton_4->show();
        curBalance -= 1000;
        ui->label_2->setText(QString::number(curBalance));

    }

    else {
        // if (weights[pc1] == weights[pc2]) {
        //     ui->label_4->setText(QString::fromStdString("Do you want to split?"));
        //     std::string command;
        //     std::getline(std::cin, command);
        //     if (command == "yes") {
        //         std::vector<std::string> pcards1, pcards2;
        //         pcards1.push_back(pc1);
        //         pcards2.push_back(pc2);
        //         std::cout<< "First hand:" << '\n';
        //         basicLine(weights, deck, pcards1, gameEnded);
        //         std::cout<< "Second hand:" << '\n';
        //         bool gameEnded2 = 0;
        //         basicLine(weights, deck, pcards2, gameEnded2);
        //         if (gameEnded == 0 and gameEnded2 == 1) {
        //             dealerMove(weights, deck, dcards, pcards1);
        //         }
        //         else if (gameEnded == 1 and gameEnded2 == 0) {
        //             dealerMove(weights, deck, dcards, pcards2);
        //         }
        //         else if (gameEnded == 0 and gameEnded2 == 0) {
        //             dealerSplit(weights, deck, dcards, pcards1, pcards2);
        //             gameEnded = 1;
        //         }
        //     }
        //     else {
        //         doubleLine(weights, deck, pcards, gameEnded);
        //     }
        // }
        ui->pushButton_5->show();
        ui->pushButton_6->show();
        ui->pushButton_7->show();


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









void blackj::on_pushButton_5_clicked()
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
    ui->label_4->setText(QString::fromStdString(penis));
    if (handSum(weights, pcards) > 21) {
        ui->label_5->setText(QString::fromStdString("you bust haha"));
        curBalance -= 1000;
        ui->pushButton_5->hide();
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->pushButton_4->show();
        ui->label_2->setText(QString::number(curBalance));

    }
    else if (handSum(weights, pcards) < 21) {
        ui->pushButton_7->hide();
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
        ui->label_3->setText(QString::fromStdString(penis));
        if (handSum(weights, dcards) > 21) {
            ui->label_5->setText(QString::fromStdString("Dealer loh, you win"));
            curBalance += 1000;
            ui->pushButton_5->hide();
            ui->pushButton_6->hide();
            ui->pushButton_7->hide();
            ui->pushButton_4->show();
            ui->label_2->setText(QString::number(curBalance));

        }
        else {
            if (handSum(weights, dcards) > handSum(weights, pcards)) {
                std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->label_5->setText(QString::fromStdString(rez));
                curBalance -= 1000;
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
                ui->label_2->setText(QString::number(curBalance));
            }
            else if (handSum(weights, dcards) < handSum(weights, pcards)) {
                std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->label_5->setText(QString::fromStdString(rez));
                curBalance += 1000;
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
                ui->label_2->setText(QString::number(curBalance));
            }
            else {
                std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
                ui->label_5->setText(QString::fromStdString(rez));
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
                ui->label_2->setText(QString::number(curBalance));
            }
        }
    }
}


void blackj::on_pushButton_6_clicked()
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
    ui->label_3->setText(QString::fromStdString(penis));
    if (handSum(weights, dcards) > 21) {
        ui->label_5->setText(QString::fromStdString("Dealer loh, you win"));
        curBalance += 1000;
        ui->pushButton_5->hide();
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->pushButton_4->show();
        ui->label_2->setText(QString::number(curBalance));

    }
    else {
        if (handSum(weights, dcards) > handSum(weights, pcards)) {
            std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
            ui->label_5->setText(QString::fromStdString(rez));
            curBalance -= 1000;
            ui->pushButton_5->hide();
            ui->pushButton_6->hide();
            ui->pushButton_7->hide();
            ui->pushButton_4->show();
            ui->label_2->setText(QString::number(curBalance));
        }
        else if (handSum(weights, dcards) < handSum(weights, pcards)) {
            std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
            ui->label_5->setText(QString::fromStdString(rez));
            curBalance += 1000;
            ui->pushButton_5->hide();
            ui->pushButton_6->hide();
            ui->pushButton_7->hide();
            ui->pushButton_4->show();
            ui->label_2->setText(QString::number(curBalance));
        }
        else {
            std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
            ui->label_5->setText(QString::fromStdString(rez));
            ui->pushButton_5->hide();
            ui->pushButton_6->hide();
            ui->pushButton_7->hide();
            ui->pushButton_4->show();
            ui->label_2->setText(QString::number(curBalance));
        }
    }
}


void blackj::on_pushButton_7_clicked()
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
    ui->label_4->setText(QString::fromStdString(penis));
    if (handSum(weights, pcards) > 21) {
        ui->label_5->setText(QString::fromStdString("you bust haha"));
        curBalance -= 2000;
        ui->pushButton_5->hide();
        ui->pushButton_6->hide();
        ui->pushButton_7->hide();
        ui->pushButton_4->show();
        ui->label_2->setText(QString::number(curBalance));
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
        ui->label_3->setText(QString::fromStdString(penis));
        if (handSum(weights, dcards) > 21) {
            ui->label_5->setText(QString::fromStdString("Dealer loh, you win"));
            curBalance += 2000;
            ui->pushButton_5->hide();
            ui->pushButton_6->hide();
            ui->pushButton_7->hide();
            ui->pushButton_4->show();
            ui->label_2->setText(QString::number(curBalance));

        }
        else {
            if (handSum(weights, dcards) > handSum(weights, pcards)) {
                std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->label_5->setText(QString::fromStdString(rez));
                curBalance -= 2000;
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
                ui->label_2->setText(QString::number(curBalance));
            }
            else if (handSum(weights, dcards) < handSum(weights, pcards)) {
                std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
                ui->label_5->setText(QString::fromStdString(rez));
                curBalance += 2000;
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
                ui->label_2->setText(QString::number(curBalance));
            }
            else {
                std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
                ui->label_5->setText(QString::fromStdString(rez));
                ui->pushButton_5->hide();
                ui->pushButton_6->hide();
                ui->pushButton_7->hide();
                ui->pushButton_4->show();
            }
        }
    }
}


// void dealerMove(std::map<std::string, std::string> weights, std::vector<std::string>& deck, std::vector<std::string>& dcards, std::vector<std::string>& pcards) {
//     while (handSum(weights, dcards) < 17) {
//         int rand = random(0, deck.size() - 1);
//         std::string dc = deck[rand];
//         dcards.push_back(dc);
//         deck.erase(deck.begin() + rand);
//     }
//     std::string penis = "";
//     for (size_t i = 0; i < pcards.size(); i++) {
//         penis = penis + pcard[i] + " ";
//     }
//     penis = penis + "; sum - " + std::to_string(handSum(weights, pcards));
//     ui->label_3->setText(penis);
//     if (handSum(weights, dcards) > 21) {
//         ui->label_5->setText(QString::fromStdString("Dealer loh, you win"));
//         curBalance += 1000;
//         ui->pushButton_5->hide();
//         ui->pushButton_6->hide();
//         ui->pushButton_7->hide();

//     }
//     else {
//         if (handSum(weights, dcards) > handSum(weights, pcards)) {
//             std::string rez = "Dealer wins: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
//             ui->label_5->setText(QString::fromStdString(rez));
//             curBalance -= 1000;
//             ui->pushButton_5->hide();
//             ui->pushButton_6->hide();
//             ui->pushButton_7->hide();
//         }
//         else if (handSum(weights, dcards) < handSum(weights, pcards)) {
//             std::string rez = "You win: " + std::to_string(handSum(weights, dcards)) + " vs " + std::to_string(handSum(weights, pcards));
//             ui->label_5->setText(QString::fromStdString(rez));
//             curBalance -= 1000;
//             ui->pushButton_5->hide();
//             ui->pushButton_6->hide();
//             ui->pushButton_7->hide();
//         }
//         else {
//             std::string rez = "Tie. Both of you have " + std::to_string(handSum(weights, dcards));
//             ui->label_5->setText(QString::fromStdString(rez));
//             ui->pushButton_5->hide();
//             ui->pushButton_6->hide();
//             ui->pushButton_7->hide();
//         }
//     }
// }

