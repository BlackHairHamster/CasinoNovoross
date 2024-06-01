#include "mines.h"
#include "ui_mines.h"
#include "mainwindow.h"
#include <string>

int randomm(int, int);
double roundd(double);

mines::mines(int balance, QWidget *parent, std::vector<std::vector<int>> zeroField)
    : QDialog(parent)
    , ui(new Ui::mines)
    , field(zeroField)
    , curBalance(balance)
{
    ui->setupUi(this);
    ui->label->setText(QString::number(curBalance));

    buttons = {
        ui->pushButton_2,
        ui->pushButton_3,
        ui->pushButton_4,
        ui->pushButton_5,
        ui->pushButton_6,
        ui->pushButton_7,
        ui->pushButton_9,
        ui->pushButton_11,
        ui->pushButton_8,
        ui->pushButton_10,
        ui->pushButton_12,
        ui->pushButton_14,
        ui->pushButton_16,
        ui->pushButton_13,
        ui->pushButton_15,
        ui->pushButton_17,
        ui->pushButton_19,
        ui->pushButton_21,
        ui->pushButton_18,
        ui->pushButton_20,
        ui->pushButton_22,
        ui->pushButton_24,
        ui->pushButton_26,
        ui->pushButton_23,
        ui->pushButton_25
    };
    minesNumber = 3;
    winProbability = 1;
    cellsOpened = 0;
    clickedCells = zeroField;
    hideMines();
    ui->pushButton_31->hide();
}

mines::~mines()
{
    delete ui;
}

int randomm(int min, int max)
{
    static bool firstt = true;
    if (firstt)
    {
        srand( time(NULL) );
        firstt = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

double roundd(double a)
{
    double value = (int)(a * 100 + .5);
    return (double)value / 100;
}

void mines::hideMines() {
    for (size_t i = 0; i < 25; i++) {
        buttons[i]->hide();
    }
}

void mines::showMines() {
    for (size_t i = 0; i < 25; i++) {
        buttons[i]->show();
        buttons[i]->setText(QString::fromStdString("?"));
    }
}


void mines::fieldReveal() {
    for (size_t i = 0; i < 25; i++) {
        buttons[i]->setText(QString::fromStdString(std::to_string(field[i/5][i%5])));
    }
    ui->pushButton->show();
}

void mines::hideMinesNumberChoice() {
    ui->label_3->hide();
    ui->pushButton_28->hide();
    ui->pushButton_29->hide();
    ui->pushButton_30->hide();
}

void mines::showMinesNumberChoice() {
    ui->label_3->show();
    ui->pushButton_28->show();
    ui->pushButton_29->show();
    ui->pushButton_30->show();
}

void mines::checkCell(int a, int b, QPushButton* button) {
    if (field[a][b] == 0) {
        winProbability *= (double) (25 - cellsOpened - minesNumber)/(25 - cellsOpened);
        multiplier = roundd((double) (97/(100 * winProbability)));
        ui->label_4->setText(QString::fromStdString(std::to_string(winProbability)));
        ui->label_5->setText(QString::fromStdString(std::to_string(multiplier)));
        button->setText(QString::fromStdString("0"));
        cellsOpened++;
        if (cellsOpened == 1) ui->pushButton_31->show();
        ui->label_6->show();
        ui->label_6->setText(QString::fromStdString(std::to_string((int)(multiplier * 1000))));
    }
    else {
        fieldReveal();
        cellsOpened = 0;
        winProbability = 1;
        ui->pushButton_31->hide();
        ui->label_6->hide();
        curBalance -= 1000;
        ui->label->setText(QString::number(curBalance));
        showMinesNumberChoice();
    }
}

void mines::on_pushButton_clicked() {
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            field[i][j] = 0;
            clickedCells[i][j] = 0;
        }
    }
    int cnt = 0;
    while (cnt < minesNumber) {
        int rand1 = randomm(0, 4);
        int rand2 = randomm(0, 4);
        if (field[rand1][rand2] != 1){
            field[rand1][rand2] = 1;
            cnt++;
        }
    }

    std::string pencil;
    for (size_t i = 0; i < 5; i++) {
        for (size_t j = 0; j < 5; j++) {
            pencil += std::to_string(field[i][j]) + " ";
            if (j==4) pencil += '\n';
        }
    }
    ui->label_2->setText(QString::fromStdString(pencil));

    hideMinesNumberChoice();
    ui->pushButton->hide();
    showMines();
}

void mines::on_pushButton_2_clicked()
{
    if (clickedCells[0][0] == 0) {
        checkCell(0, 0, ui->pushButton_2);
        clickedCells[0][0] = 1;
    }
}


void mines::on_pushButton_3_clicked()
{
    if (clickedCells[0][1] == 0) {
        checkCell(0, 1, ui->pushButton_3);
        clickedCells[0][1] = 1;
    }
}


void mines::on_pushButton_4_clicked()
{
    if (clickedCells[0][2] == 0) {
        checkCell(0, 2, ui->pushButton_4);
        clickedCells[0][2] = 1;
    }
}


