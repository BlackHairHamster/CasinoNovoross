#ifndef TIMEOUTWINDOW_H
#define TIMEOUTWINDOW_H

#include <QDialog>
#include <QTimer>
#include <QLabel>

namespace Ui {
class timeoutwindow;
}

class timeoutwindow : public QDialog
{
    Q_OBJECT

public:
    explicit timeoutwindow(double, double, QWidget *parent = nullptr);
    ~timeoutwindow();

private slots:
    void on_pushButton_clicked();
    void onTimeout();
    void updateRemainingTime();

private:
    Ui::timeoutwindow *ui;
    double curBalance;
    double net;
    QTimer *timer;
    QTimer *updateTimer; // Timer for updating the display
    QLabel *timeLabel; // Label to display the remaining time
    int remainingTime; // Time remaining in milliseconds
};

#endif // TIMEOUTWINDOW_H
