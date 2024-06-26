#include "pokerwindow.h"
#include "handsupgame.h"
#include "balanceManager.h"
#include "ui_pokerwindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QThread>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <sqlite3.h>
#include <iostream>


pokerwindow::pokerwindow(QWidget *parent, QString login, QString userName )
    : QWidget(parent)
    , ui(new Ui::pokerwindow)
    , pokerPot(0)
    , pokerAnte(0)
    , pokerPrize(0)
    , pokerStage("start")
    , login(login)
    , userName(userName)
{
    ui->setupUi(this);
    ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    connect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
            this, &pokerwindow::updateDisplayedBalance);
    connect(ui->pokerStartButton, SIGNAL(clicked()), this, SLOT(on_StartPokerButton_Clicked()));
    ui->gamePokerWidget->hide();
    ui->pokerPreflopBar->hide();
    ui->pokerFlopBar->hide();
    ui->pokerLoseBar->hide();
    ui->pokerMysteryWidget->hide();
    QPixmap mysteryCard(":/files/img/cards1/backCard.png");
    ui->mysteryCard1->setPixmap(mysteryCard);
    ui->mysteryCard2->setPixmap(mysteryCard);
    QPixmap flopBG(":/files/img/flopBG.png");

    ui->flopBackground->setPixmap(flopBG);
    QPixmap turnBG(":/files/img/turnBG.png");
    ui->turnCell->setPixmap(turnBG);
    QPixmap riverBG(":/files/img/riverBG.png");
    ui->riverBackground->setPixmap(riverBG);
}



pokerwindow::~pokerwindow()
{
    // Отказываемся от подписки, если окно закрывается
    disconnect(&BalanceManager::balanceInstance(), &BalanceManager::balanceChanged,
               this, &pokerwindow::updateDisplayedBalance);

    delete ui;
}

std::string unWrapHand(hand hand) {
    std::map<int, std::string> suit{
                                    {0, "c"}, {1, "d"}, {2, "h"}, {3, "s"}};
    std::map<int, std::string> nominal{
                                       {1, "2"}, {2, "3"}, {3, "4"}, {4, "5"}, {5, "6"},
                                       {6, "7"}, {7, "8"}, {8, "9"}, {9, "T"}, {10, "J"},
                                       {11, "Q"}, {12, "K"}, {13, "A"}};
    return nominal[hand.first.first] + suit[hand.first.second] + " " + nominal[hand.second.first] + suit[hand.second.second];
}

std::string unWrapTable(table table) {
    std::map<int, std::string> suit{
                                    {0, "c"}, {1, "d"}, {2, "h"}, {3, "s"}};
    std::map<int, std::string> nominal{
                                       {1, "2"}, {2, "3"}, {3, "4"}, {4, "5"}, {5, "6"},
                                       {6, "7"}, {7, "8"}, {8, "9"}, {9, "T"}, {10, "J"},
                                       {11, "Q"}, {12, "K"}, {13, "A"}};
    std::string ans = "";
    for (size_t i = 0; i < 5; i++) {
        ans += nominal[table[i].first] + suit[table[i].second] + " ";
    }
    return ans;
}



void pokerwindow::on_pokerDepositButton_clicked()
{
    BalanceManager::balanceInstance().depositBalance(1000);
    ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
}


void pokerwindow::updateDisplayedBalance(int newBalance)
{
    ui->pokerBalanceNum->setText(QString::number(newBalance));
}


void pokerwindow::on_backButton_clicked()
{
    emit balanceChanged(BalanceManager::balanceInstance().getBalance());
    this->hide();
}


