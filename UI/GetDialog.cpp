//
// Created by 86156 on 2023/4/8.
//

#include "GetDialog.h"

GetDialog::GetDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::GetDialog) {
    ui->setupUi(this);

}

GetDialog::~GetDialog() {
    delete ui;
}

QLineEdit *GetDialog::getLineEdit_3() {
    return ui->lineEdit_3;
}

QLineEdit *GetDialog::getLineEdit_2() {
    return ui->lineEdit_2;
}
