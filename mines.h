#ifndef MINES_H
#define MINES_H

#include <QDialog>
#include <vector>

namespace Ui {
class mines;
}

class mines : public QDialog
{
    Q_OBJECT

public:
    explicit mines(int balance, QWidget *parent = nullptr, std::vector<std::vector<int>> zeroField = std::vector<std::vector<int>>(5, std::vector<int>(5, 0)));
    ~mines();

private slots:
    void hideMines();
    void showMines();
    void fieldReveal();
    void hideMinesNumberChoice();
    void showMinesNumberChoice();
    void checkCell(int, int, QPushButton*);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

private:
    Ui::mines *ui;
    int curBalance;
    std::vector<std::vector<int>> field;
    std::vector<std::vector<int>> clickedCells;
    std::vector<QPushButton*> buttons;
    int minesNumber;
    double winProbability;
    int cellsOpened;
    double multiplier;
};

#endif // MINES_H