void pokerwindow::updateDisplay()
{
    const hand& playerHand = game.getPlayerHand();
    const hand& dealerHand = game.getDealerHand();
    const table& tableCards = game.getTableCards();
    std::map<int, std::string> suit{
                                    {0, "c"}, {1, "d"}, {2, "h"}, {3, "s"}};
    std::map<int, std::string> nominal{
                                       {1, "2"}, {2, "3"}, {3, "4"}, {4, "5"}, {5, "6"},
                                       {6, "7"}, {7, "8"}, {8, "9"}, {9, "T"}, {10, "J"},
                                       {11, "Q"}, {12, "K"}, {13, "A"}};



    auto cardToText = [&](const card& c) {
        return QString("%1%2").arg(QString::fromStdString(nominal[c.first])).arg(QString::fromStdString(suit[c.second]));
    };

    if (pokerStage == "flop"){
        QString PCard1Text = QString(":/files/img/cards1/%1.png").arg(cardToText(playerHand.first));
        QString PCard2Text = QString(":/files/img/cards1/%1.png").arg(cardToText(playerHand.second));
        QPixmap PCard1(PCard1Text);
        QPixmap PCard2(PCard2Text);
        QString DCard1Text = QString(":/files/img/cards1/%1.png").arg(cardToText(dealerHand.first));
        QString DCard2Text = QString(":/files/img/cards1/%1.png").arg(cardToText(dealerHand.second));
        QPixmap DCard1(DCard1Text);
        QPixmap DCard2(DCard2Text);

        ui->pokerPCard1->setPixmap(PCard1);
        ui->pokerPCard2->setPixmap(PCard2);
        ui->pokerDCard1->setPixmap(DCard1);
        ui->pokerDCard2->setPixmap(DCard2);
    } else if (pokerStage == "turn"){
        QString tableText;
        int cardCounter = 0;
        for (const auto& card : tableCards) {
            if (cardCounter < 3) {
                tableText += QString("%1 ").arg(cardToText(card));
                cardCounter++;
            } else {
                break;
            }

        }
        QString flopCardText1 = QString(":/files/img/cards1/%1.png").arg(cardToText(tableCards[0]));
        QPixmap flopCard1(flopCardText1);
        ui->pokerFlopCard1->setPixmap(flopCard1);
        QString flopCardText2 = QString(":/files/img/cards1/%1.png").arg(cardToText(tableCards[1]));
        QPixmap flopCard2(flopCardText2);
        ui->pokerFlopCard2->setPixmap(flopCard2);
        QString flopCardText3 = QString(":/files/img/cards1/%1.png").arg(cardToText(tableCards[2]));
        QPixmap flopCard3(flopCardText3);
        ui->pokerFlopCard3->setPixmap(flopCard3);
    } else if (pokerStage == "river") {
        QString turnCardText = QString(":/files/img/cards1/%1.png").arg(cardToText(tableCards[3]));
        QPixmap turnCard(turnCardText);
        ui->pokerTurnCard->setPixmap(turnCard);
    } else if (pokerStage == "showdown") {
        QString riverCardText = QString(":/files/img/cards1/%1.png").arg(cardToText(tableCards[4]));
        QPixmap riverCard(riverCardText);
        ui->pokerRiverCard->setPixmap(riverCard);
        game.printAll();
        ui->pokerFlopBar->hide();
        ui->pokerLoseBar->show();

        sqlite3* db;
        char* errMsg = 0;
        int rc = sqlite3_open("/Users/capybastercarbonaster/Desktop/RegistrationDB.db", &db);


        std::string sqlStatement = "CREATE TABLE IF NOT EXISTS games ("
                                   "login_of_user TEXT, "
                                   "player_cards TEXT, "
                                   "dealer_cards TEXT, "
                                   "table_cards  TEXT, "
                                   "ante TEXT, "
                                   "flop TEXT, "
                                   "turn TEXT, "
                                   "river TEXT, "
                                   "game_rez TEXT, "
                                   "winning_comb TEXT, "
                                   "balance_change TEXT)";

        rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error";
            sqlite3_free(errMsg);
        }
        QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
        dbq.setDatabaseName("/Users/capybastercarbonaster/Desktop/RegistrationDB.db");
        if (!dbq.open()) {
            std::cout << "Unable to open the database"; // без этого не работает
            QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");
            return;
        }

        std::string stringHandP = unWrapHand(game.getPlayerHand());
        std::string stringHandD = unWrapHand(game.getPlayerHand());
        std::string stringTable = unWrapTable(game.getTableCards());
        std::string loginn = login.toStdString();
        std::string whoWon = game.getWinner();
        std::string change;
        if (whoWon ==  "player") change = std::to_string(3*pokerAnte + pokerAnte*isTurnBet + pokerAnte*isRiverBet);
        else if (whoWon == "dealer") change = "-" + std::to_string(3*pokerAnte + pokerAnte*isTurnBet + pokerAnte*isRiverBet);
        else change = "0";

        sqlStatement = "INSERT INTO games (login_of_user, player_cards, dealer_cards, table_cards, ante, flop, turn, river, game_rez, winning_comb, balance_change) VALUES ('" + loginn + "', '" + stringHandP + "', '" + stringHandD + "','" + stringTable + "', '" + std::to_string(pokerAnte)  + "', '" + std::to_string(pokerAnte*2) + "', '" + std::to_string(pokerAnte*isTurnBet) + "', '" + std::to_string(pokerAnte*isRiverBet) + "', '" + whoWon + "', '" + game.getWinningComb() + "', '" + change + "');";
        rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
        sqlite3_close(db);


        if (game.getWinner() == "player"){
            pokerPrize = 2*(pokerPot-pokerAnte);
            QString winMessage = QString("You win %1 with %2").arg(pokerPrize).arg(QString::fromStdString(game.getWinningComb()));
            QMessageBox::information(this, "", winMessage);
            ui->pokerPrizeNum->setText(QString::number(pokerPrize));
            BalanceManager::balanceInstance().depositBalance(pokerPrize);
            ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        } else if (game.getWinner() == "dealer"){

            QString loseMessage = QString("You lose %1 to %2").arg(pokerPot).arg(QString::fromStdString(game.getWinningComb()));
            QMessageBox::information(this, "", loseMessage);
            ui->pokerPreflopBar->hide();
            ui->pokerLoseBar->show();
        } else if (game.getWinner() == "tie") {
            pokerPrize = pokerPot;
            QString tieMessage = QString("Tie: %1").arg(QString::fromStdString(game.getWinningComb()));
            QMessageBox::information(this, "", tieMessage);
            ui->pokerPrizeNum->setText(QString::number(pokerPrize));
            BalanceManager::balanceInstance().depositBalance(pokerPrize);
            ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        }
    } else if (pokerStage == "again"){
        ui->pokerFlopCard1->setPixmap(QPixmap());
        ui->pokerFlopCard2->setPixmap(QPixmap());
        ui->pokerFlopCard3->setPixmap(QPixmap());
        ui->pokerTurnCard->setPixmap(QPixmap());
        ui->pokerRiverCard->setPixmap(QPixmap());
        ui->pokerPCard1->setPixmap(QPixmap());
        ui->pokerPCard2->setPixmap(QPixmap());
        ui->pokerDCard1->setPixmap(QPixmap());
        ui->pokerDCard2->setPixmap(QPixmap());
    }

}

