#ifndef BALANCEMANAGER_H
#define BALANCEMANAGER_H

#include <QObject>

class BalanceManager : public QObject {
    Q_OBJECT
public:
    static BalanceManager& balanceInstance() {
        static BalanceManager balanceManagerInstance;
        return balanceManagerInstance;
    }

    int getBalance() const { return balance; }
    void setBalance(double newBalance) {
        if(balance != newBalance) {
            balance = newBalance;
            emit balanceChanged(balance); // Оповещение об изменении баланса
        }
    }
    void depositBalance(double amount) { setBalance(balance + amount); }
    void withdrawBalance(double amount) { setBalance(balance - amount); }
    //void changeNet(double amount) {net + amount};

signals:
    void balanceChanged(double newBalance);

private:
    double balance;
    double net;
    BalanceManager() : balance(0) { }
    ~BalanceManager() { }
    BalanceManager(const BalanceManager&) = delete;
    BalanceManager& operator=(const BalanceManager&) = delete;

};

#endif // BALANCEMANAGER_H
