#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    firstTime(true)
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

//converts QString to std::wstring
std::wstring MainWindow::qs2ws(const QString& str){
    return (std::wstring((wchar_t*)str.unicode(), str.length()));
}


void MainWindow::clearAllEdits(){
    ui->subjectTitle->clear();
    ui->teacherName->clear();
    ui->lectureHours1->clear();
    ui->lectureHours1->clear();
    ui->labWorkHours1->clear();
    ui->labWorkHours1->clear();
    ui->practiceHours1->clear();
    ui->practiceHours1->clear();
    ui->independentWorkHours1->clear();
    ui->independentWorkHours1->clear();
    ui->exam1->setChecked(false);
    ui->exam2->setChecked(false);
    ui->test1->setChecked(false);
    ui->test2->setChecked(false);
    ui->yearWork1->setChecked(false);
    ui->yearWork2->setChecked(false);
    ui->computerClassNecessary->setChecked(false);
    ui->projectorNecessary->setChecked(false);
}

void MainWindow::addSubjectsToTree(int year){
    //TODO Номер для дисциплин - может столбцы заюзать?
    //TODO fix булевые комп аудитории и тд

    //TODO еще одно под меню для дисциплин по выбору
    // может сделать так чтобы в таблице хранилась ссылка на дисциплину которую какраз можно модифицировать? в 1ом столбце?
   // можно сделать массив указателей на эти элементы. И давать в другой столбец id в этом массиве.
    //Отправить абатурову наверно.
    //TODO добавить возможность редактирования свойств предметов

    for(std::vector<Cycle>::iterator it = curriculum->cycles.begin(); it!= curriculum->cycles.end(); ++it){
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
    clearAllEdits();


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


    if(!firstTime)
        delete curriculum;
    curriculum = new Curriculum(year, L"название.xls");
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

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //Нужна проверка на то что это не цикл - нужны именно те поля у которых 2 колонки.
    int subjectId = (item->text(1)).toInt(); // get subject id
    //ui->subjectTitle->setText(item->text(1));

    // convert to int
    Subject* subject = this->subjects[subjectId];
    // Выведем общую информацию
    ui->subjectTitle->setText(QString::fromStdWString(subject->title));
    ui->subjectTitle->home(false); // чтобы название предмета было видно с начала, если оно длинное
    ui->teacherName->setText(QString::fromStdWString(subject->teacherName));
    ui->computerClassNecessary->setChecked(subject->computerClassNecessary);
    ui->projectorNecessary->setChecked(subject->projectorNecessary);

    // Выведем информацию о первом семестре
    ui->lectureHours1->setText(QString::number(subject->firstSemester.lectureHours));
    ui->labWorkHours1->setText(QString::number(subject->firstSemester.labWorkHours));
    ui->practiceHours1->setText(QString::number(subject->firstSemester.practiceHours));
    ui->independentWorkHours1->setText(QString::number(subject->firstSemester.independentWorkHours));
    ui->exam1->setChecked(subject->firstSemester.exam);
    ui->test1->setChecked(subject->firstSemester.test);
    ui->yearWork1->setChecked(subject->firstSemester.yearWork);

    // Выведем информацию о втором семестре
    ui->lectureHours2->setText(QString::number(subject->secondSemester.lectureHours));
    ui->labWorkHours2->setText(QString::number(subject->secondSemester.labWorkHours));
    ui->practiceHours2->setText(QString::number(subject->secondSemester.practiceHours));
    ui->independentWorkHours2->setText(QString::number(subject->secondSemester.independentWorkHours));
    ui->exam2->setChecked(subject->secondSemester.exam);
    ui->test2->setChecked(subject->secondSemester.test);
    ui->yearWork2->setChecked(subject->secondSemester.yearWork);

}

void MainWindow::on_pushButton_2_clicked()
{
    // Получим выбранный элемент в treeWidget
    QTreeWidgetItem *selectedItem = ui->treeWidget->selectedItems()[0];
    //и в него запишем все данные.
    int subjectId = (selectedItem->text(1)).toInt(); // get subject id
    Subject* subject = this->subjects[subjectId];

    //Сохраним общую информацию
    subject->title = qs2ws(ui->subjectTitle->text());
    subject->teacherName = qs2ws(ui->teacherName->text());
    subject->computerClassNecessary = ui->computerClassNecessary->isChecked();
    subject->projectorNecessary = ui->computerClassNecessary->isChecked();

    //Сохраним изменения для первого семестра
    subject->firstSemester.lectureHours = ui->lectureHours1->text().toInt();
    subject->firstSemester.labWorkHours = ui->labWorkHours1->text().toInt();
    subject->firstSemester.practiceHours = ui->practiceHours1->text().toInt();
    subject->firstSemester.independentWorkHours = ui->independentWorkHours1->text().toInt();
    subject->firstSemester.exam = ui->exam1->isChecked();
    subject->firstSemester.test = ui->test1->isChecked();
    subject->firstSemester.yearWork = ui->yearWork1->isChecked();

    //Сохраним измения для второго семестра
    subject->secondSemester.lectureHours = ui->lectureHours2->text().toInt();
    subject->secondSemester.labWorkHours = ui->labWorkHours2->text().toInt();
    subject->secondSemester.practiceHours = ui->practiceHours2->text().toInt();
    subject->secondSemester.independentWorkHours = ui->independentWorkHours2->text().toInt();
    subject->secondSemester.exam = ui->exam2->isChecked();
    subject->secondSemester.test = ui->test2->isChecked();
    subject->secondSemester.yearWork = ui->yearWork2->isChecked();

}