void pokerwindow::on_pokerStartButton_clicked()
{
    ui->gamePokerWidget->show();
    ui->mainPokerMenuWidget->hide();
    pokerStage="ante";
}


void pokerwindow::on_pokerDealButton_clicked()
{
    QString anteBet = ui->anteInputBar->text();
    pokerAnte = anteBet.toInt();
    ui->anteInputBar->setDisabled(true);
    pokerPot = pokerAnte;
    ui->pokerPotNum->setText(QString::number(pokerPot));
    BalanceManager::balanceInstance().withdrawBalance(pokerAnte);
    ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    ui->pokerDealBar->hide();
    ui->pokerPreflopBar->show();
    ui->pokerMysteryWidget->show();
    pokerStage="flop";
    game.startNewGame();
    updateDisplay();
}



void pokerwindow::on_pokerRaiseButton_clicked()
{
    pokerPot*=3;
    ui->pokerPotNum->setText(QString::number(pokerPot));
    BalanceManager::balanceInstance().withdrawBalance(2*pokerAnte);
    ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
    ui->flopNum->setText(QString::number(2*pokerAnte));
    ui->pokerPreflopBar->hide();
    ui->pokerFlopBar->show();
    pokerStage="turn";
    updateDisplay();

}

void pokerwindow::on_pokerFoldButton_clicked()
{
    pokerStage = "lose";
    QString loseMessage = QString("You lose %1").arg(pokerPot);
    QMessageBox::information(this, "", loseMessage);
    ui->pokerPreflopBar->hide();
    ui->pokerLoseBar->show();

    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("/Users/capybastercarbonaster/Desktop/RegistrationDB.db", &db);


    std::string sqlStatement = "CREATE TABLE IF NOT EXISTS games ("
                                   "login_of_user TEXT, "
     "player_cards TEXT, "
     "dealer_cards TEXT, "
     "table_cards  TEXT, "
     "ante TEXT, "
     "flop TEXT, "
     "turn TEXT, "
     "river TEXT, "
     "game_rez TEXT, "
     "winning_comb TEXT, "
     "balance_change TEXT)";

    rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error";
        sqlite3_free(errMsg);
    }
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/capybastercarbonaster/Desktop/RegistrationDB.db");
    if (!dbq.open()) {
        std::cout << "Unable to open the database"; // без этого не работает
        QMessageBox::information(this, "бд не подключена","чушпанчик, базу данных кто подлючать будет? не по пацански...");
        return;
    }

    std::string stringHandP = unWrapHand(game.getPlayerHand());
    std::string loginn = login.toStdString();
    std::string DNF = "DNF";
    std::string change = "-" + std::to_string(pokerAnte);

    sqlStatement = "INSERT INTO games (login_of_user, player_cards, dealer_cards, table_cards, ante, flop, turn, river, game_rez, winning_comb, balance_change) VALUES ('" + loginn + "', '" + stringHandP + "', '" + "-" + "','" + "-" + "', '" + std::to_string(pokerAnte)  + "', '" + DNF + "', '" + DNF + "', '" + DNF + "', '" + "dealer" + "', '" + "-" + "', '" + change + "');";
            rc = sqlite3_exec(db, sqlStatement.c_str(), 0, 0, &errMsg);
            sqlite3_close(db);

}



