#ifndef BLACKJ_H
#define BLACKJ_H

#include <QDialog>

namespace Ui {
class blackj;
}

class blackj : public QDialog
{
    Q_OBJECT

public:
    explicit blackj(QWidget *parent = nullptr);
    ~blackj();

private slots:
    void on_blackjBackButton_clicked();

    void on_blackjPlayButton_clicked();

    void on_blackjHitButton_clicked();

    void on_blackjStayButton_clicked();

    void on_blackjDoubleButton_clicked();

    void updateDisplayedBalance(int newBalance);

    void on_blackjDepositButton_clicked();


signals:
    void balanceChanged(int newBalance);

private:
    Ui::blackj *ui;
    int blackjBet;
};

#endif // BLACKJ_H
