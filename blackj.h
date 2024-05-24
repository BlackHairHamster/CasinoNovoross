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
    explicit blackj(int balance, QWidget *parent = nullptr);
    ~blackj();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::blackj *ui;
    int curBalance;
};

#endif // BLACKJ_H
