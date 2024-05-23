#include "pokerwindow.h"
#include "handsupgame.h"
#include "balanceManager.h"
#include "ui_pokerwindow.h"
#include <QPixmap>
#include <QMessageBox>

pokerwindow::pokerwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pokerwindow)
    , pokerPot(0)
    , pokerAnte(0)
    , pokerPrize(0)
    , pokerStage("start")
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
        QString playerHandText = QString("%1 %2 ")
                                     .arg(cardToText(playerHand.first))
                                     .arg(cardToText(playerHand.second));
        QString dealerHandText = QString("%1 %2 ")
                                     .arg(cardToText(dealerHand.first))
                                     .arg(cardToText(dealerHand.second));

        ui->pokerPHandContent->setText(playerHandText);
        ui->pokerDHandContent->setText(dealerHandText);
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
        ui->pokerFlopContent->setText(tableText);
    } else if (pokerStage == "river") {
        QString tableText = QString("%1").arg(cardToText(tableCards[3]));
        ui->pokerTurnContent->setText(tableText);
    } else if (pokerStage == "showdown") {
        QString tableText = QString("%1").arg(cardToText(tableCards[4]));
        ui->pokerRiverContent->setText(tableText);
        game.printAll();
        ui->pokerFlopBar->hide();
        ui->pokerLoseBar->show();
        if (game.getWinner() == "player"){
            pokerPrize = 2*pokerPot;
            QString winMessage = QString("You win +%1").arg(pokerPrize);
            QMessageBox::information(this, "", winMessage);
            ui->pokerPrizeNum->setText(QString::number(pokerPrize));
            BalanceManager::balanceInstance().depositBalance(pokerPrize);
            ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        } else if (game.getWinner() == "dealer"){
            QString loseMessage = QString("You lose -%1").arg(pokerPot);
            QMessageBox::information(this, "", loseMessage);
            ui->pokerPreflopBar->hide();
            ui->pokerLoseBar->show();
        } else if (game.getWinner() == "tie") {
            pokerPrize = pokerPot;
            QMessageBox::information(this, "", "Tie");
            ui->pokerPrizeNum->setText(QString::number(pokerPrize));
            BalanceManager::balanceInstance().depositBalance(pokerPrize);
            ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        }
    } else if (pokerStage == "again"){
        ui->pokerFlopContent->setText("");
        ui->pokerTurnContent->setText("");
        ui->pokerRiverContent->setText("");
        ui->pokerPHandContent->setText("");
        ui->pokerDHandContent->setText("");
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
    QString loseMessage = QString("You lose -%1").arg(pokerPot);
    QMessageBox::information(this, "", loseMessage);
    ui->pokerPreflopBar->hide();
    ui->pokerLoseBar->show();
}



void pokerwindow::on_pokerBetButton_clicked()
{
    if (pokerStage == "turn"){
        pokerPot+=pokerAnte;
        ui->pokerPotNum->setText(QString::number(pokerPot));
        BalanceManager::balanceInstance().withdrawBalance(pokerAnte);
        ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->turnNum->setText(QString::number(pokerAnte));
        pokerStage = "river";
        updateDisplay();


    } else if (pokerStage == "river") {
        pokerPot+=pokerAnte;
        ui->pokerPotNum->setText(QString::number(pokerPot));
        BalanceManager::balanceInstance().withdrawBalance(pokerAnte);
        ui->pokerBalanceNum->setText(QString::number(BalanceManager::balanceInstance().getBalance()));
        ui->riverNum->setText(QString::number(pokerAnte));
        pokerStage = "showdown";
        updateDisplay();
    }

}


void pokerwindow::on_pokerCheckButton_clicked()
{
    if (pokerStage == "turn"){
        ui->turnNum->setText("0");
        pokerStage = "river";
        updateDisplay();


    } else if (pokerStage=="river") {
        ui->riverNum->setText("0");
        pokerStage = "showdown";
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

