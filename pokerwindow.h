#ifndef POKERWINDOW_H
#define POKERWINDOW_H

#include <QWidget>
#include "handsupgame.h"

namespace Ui {
class pokerwindow;
}

class pokerwindow : public QWidget
{
    Q_OBJECT

public:
    explicit pokerwindow(QWidget *parent = nullptr, QString login = "natriy", QString userName = "sus");
    ~pokerwindow();

private slots:
    void on_pokerDepositButton_clicked();
    void on_backButton_clicked();
    void updateDisplayedBalance(int newBalance);
    void on_pokerStartButton_clicked();

    void on_pokerDealButton_clicked();

    void on_pokerRaiseButton_clicked();

    void on_pokerBetButton_clicked();

    void on_pokerCheckButton_clicked();

    void on_pokerFoldButton_clicked();
    void updateDisplay();
    void on_pokerAgainButton_clicked();

signals:
    void balanceChanged(int newBalance);

private:
    Ui::pokerwindow *ui;
    int pokerPot;
    int pokerAnte;
    int pokerPrize;
    int isTurnBet;
    int isRiverBet;
    QString pokerStage;
    HandsUpGame game;
    QString login;
    QString userName;

};

#endif // POKERWINDOW_H
