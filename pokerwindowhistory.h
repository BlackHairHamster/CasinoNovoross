#ifndef POKERWINDOWHISTORY_H
#define POKERWINDOWHISTORY_H

#include <QDialog>
#include <QSortFilterProxyModel>


namespace Ui {
class pokerwindowhistory;
}

class pokerwindowhistory : public QDialog
{
    Q_OBJECT

public:
    explicit pokerwindowhistory(QWidget *parent = nullptr, QString userName = "sus", QString login = "sus");
    QSortFilterProxyModel *proxyModel;
    ~pokerwindowhistory();

private slots:
    void on_filterLineEdit_textChanged(const QString &text);


private:
    Ui::pokerwindowhistory *ui;
    QString userName;
    QString login;
};

#endif // POKERWINDOWHISTORY_H
