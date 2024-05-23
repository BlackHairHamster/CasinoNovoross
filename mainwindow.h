#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "balanceManager.h"
#include "pokerwindow.h"
#include "loginization.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateBalanceDisplay(int newBalance);


private slots:
    void on_depositButton_clicked();
    void on_posterButton1_clicked();
    void on_signInButton_clicked();

    void on_signUpButton_clicked();

private:
    Ui::MainWindow *ui;
    pokerwindow *window;
    loginization *loginWindow;
};
#endif // MAINWINDOW_H
