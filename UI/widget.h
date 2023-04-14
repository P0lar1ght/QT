//
// Created by 86156 on 2023/1/5.
//

#ifndef QT_WIDGET_H
#define QT_WIDGET_H

#include <QWidget>
#include "string"
#include "../Query/CQueryIdCardFromFile.h"
#include "../Query/CQueryPhoneFromFile.h"
#include "../Query/CQueryIPFromFile.h"
#include "../SQLPackage/SQLiteConnection.h"
QT_BEGIN_NAMESPACE
namespace Ui {
    class widget;
}
QT_END_NAMESPACE
class widget : public QWidget {
Q_OBJECT

public:
    explicit widget(QWidget *parent = nullptr);

    ~widget() override;

    void on_openDialogButton_clicked();

    bool saveTableToCsv(const QString& tableName, const QString& filePath);

    void showQueryIDCardData();

    void showQueryPhoneData();

    void showQueryIPData();

    bool check();

private:
    Ui::widget *ui;
    int start{};
    int end{};
    std::string Table;
    std::string Column;
    CQueryIPFromFile *ipFromFile = new CQueryIPFromFile();
    CQueryIdCardFromFile *idCardFromFile = new CQueryIdCardFromFile();
    CQueryPhoneFromFile *phoneFromFile = new CQueryPhoneFromFile();
    SQLiteConnection *DB = new SQLiteConnection();

    std::string id_card_string{};
    std::string ip_string{};
    std::string mobile_phone_string{};
};

#endif //QT_WIDGET_H
