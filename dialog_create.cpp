#include "dialog_create.h"
#include "ui_dialog_create.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

Dialog_create::Dialog_create(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_create)
{
    ui->setupUi(this);
    ui->date->setDate(QDate::currentDate());        //встановлення в календарі сьогоднішньої дати
}

Dialog_create::Dialog_create(QString path, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_create)
{
     path_file = path;
     ui->setupUi(this);
     ui->date->setDate(QDate::currentDate());        //встановлення в календарі сьогоднішньої дати
    //QMessageBox::information(0,"a", path);
}
QString path_file;
Dialog_create::~Dialog_create()
{
    delete ui;
}

void Dialog_create::on_button_accepted()
{
    if (ui->text_route->toPlainText() == "")
    {
        QMessageBox::information(0,"Error", "Ви не додати населені пункти");
    } else {
        QString time1 = ui->start_time->text().length() != 4 ? ui->start_time->text() : "0" + ui->start_time->text(),
            time2 = ui->end_time->text().length() != 4 ? ui->end_time->text() : "0" + ui->end_time->text();
        QString str = "\n" + ui->date->date().toString("dd.MM.yyyy") + " " +
                time1 + " " + time2 + " " +
                ui->text_route->toPlainText().mid(0,ui->text_route->toPlainText().length() -1);
        QFile file(path_file);
        if (file.open(QIODevice::Append)) {
            QTextStream stream(&file);
            stream << str;
            file.close();
            QMessageBox::information(0,"info", "Маршрут успішно додано");
        } else QMessageBox::information(0,"Error", "Файл не вдалось відкрити");

    }
}

void Dialog_create::on_pushButton_clicked()
{
    QString text = ui->text_route->toPlainText();
    QString str = ui->add_city->displayText();
    ui->text_route->setText(text + str+ ">");
    ui->add_city->setText("");
}
