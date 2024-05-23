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
    void setBalance(int newBalance) {
        if(balance != newBalance) {
            balance = newBalance;
            emit balanceChanged(balance); // Оповещение об изменении баланса
        }
    }
    void depositBalance(int amount) { setBalance(balance + amount); }
    void withdrawBalance(int amount) { setBalance(balance - amount); }

signals:
    void balanceChanged(int newBalance);

private:
    int balance;
    BalanceManager() : balance(0) { }
    ~BalanceManager() { }
    BalanceManager(const BalanceManager&) = delete;
    BalanceManager& operator=(const BalanceManager&) = delete;

};

#endif // BALANCEMANAGER_H
