#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_create.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QStringList headers = {"Початок подорожі","Час відбуття", "Кінець подорожі", "Час прибуття" , "Маршрут"};
    ui->setupUi(this);
    ui->date->setDate(QDate::currentDate());        //встановлення в календарі сьогоднішньої дати
    ui->table->setHorizontalHeaderLabels(headers);  //встановлюєм заголовки таблиці
    ui->table->resizeColumnsToContents();           //підганяєм розміри стовпця
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //забороняєм редагування таблиці
}


MainWindow::~MainWindow()
{
    delete ui;
}


//відкрити діалог пошуку файлу
void MainWindow::on_open_file_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), ".", tr("Text files (*.txt)"));
    path = fileName;    //для відкриття діалогу(створення маршруту)

    //діалогове вікно відкривання файлу
    //E:/HeidiSQL/license.txt

    QFile file(fileName);                                           //відкриваєм файл
    if (!file.open(QIODevice::ReadOnly)){                           //якшо файл не відкрито викидаєм помилку
        QMessageBox::warning(0, "error", file.errorString());
    } else
        //перевіряєм чи список заповнений (файл відкрито)
        if (!list.empty()){
            QMessageBox::information(0,"Information", "Відкрито новий файл");
            list.clear();
        }

    QTextStream in(&file);          //читаєм із файлу
    QString str,time1, time2, s;
    QList<QString> l;
    QDate d;

    while(!in.atEnd())  //читаєм файл до кінця
    {
        str = in.readLine();    //читаєм стрічку із файлу
        if (str != ""){
            d = QDate::fromString(str.mid(0,10),"dd.MM.yyyy");
            time1 = str.mid(11,5);   //перших 5 символів записуєм у дату
            time2 = str.mid(17,5);   //наступні символи у наступну дату
            s = "";
            l.clear();
            for (int i = 23; i < str.length(); i++) //читаєм маршрут
            {
                if (str[i] == '>')
                {
                    l.push_back(s);
                    s = "";
                }else s+= str[i];

            }
            l.push_back(s);
            list.push_back({time1,time2,d, l});   //додаєм маршрут до списку всіх маршрутів
        }
    }

    file.close();
}

//відкрити діалог про розробника
void MainWindow::on_info_triggered()
{
    QMessageBox::about(0, "Info", "Програму написав для курсового проекту\n з системного програмування\nстудент групи КІ-406\nКоцюк Назар ");
}

//відкрити діалог інструкція
void MainWindow::on_instruction_triggered()
{
    QMessageBox::information(0, "Help", "- Для початку роботи із програмою потрібно створити або відкрити файл у пункті меню Файл;\n"
                                        "- Додати маршрут можна натиснувши відповідну кнопку на користувацькому інтефейсі, та заповнивши всі поля натиснути Додати;\n"

                             "Для пошуку маршруту потрібно заповнити поля:\n"
                             "\t- початок маршруту\n\t- кінець маршруту\n\t- дата\n"
                             "При знаходженні маршрутів які відповідають запиту, їх буде виведено у таблицю нижче;"
                             "При натиснені на кнопку всі маршрути буде виведено список всіх маршрутів у таблицю;");
}


//пошук по вказаних маршрутах (Кнопка пошук)
void MainWindow::on_search_btn_clicked()
{
    QString start = ui->start_line->displayText(),      //отримуєм дані із TextEdit(на вікні куда вводимо текст)
            end = ui->end_line->displayText(),
            route;

    bool find_start = false,
            found = false;
    int row_count = 0;
    if (path != ""){
        if (!list.empty())
        {
            if (start == "" || end == "")
                QMessageBox::information(0, "error", "Не введенно одне із полів");
            else {
                //задаєм початковий вигляд таблиці
                ui->table->clear();
                ui->table->setRowCount(1);  // кількість рядків
                QStringList headers = {"Початок подорожі","Час відбуття", "Кінець подорожі", "Час прибуття" , "Маршрут"};
                ui->table->setHorizontalHeaderLabels(headers);  //встановлюєм заголовки таблиці

                //Проходження по списку всіх маршрутів
                QList<road>::const_iterator i;
                for (i = list.begin(); i != list.end(); ++i)
                {
                    route = "";
                    found = false;
                    find_start = false;

                    if (!ui->date->date().toString().compare(i->date.toString())){
                        //Пошук стрічок по списку routes і порівняння із введеними start_line end_line
                        for (int j = 0; j < i->route.size();j++ )
                        {
                            if (i->route[j] != '>')
                                route += i->route[j] + " ";
                            if (i->route[j] == start)
                            {
                                find_start = true;
                            }
                            if (find_start && i->route[j] == end)
                            {
                                found = true;
                            }
                        }
                        if (found)
                        {   //Заповнюєм таблицю отриманими даними
                            ui->table->setItem(row_count, 0, new QTableWidgetItem(start));
                            ui->table->setItem(row_count, 1, new QTableWidgetItem(i->time1));
                            ui->table->setItem(row_count, 2, new QTableWidgetItem(end));
                            ui->table->setItem(row_count, 3, new QTableWidgetItem(i->time2));
                            ui->table->setItem(row_count, 4, new QTableWidgetItem(route));
                            ui->table->resizeRowsToContents();

                            row_count++;
                            ui->table->insertRow(row_count);
                        }
                    }
                }
            }
        }
        else QMessageBox::information(0,"error", "Файл без данних");
    } else QMessageBox::information(0,"error", "Файл не відкрито");
}

//створити файл
void MainWindow::on_create_file_triggered()
{
    //Відкриваємо діалог вибір папки для створення файлу
    QString path = QFileDialog::getExistingDirectory(this, tr("Create file"));
    QString filename = "Data.txt";  //тут задаєм імя файлу
    QFile file(path + filename);
    //відкриваєм файл
    if (file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(0,"info", "Файл створено: " + path + filename);
        file.close();
    }
}

void MainWindow::on_create_btn_clicked()
{
    if (path != ""){
        Dialog_create * dg = new Dialog_create(path);
        dg->show();}
    else QMessageBox::information(0,"Error", "Файл не відкрито");
}

void MainWindow::on_all_btn_clicked()
{
    ui->table->setRowCount(0);
    //Проходження по списку всіх маршрутів
    QList<road>::const_iterator i;
    for (i = list.begin(); i != list.end(); ++i)
    {
                ui->table->setRowCount(ui->table->rowCount() + 1);
                ui->table->setItem(ui->table->rowCount() - 1, 0, new QTableWidgetItem(i->route.first()));
                ui->table->setItem(ui->table->rowCount() - 1, 1, new QTableWidgetItem(i->time1));
                ui->table->setItem(ui->table->rowCount() - 1, 2, new QTableWidgetItem(i->route.last()));
                ui->table->setItem(ui->table->rowCount() - 1, 3, new QTableWidgetItem(i->time2));

                QString route = "";
                for (int j = 0; j < i->route.size();j++ )
                    if (i->route[j] != '>')
                        route += i->route[j] + " ";

                ui->table->setItem(ui->table->rowCount() - 1, 4, new QTableWidgetItem(route));
                ui->table->resizeRowsToContents();
    }
}
