#include "pokerwindowhistory.h"
#include "ui_pokerwindowhistory.h"
#include <QSqlTableModel>
#include <QSortFilterProxyModel>
#include <QRegularExpression>
#include <QTableView>
#include <iostream>

pokerwindowhistory::pokerwindowhistory(QWidget *parent, QString userName , QString login)
    : QDialog(parent)
    , ui(new Ui::pokerwindowhistory)
    , login(login)
    , userName(userName)
{
    ui->setupUi(this);
    QSqlDatabase dbq = QSqlDatabase::addDatabase("QSQLITE");
    dbq.setDatabaseName("/Users/capybastercarbonaster/Desktop/RegistrationDB.db");
    if (!dbq.open()) {
        std::cout << "Unable to open the database";
        return;
    }

    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("games");
    // Устанавливаем фильтр для отображения только записей этого пользователя
    model->setFilter(QString("login_of_user = '%1'").arg(login)); // Предположим что колонка с логинами называется userLogin
    model->select();

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->pokerHistoryLayout->layout());
    QTableView *tableView = new QTableView();
    layout->addWidget(tableView);

    tableView->setModel(proxyModel);
    tableView->setSortingEnabled(true);
    tableView->resizeColumnsToContents();
    tableView->hideColumn(0); // Скрываем колонку с логинами в GUI, но она остается в модели для фильтрации данных

    tableView->show();
    connect(ui->filterLineEdit, &QLineEdit::textChanged, this, &pokerwindowhistory::on_filterLineEdit_textChanged);
}

pokerwindowhistory::~pokerwindowhistory() {
    delete ui;
    delete proxyModel;
}

void pokerwindowhistory::on_filterLineEdit_textChanged(const QString &text) {
    // Теперь нам не нужно искать по логину, поэтому фильтруем данные исключительно по введеному тексту
    // Примечание: здесь предполагается, что фильтрация идет по второй колонке (например, по имени пользователя, если таковая есть)
    // Вам может потребоваться изменить индекс столбца (например, на 1, если требуется фильтровать по первой видимой колонке)
    proxyModel->setFilterKeyColumn(1); // Фильтрация по второй колонке
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterRegularExpression(QRegularExpression::escape(text));
}