void mines::on_pushButton_5_clicked()
{
    if (clickedCells[0][3] == 0) {
        checkCell(0, 3, ui->pushButton_5);
        clickedCells[0][3] = 1;
    }
}


void mines::on_pushButton_6_clicked()
{
    if (clickedCells[0][4] == 0) {
        checkCell(0, 4, ui->pushButton_6);
        clickedCells[0][4] = 1;
    }
}


void mines::on_pushButton_7_clicked()
{
    if (clickedCells[1][0] == 0) {
        checkCell(1, 0, ui->pushButton_7);
        clickedCells[1][0] = 1;
    }
}


void mines::on_pushButton_9_clicked()
{
    if (clickedCells[1][1] == 0) {
        checkCell(1, 1, ui->pushButton_9);
        clickedCells[1][1] = 1;
    }
}


void mines::on_pushButton_11_clicked()
{
    if (clickedCells[1][2] == 0) {
        checkCell(1, 2, ui->pushButton_11);
        clickedCells[1][2] = 1;
    }
}


void mines::on_pushButton_8_clicked()
{
    if (clickedCells[1][3] == 0) {
        checkCell(1, 3, ui->pushButton_8);
        clickedCells[1][3] = 1;
    }
}


void mines::on_pushButton_10_clicked()
{
    if (clickedCells[1][4] == 0) {
        checkCell(1, 4, ui->pushButton_10);
        clickedCells[1][4] = 1;
    }
}


void mines::on_pushButton_12_clicked()
{
    if (clickedCells[2][0] == 0) {
        checkCell(2, 0, ui->pushButton_12);
        clickedCells[2][0] = 1;
    }
}


void mines::on_pushButton_14_clicked()
{
    if (clickedCells[2][1] == 0) {
        checkCell(2, 1, ui->pushButton_14);
        clickedCells[2][1] = 1;
    }
}


void mines::on_pushButton_16_clicked()
{
    if (clickedCells[2][2] == 0) {
        checkCell(2, 2, ui->pushButton_16);
        clickedCells[2][2] = 1;
    }
}


void mines::on_pushButton_13_clicked()
{
    if (clickedCells[2][3] == 0) {
        checkCell(2, 3, ui->pushButton_13);
        clickedCells[2][3] = 1;
    }
}


void mines::on_pushButton_15_clicked()
{
    if (clickedCells[2][4] == 0) {
        checkCell(2, 4, ui->pushButton_15);
        clickedCells[2][4] = 1;
    }
}


void mines::on_pushButton_17_clicked()
{
    if (clickedCells[3][0] == 0) {
        checkCell(3, 0, ui->pushButton_17);
        clickedCells[3][0] = 1;
    }
}


void mines::on_pushButton_19_clicked()
{
    if (clickedCells[3][1] == 0) {
        checkCell(3, 1, ui->pushButton_19);
        clickedCells[3][1] = 1;
    }
}


void mines::on_pushButton_21_clicked()
{
    if (clickedCells[3][2] == 0) {
        checkCell(3, 2, ui->pushButton_21);
        clickedCells[3][2] = 1;
    }
}


void mines::on_pushButton_18_clicked()
{
    if (clickedCells[3][3] == 0) {
        checkCell(3, 3, ui->pushButton_18);
        clickedCells[3][3] = 1;
    }
}


void mines::on_pushButton_20_clicked()
{
    if (clickedCells[3][4] == 0) {
        checkCell(3, 4, ui->pushButton_20);
        clickedCells[3][4] = 1;
    }
}


void mines::on_pushButton_22_clicked()
{
    if (clickedCells[4][0] == 0) {
        checkCell(4, 0, ui->pushButton_22);
        clickedCells[4][0] = 1;
    }
}


void mines::on_pushButton_24_clicked()
{
    if (clickedCells[4][1] == 0) {
        checkCell(4, 1, ui->pushButton_24);
        clickedCells[4][1] = 1;
    }
}


void mines::on_pushButton_26_clicked()
{
    if (clickedCells[4][2] == 0) {
        checkCell(4, 2, ui->pushButton_26);
        clickedCells[4][2] = 1;
    }
}


void mines::on_pushButton_23_clicked()
{
    if (clickedCells[4][3] == 0) {
        checkCell(4, 3, ui->pushButton_23);
        clickedCells[4][3] = 1;
    }
}


void mines::on_pushButton_25_clicked()
{
    if (clickedCells[4][4] == 0) {
        checkCell(4, 4, ui->pushButton_25);
        clickedCells[4][4] = 1;
    }
}


void mines::on_pushButton_27_clicked()
{
    hide();
    MainWindow *hub = new MainWindow(curBalance, this);
    hub->show();
}


void mines::on_pushButton_28_clicked()
{
    minesNumber = 1;
}


void mines::on_pushButton_29_clicked()
{
    minesNumber = 3;
}


void mines::on_pushButton_30_clicked()
{
    minesNumber = 5;
}


void mines::on_pushButton_31_clicked()
{
    cellsOpened = 0;
    winProbability = 1;
    fieldReveal();
    ui->pushButton_31->hide();
    showMinesNumberChoice();
    ui->label_6->hide();
    curBalance += multiplier * 1000;
    ui->label->setText(QString::number(curBalance));
}

