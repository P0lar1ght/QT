//
// Created by 86156 on 2023/4/8.
//

#ifndef QT_GETDIALOG_H
#define QT_GETDIALOG_H

#include "ui_GetDialog.h"

namespace Ui {
    class GetDialog;
}
class GetDialog : public QDialog {
Q_OBJECT
public:
    explicit GetDialog(QWidget *parent = nullptr);

    ~GetDialog() override;
    QLineEdit* getLineEdit_3();
    QLineEdit* getLineEdit_2();

private:
    Ui::GetDialog *ui;
};


#endif //QT_GETDIALOG_H
