#ifndef LOGINIZATION_H
#define LOGINIZATION_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlDriver>


namespace Ui {
class loginization;
}

class loginization : public QDialog
{
    Q_OBJECT

public:
    explicit loginization(QWidget *parent = nullptr);
    ~loginization();

private slots:
    void on_signInLoginButton_clicked();
    void on_radioButton_clicked(bool checked);
    void on_signupLoginButton_clicked();

private:
    Ui::loginization *ui;
    QSqlDatabase database;
};

#endif // LOGINIZATION_H
