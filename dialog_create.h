#ifndef DIALOG_CREATE_H
#define DIALOG_CREATE_H

#include <QDialog>

namespace Ui {
class Dialog_create;
}

class Dialog_create : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_create(QWidget *parent = nullptr);
    Dialog_create(QString path, QWidget *parent = nullptr);
    ~Dialog_create();

    QString path_file;

private slots:
    void on_button_accepted();
    void on_pushButton_clicked();

private:
    Ui::Dialog_create *ui;
};

#endif // DIALOG_CREATE_H
