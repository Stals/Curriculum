#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "curriculumfile.h"


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

//std::wstring getCellString(int row, int col){
//   BasicExcelCell* cell = sheet->Cell(row, col);
//   const wchar_t* cellText = cell->GetWString();
//   std::wstring result = cellText; //convert to wstring
//   return result;
//}

//int getCellInt(int row, int col){
//    BasicExcelCell* cell = sheet->Cell(row, col);
//    int cellInt = cell->GetInteger();
//    return cellInt;
//}

// И при этом храним sheet внутри класса (он созддается в конструкторе)
// Вообщем мне нужнен класс который будет общаться с excelFormat
// и 2ой который мне в программу просто выдаст готовый список занятий


// В диплом: храним  в wstring, чтобы потом вывести на форму. Почему нельзя в std::string? потому что станет ASCII, а нам нужнен unicode

void MainWindow::on_pushButton_clicked()
{    
    CurriculumFile curriculumFile(L"название.xls");

    std::wstring cellText = curriculumFile.getCellString(1,6,0);
    ui->label->setText(QString::fromStdWString(cellText));

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
