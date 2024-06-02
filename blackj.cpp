#include "blackj.h"
#include "ui_blackj.h"
#include "mainwindow.h"
#include <random>
#include <vector>
#include <map>
#include <string>
#include <QThread>

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

blackj::blackj(QWidget *parent, QString login)
    : QDialog(parent)
    , ui(new Ui::blackj)
    , login(login)
{
    ui->setupUi(this);
    ui->blackjResultBar->hide();
    ui->blackjHitButton->hide();
    ui->blackjStayButton->hide();
    ui->blackjDoubleButton->hide();
    ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
            this, &blackj::updateDisplayedBalance);
    ui->blackjDealerCards2Widget->hide();
    ui->blackjDealerCards3Widget->hide();
    ui->blackjDealerCards4Widget->hide();
    ui->blackjDealerCards5Widget->hide();
    ui->blackjPlayerCards2Widget->hide();
    ui->blackjPlayerCards3Widget->hide();
    ui->blackjPlayerCards4Widget->hide();
    ui->blackjPlayerCards5Widget->hide();

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
    MainWindow *hub = new MainWindow(this, login);
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
    ui->blackjPrizeNum->setText(" ");
    ui->blackjResultBar->setText(QString::fromStdString(""));
    ui->blackjResultBar->show();

    ui->blackjDealerCards2Widget->hide();
    ui->blackjDealerCards3Widget->hide();
    ui->blackjDealerCards4Widget->hide();
    ui->blackjDealerCards5Widget->hide();
    ui->blackjPlayerCards2Widget->hide();
    ui->blackjPlayerCards3Widget->hide();
    ui->blackjPlayerCards4Widget->hide();
    ui->blackjPlayerCards5Widget->hide();

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
    //
    QString dealer = QString::fromStdString( dc1 + " ?");
    QString DCard1Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dc1)));
    QPixmap DCard1(DCard1Text);
    ui->blackjDealerCards2_1->setPixmap(DCard1);
    ui->blackjDealerCards3_1->setPixmap(DCard1);
    ui->blackjDealerCards4_1->setPixmap(DCard1);
    ui->blackjDealerCards5_1->setPixmap(DCard1);
    QPixmap backCard(":/files/img/cards1/backCard.png");
    ui->blackjDealerCards2_2->setPixmap(backCard);
    ui->blackjDealerCards2Widget->show();
    //
    ui->blackjDealerSumNum->setText(QString::fromStdString(weights[dc1]));
    //
    QString player = QString::fromStdString(pc1 + " "+ pc2);
    ui->blackjPlayerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, pcards))));
    QString PCard1Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pc1)));
    QPixmap PCard1(PCard1Text);
    ui->blackjPlayerCards2_1->setPixmap(PCard1);
    ui->blackjPlayerCards3_1->setPixmap(PCard1);
    ui->blackjPlayerCards4_1->setPixmap(PCard1);
    ui->blackjPlayerCards5_1->setPixmap(PCard1);
    QString PCard2Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pc2)));
    QPixmap PCard2(PCard2Text);
    ui->blackjPlayerCards2_2->setPixmap(PCard2);
    ui->blackjPlayerCards3_2->setPixmap(PCard2);
    ui->blackjPlayerCards4_2->setPixmap(PCard2);
    ui->blackjPlayerCards5_2->setPixmap(PCard2);
    ui->blackjPlayerCards2Widget->show();
    //
    //
    if (handSum(weights, pcards) == 21) {
        ui->blackjResultBar->setText(QString::fromStdString("Blackjack, you win!"));
        ui->blackjPlayButton->show();
        BalanceManager::balanceInstance().depositBalance(blackjBet*3);
        ui->blackjPrizeNum->setText(QString::number(blackjBet*3));
        ui->blackjBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    }
    else if (dc1 == "A" and handSum(weights, dcards) == 21) {
        QString DCard2Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dc2)));
        QPixmap DCard2(DCard2Text);
        ui->blackjDealerCards2_2->setPixmap(DCard2);
        ui->blackjDealerSumNum->setText("21");
        QThread::sleep(1);

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
    penis = penis;
    ui->blackjPlayerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, pcards))));

    if(pcards.size() == 3){
        ui->blackjPlayerCards2Widget->hide();
        QString PCard3Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[2])));
        QPixmap PCard3(PCard3Text);
        ui->blackjPlayerCards3_3->setPixmap(PCard3);
        ui->blackjPlayerCards4_3->setPixmap(PCard3);
        ui->blackjPlayerCards5_3->setPixmap(PCard3);
        ui->blackjPlayerCards3Widget->show();
    } else if (pcards.size()==4){
        ui->blackjPlayerCards3Widget->hide();
        QString PCard4Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[3])));
        QPixmap PCard4(PCard4Text);
        ui->blackjPlayerCards4_4->setPixmap(PCard4);
        ui->blackjPlayerCards5_4->setPixmap(PCard4);
        ui->blackjPlayerCards4Widget->show();
    } else {
        ui->blackjPlayerCards4Widget->hide();
        QString PCard5Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[4])));
        QPixmap PCard5(PCard5Text);
        ui->blackjPlayerCards5_5->setPixmap(PCard5);
        ui->blackjPlayerCards5Widget->show();
    }

    if (handSum(weights, pcards) > 21) {
        ui->blackjDealerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, dcards))));
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
        QString DCard2Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[1])));
        QPixmap DCard2(DCard2Text);
        ui->blackjDealerCards2_2->setPixmap(DCard2);
        ui->blackjDealerCards3_2->setPixmap(DCard2);
        ui->blackjDealerCards4_2->setPixmap(DCard2);
        ui->blackjDealerCards5_2->setPixmap(DCard2);
        while (handSum(weights, dcards) < 17) {
            int rand = random(0, deck.size() - 1);
            std::string dc = deck[rand];
            dcards.push_back(dc);
            deck.erase(deck.begin() + rand);
        }
        std::string penis = "";
        for (size_t i = 0; i < dcards.size(); i++) {
            penis = penis + dcards[i] + " ";
            if(dcards.size() >= 3){
                ui->blackjDealerCards2Widget->hide();
                QString DCard3Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[2])));
                QPixmap DCard3(DCard3Text);
                ui->blackjDealerCards3_3->setPixmap(DCard3);
                ui->blackjDealerCards4_3->setPixmap(DCard3);
                ui->blackjDealerCards5_3->setPixmap(DCard3);
                ui->blackjDealerCards3Widget->show();
            } else if (dcards.size()>=4){
                ui->blackjDealerCards3Widget->hide();
                QString DCard4Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[3])));
                QPixmap DCard4(DCard4Text);
                ui->blackjDealerCards4_4->setPixmap(DCard4);
                ui->blackjDealerCards5_4->setPixmap(DCard4);
                ui->blackjDealerCards4Widget->show();
            } else {
                ui->blackjDealerCards4Widget->hide();
                QString DCard5Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[4])));
                QPixmap DCard5(DCard5Text);
                ui->blackjDealerCards5_5->setPixmap(DCard5);
                ui->blackjDealerCards5Widget->show();
            }
        }
        penis = penis;
        ui->blackjDealerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, dcards))));
        ui->blackjBetNum->setDisabled(false);
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
    QString DCard2Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[1])));
    QPixmap DCard2(DCard2Text);
    ui->blackjDealerCards2_2->setPixmap(DCard2);
    ui->blackjDealerCards3_2->setPixmap(DCard2);
    ui->blackjDealerCards4_2->setPixmap(DCard2);
    ui->blackjDealerCards5_2->setPixmap(DCard2);
    while (handSum(weights, dcards) < 17) {
        int rand = random(0, deck.size() - 1);
        std::string dc = deck[rand];
        dcards.push_back(dc);
        deck.erase(deck.begin() + rand);
    }
    std::string penis = "";
    for (size_t i = 0; i < dcards.size(); i++) {
        penis = penis + dcards[i] + " ";

        if(dcards.size() >= 3){
            ui->blackjDealerCards2Widget->hide();
            QString DCard3Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[2])));
            QPixmap DCard3(DCard3Text);
            ui->blackjDealerCards3_3->setPixmap(DCard3);
            ui->blackjDealerCards4_3->setPixmap(DCard3);
            ui->blackjDealerCards5_3->setPixmap(DCard3);
            ui->blackjDealerCards3Widget->show();
        } if (dcards.size()>=4){
            ui->blackjDealerCards2Widget->hide();
            ui->blackjDealerCards3Widget->hide();
            QString DCard4Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[3])));
            QPixmap DCard4(DCard4Text);
            ui->blackjDealerCards4_4->setPixmap(DCard4);
            ui->blackjDealerCards5_4->setPixmap(DCard4);
            ui->blackjDealerCards4Widget->show();
        } if(dcards.size() == 5){
            ui->blackjDealerCards2Widget->hide();
            ui->blackjDealerCards3Widget->hide();
            ui->blackjDealerCards4Widget->hide();
            QString DCard5Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[4])));
            QPixmap DCard5(DCard5Text);
            ui->blackjDealerCards5_5->setPixmap(DCard5);
            ui->blackjDealerCards5Widget->show();
        }
    }
    penis = penis;
    ui->blackjDealerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, dcards))));
    ui->blackjBetNum->setDisabled(false);
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
    QString DCard2Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[1])));
    QPixmap DCard2(DCard2Text);
    ui->blackjDealerCards2_2->setPixmap(DCard2);
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
        if(pcards.size() == 3){
            ui->blackjPlayerCards2Widget->hide();
            QString PCard3Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[2])));
            QPixmap PCard3(PCard3Text);
            ui->blackjPlayerCards3_3->setPixmap(PCard3);
            ui->blackjPlayerCards3Widget->show();
        } else if (pcards.size()==4){
            ui->blackjPlayerCards3Widget->hide();
            QString PCard4Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[3])));
            QPixmap PCard4(PCard4Text);
            ui->blackjPlayerCards4_4->setPixmap(PCard4);
            ui->blackjPlayerCards4Widget->show();
        } else {
            ui->blackjPlayerCards4Widget->hide();
            QString PCard5Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((pcards[4])));
            QPixmap PCard5(PCard5Text);
            ui->blackjPlayerCards5_5->setPixmap(PCard5);
            ui->blackjPlayerCards5Widget->show();
        }
    }
    penis = penis;
    ui->blackjPlayerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, pcards))));
    ui->blackjBetNum->setDisabled(false);
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

            if(dcards.size() >= 3){
                ui->blackjDealerCards2Widget->hide();
                QString DCard3Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[2])));
                QPixmap DCard3(DCard3Text);
                ui->blackjDealerCards3_3->setPixmap(DCard3);
                ui->blackjDealerCards4_3->setPixmap(DCard3);
                ui->blackjDealerCards5_3->setPixmap(DCard3);
                ui->blackjDealerCards3Widget->show();
            } if (dcards.size()>=4){
                ui->blackjDealerCards3Widget->hide();
                QString DCard4Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[3])));
                QPixmap DCard4(DCard4Text);
                ui->blackjDealerCards4_4->setPixmap(DCard4);
                ui->blackjDealerCards5_4->setPixmap(DCard4);
                ui->blackjDealerCards4Widget->show();
            } if(dcards.size() == 5){
                ui->blackjDealerCards4Widget->hide();
                QString DCard5Text = QString(":/files/img/cards1/%1s.png").arg(QString::fromStdString((dcards[4])));
                QPixmap DCard5(DCard5Text);
                ui->blackjDealerCards5_5->setPixmap(DCard5);
                ui->blackjDealerCards5Widget->show();
            }
        }
        penis = penis;
        ui->blackjDealerSumNum->setText(QString::fromStdString(std::to_string(handSum(weights, dcards))));
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
                ui->blackjBetNum->setDisabled(false);
            }
        }
    }
}



