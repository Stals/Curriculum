#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    // Удалим все элементы
    ui->treeWidget->clear();
    this->subjects.clear();
}

void MainWindow::addSubjectsToTree(int year){
    //TODO Номер для дисциплин - может столбцы заюзать?

    //TODO еще одно под меню для дисциплин по выбору
    // может сделать так чтобы в таблице хранилась ссылка на дисциплину которую какраз можно модифицировать? в 1ом столбце?
   // можно сделать массив указателей на эти элементы. И давать в другой столбец id в этом массиве.
    //Отправить абатурову наверно.
    //TODO добавить возможность редактирования свойств предметов

    Curriculum curriculum(year, L"название.xls");
    for(std::vector<Cycle>::iterator it = curriculum.cycles.begin(); it!= curriculum.cycles.end(); ++it){
        QTreeWidgetItem *cycle = new QTreeWidgetItem(ui->treeWidget);
        QString cycleName = QString::fromStdWString(it->name.shortName) + " - " +
                QString::fromStdWString(it->name.fullName);
        cycle->setText(0, cycleName);

        // Добавим обязательные дисциплины
        for(size_t i = 0; i < it->subjects.size(); ++i){
            QTreeWidgetItem *subject = new QTreeWidgetItem(cycle);
            subject->setText(0, QString::fromStdWString(it->subjects[i].titleNumber) + " " +
                             QString::fromStdWString(it->subjects[i].title));
            // добавим указатель на этот элемент в вектор, а id сохраним во втором столбце (для редактирования)
            this->subjects.push_back(&(it->subjects.at(i)));
            int subjectId = this->subjects.size() - 1;
            subject->setText(1, QString::number(subjectId));

            if(it->name.shortName == L"ФТД") // Факультативы
                subject->setCheckState(0, Qt::Unchecked);
            cycle->addChild(subject);
        }

        // Добавим дисциплины по выбору, если такие есть
        if(!it->subSubjects.empty()){
            QTreeWidgetItem *subSubjects = new QTreeWidgetItem(cycle);
            subSubjects->setText(0, QString::fromUtf8("Дисциплины по выбору"));
            for(size_t i = 0; i < it->subSubjects.size(); ++i){
                QTreeWidgetItem *subSubject = new QTreeWidgetItem(subSubjects);
                subSubject->setText(0, QString::fromStdWString(it->subSubjects[i].titleNumber) + " " +
                                 QString::fromStdWString(it->subSubjects[i].title));
                // добавим указатель на этот элемент в вектор, а id сохраним во втором столбце (для редактирования)
                this->subjects.push_back(&(it->subSubjects.at(i)));
                int subjectId = this->subjects.size() - 1;
                subSubject->setText(1, QString::number(subjectId));

                subSubject->setCheckState(0, Qt::Unchecked);
                subSubjects->addChild(subSubject);
            }
        }
    }
    // TODO не забыть сделать ui->treeWidget->clear(); при переходе на другой этап

    //TODO нарисовать архитектуру для диплома (и нада будет описать наверно)
}

// Для того чтобы была возможность редактировать - при добавлении элементов в treeWidget
// я также добавляю сыылки на них в контейнер, чтобы просто обращаться по id загруженного предмета




// И при этом храним sheet внутри класса (он созддается в конструкторе)
// Вообщем мне нужнен класс который будет общаться с excelFormat
// и 2ой который мне в программу просто выдаст готовый список занятий


// В диплом: храним  в wstring, чтобы потом вывести на форму. Почему нельзя в std::string? потому что станет ASCII, а нам нужнен unicode

void MainWindow::on_pushButton_clicked()
{
    ui->treeWidget->clear();
    this->subjects.clear();

    int year; //# курса
    if(ui->radioButton_1->isChecked()){
        year = 1;
    }
    if(ui->radioButton_2->isChecked()){
        year = 2;
    }
    if(ui->radioButton_3->isChecked()){
        year = 3;
    }
    if(ui->radioButton_4->isChecked()){
        year = 4;
    }
    if(ui->radioButton_5->isChecked()){
        year = 5;
    }
    addSubjectsToTree(year);
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