void pokerwindow::on_pokerBetButton_clicked()
{
    if (pokerStage == "turn"){
        isTurnBet = 1;
        pokerPot+=pokerAnte;
        ui->pokerPotNum->setText(QString::number(pokerPot));
        BalanceManager::balanceInstance().withdrawBalance(pokerAnte);
        ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->turnNum->setText(QString::number(pokerAnte));
        pokerStage = "river";
        updateDisplay();


    } else if (pokerStage == "river") {
        isRiverBet = 1;
        pokerPot+=pokerAnte;
        ui->pokerPotNum->setText(QString::number(pokerPot));
        BalanceManager::balanceInstance().withdrawBalance(pokerAnte);
        ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->riverNum->setText(QString::number(pokerAnte));
        pokerStage = "showdown";
        ui->pokerMysteryWidget->hide();
        updateDisplay();
    }

}


void pokerwindow::on_pokerCheckButton_clicked()
{
    if (pokerStage == "turn"){
        isTurnBet = 0;
        ui->turnNum->setText("0");
        pokerStage = "river";
        updateDisplay();


    } else if (pokerStage=="river") {
        isRiverBet = 0;
        ui->riverNum->setText("0");
        pokerStage = "showdown";
        ui->pokerMysteryWidget->hide();
        updateDisplay();
        ui->pokerFlopBar->hide();
        ui->pokerLoseBar->show();
    }
}





void pokerwindow::on_pokerAgainButton_clicked()
{
    pokerStage = "again";
    updateDisplay();
    ui->pokerLoseBar->hide();
    ui->pokerDealBar->show();
    ui->pokerPotNum->setText("0");
    ui->pokerPrizeNum->setText("0");
    ui->flopNum->setText("0");
    ui->turnNum->setText("0");
    ui->riverNum->setText("0");
    ui->anteInputBar->setDisabled(false);
}



