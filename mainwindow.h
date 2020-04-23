#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QList>
#include <QDate>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



struct road {
    QString time1,time2;
    QDate date;
    QList<QString> route;
};




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_file_triggered();
    void on_info_triggered();
    void on_instruction_triggered();
    void on_search_btn_clicked();
    void on_create_file_triggered();
    void on_create_btn_clicked();
    void on_all_btn_clicked();

private:
    Ui::MainWindow *ui;
    QString path= "";
    QList<road> list; //список структур road
};
#endif // MAINWINDOW_H
