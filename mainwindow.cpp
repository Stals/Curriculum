#include <QFileDialog>

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
    delete curriculum;
}

//converts QString to std::wstring
std::wstring MainWindow::qs2ws(const QString& str){
    return (std::wstring((wchar_t*)str.unicode(), str.length()));
}


void MainWindow::clearAllEdits(){
    ui->subjectTitle->clear();
    ui->teacherName->clear();
    ui->laboriousness->clear();
    ui->totalHours->clear();
    ui->lectureHours1->clear();
    ui->lectureHours2->clear();
    ui->labWorkHours1->clear();
    ui->labWorkHours2->clear();
    ui->practiceHours1->clear();
    ui->practiceHours2->clear();
    ui->independentWorkHours1->clear();
    ui->independentWorkHours2->clear();
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
    //TODO спросить у маши как так - зачет на 2 семестре, а знаятия на 3ем.
    //TODO после того как курс отредактировали - выбранные предметы из числа по выбору - добавляются просто в subjects
    //TODO а у факультативов удаляются все кроме выбранных (если не выбран не один- удалить это цикл)

    for(std::vector<Cycle>::iterator cyclesIt = curriculum->cycles.begin(); cyclesIt!= curriculum->cycles.end(); ++cyclesIt){
        QTreeWidgetItem *cycle = new QTreeWidgetItem(ui->treeWidget);
        QString cycleName = QString::fromStdWString(cyclesIt->name.shortName) + " - " +
                QString::fromStdWString(cyclesIt->name.fullName);
        cycle->setText(0, cycleName);

        // Добавим обязательные дисциплины
        for(size_t i = 0; i < cyclesIt->subjects.size(); ++i){
            QTreeWidgetItem *subject = new QTreeWidgetItem(cycle);
            subject->setText(0, QString::fromStdWString(cyclesIt->subjects[i].titleNumber) + " " +
                             QString::fromStdWString(cyclesIt->subjects[i].title));
            // добавим указатель на этот элемент в вектор, а id сохраним во втором столбце (для редактирования)
            this->subjects.push_back(&(cyclesIt->subjects.at(i)));
            int subjectId = this->subjects.size() - 1;
            subject->setText(1, QString::number(subjectId));

            if(cyclesIt->name.shortName == L"ФТД") // Факультативы
                subject->setCheckState(0, Qt::Unchecked);
            cycle->addChild(subject);
        }


        // Добавим дисциплины по выбору, если такие есть
        if(!cyclesIt->subSubjectsEmpty()){
            std::vector<SubSubjects>::iterator subSubjectsIt = cyclesIt->subSubjects.begin();
            for(; subSubjectsIt!= cyclesIt->subSubjects.end(); ++subSubjectsIt){
                // Выведем название этой группы дисциплин по выбору
                QTreeWidgetItem *subSubjects = new QTreeWidgetItem(cycle);
                subSubjects->setText(0, QString::fromStdWString(subSubjectsIt->subjectsNumber));

                // Выведем первую дисциплину по выбору
                QTreeWidgetItem *subSubject1 = new QTreeWidgetItem(subSubjects);
                subSubject1->setText(0, QString::fromStdWString(subSubjectsIt->subjects[0].title));
                subSubject1->setCheckState(0, Qt::Unchecked);

                // добавим указатель на этот элемент в вектор, а id сохраним во втором столбце (для редактирования)
                this->subjects.push_back(&(subSubjectsIt->subjects.at(0)));
                int subjectId = this->subjects.size() - 1;
                subSubject1->setText(1, QString::number(subjectId));

                // Выведем вторую дисциплину по выбору
                QTreeWidgetItem *subSubject2 = new QTreeWidgetItem(subSubjects);
                subSubject2->setText(0, QString::fromStdWString(subSubjectsIt->subjects[1].title));
                subSubject2->setCheckState(0, Qt::Unchecked);

                // добавим указатель на этот элемент в вектор, а id сохраним во втором столбце (для редактирования)
                this->subjects.push_back(&(subSubjectsIt->subjects.at(1)));
                subjectId = this->subjects.size() - 1;
                subSubject2->setText(1, QString::number(subjectId));

            }
        }
    }
}

    // TODO не забыть сделать ui->treeWidget->clear(); при переходе на другой этап

    //TODO нарисовать архитектуру для диплома (и нада будет описать наверно)


// Для того чтобы была возможность редактировать - при добавлении элементов в treeWidget
// я также добавляю сыылки на них в контейнер, чтобы просто обращаться по id загруженного предмета




// И при этом храним sheet внутри класса (он созддается в конструкторе)
// Вообщем мне нужнен класс который будет общаться с excelFormat
// и 2ой который мне в программу просто выдаст готовый список занятий


// В диплом: храним  в wstring, чтобы потом вывести на форму. Почему нельзя в std::string? потому что станет ASCII, а нам нужнен unicode

void MainWindow::on_pushButton_clicked()
{
    // TODO если treewidget не пустой, сохранить все в базу.

    // TODO если в базе есть что-то загружать оттуда иначе дать выбор файла.

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


    filename = QFileDialog::getOpenFileName(this,
        tr("Open Curriculum"), "", tr("Excel Files (*.xls *.xlsx)"));

    if(!firstTime)
        delete curriculum;
    curriculum = new Curriculum(year, filename.toStdWString());
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
    ui->laboriousness->setText(QString::number(subject->laboriousness));
    ui->totalHours->setText(QString::number(subject->totalHours));
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
    subject->laboriousness = ui->laboriousness->text().toInt();
    subject->totalHours = ui->totalHours->text().toInt();
    subject->computerClassNecessary = ui->computerClassNecessary->isChecked();
    subject->projectorNecessary = ui->projectorNecessary->isChecked();

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
