#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(double balance = 0, double net = 0, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void onCheckBalance();

    void withdrawMoney(const QString&, double);

    void handleNetworkReply(QNetworkReply *reply);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    double generalBalance;
    double net;
    QNetworkAccessManager *networkManager;
};

#endif // MAINWINDOW_H
