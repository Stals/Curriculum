#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "curriculum.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// И при этом храним sheet внутри класса (он созддается в конструкторе)
// Вообщем мне нужнен класс который будет общаться с excelFormat
// и 2ой который мне в программу просто выдаст готовый список занятий


// В диплом: храним  в wstring, чтобы потом вывести на форму. Почему нельзя в std::string? потому что станет ASCII, а нам нужнен unicode

void MainWindow::on_pushButton_clicked()
{    
    Curriculum curriculum(1, L"название.xls");
    QString subjects = "";
    for(std::vector<Cycle>::iterator it = curriculum.cycles.begin(); it!= curriculum.cycles.end(); ++it){
        for(int i = 0; i < it->subjects.size(); ++i)
            subjects = subjects + QString::fromStdWString(it->subjects[i].title)+"\n";
    }
    ui->label->setText(subjects);
    //TODO Определять факультатив, а его предметы как обычные предметы

    //TODO как сделать хранение - в заметках на ipad.
    //TODO перенести текст отсуда куда нада (в диплом часть можно)
    //TODO нарисовать архитектуру для диплома (и нада будет описать наверно)

//    CurriculumFile curriculumFile(L"название.xls");

//    std::wstring cellText = curriculumFile.getCellString(1,6,0);
//    ui->label->setText(QString::fromStdWString(cellText));

    //int cellInt = currciculumFile.getCellInt(1,6,0);
    //ui->label->setText(QString::number(cellInt));

}

/*
Класс хранит sheet'ы и предоставляет методы получения wstring и int оттуда по row и col.
В конструктор класса мы передаем название файла который нужно открыть.
После чего мы используя этот класс заполняем контейнер дисциплин
~ как?
    читаем ++row col = 2, и если там не пусто тогда это предмет.
    Но если там есть текст - и слева не число больше 10.
    Если слева текст меньше 10 - это доп предмет. А если больше - это начало следущего цикла.

*/
