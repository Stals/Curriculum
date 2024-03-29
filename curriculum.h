// Учебный план должен хранить в себе список всех дисциплин и всего такого.
// При создании класса Учебный план - ему передается имя файла с учебным планом после чего он сам загружает его себе в память. 
// Таких учебный планов может быть 5. Или же если они равны можно сделать 1 учебный план и 4 ссылки на него. (Ну или ссылки только в тех которые его копитастят)
// Но только если я буду хранить Учебный план сразу для всех курсов.

#ifndef CURRICULUM_H
#define CURRICULUM_H
#include <vector>
#include <utility>

#include "curriculumfile.h"
#include "curriculumdb.h"
#include "schedule.h"
#include "cycle.h"
#include "subject.h"
#include "subsubjects.h"

class Curriculum{ //TODO ASAP - если я храню только для одного курса- сюда нужно передавать № курса для которого будут доставаться предметы.
public:
    Curriculum(int year, std::wstring filename = L"");
    ~Curriculum();

    // Хранит график учебного процесса этого курса
    Schedule schedule;
    // Хранит список циклов из учебного плана (Которые в свою очередь хранят список дисциплин в них входящий).
    std::vector<Cycle> cycles; //TODOmb поменять на private, если будет необходимость сделать специфичные методы доступа. (типа получить список предметов только для определенного цикла или получить список циклов. и тп.)
private:
    struct RowType {
        enum Value {
            cycle,				// цикл предметов
            subject, 			// дисциплина
            subSubjectNumber, 	// номер дисциплины по выбору (например ГСЭ.В1)
            subSubject,         // дисциплина по выбору
            unknown 			// не одна из выше перечисленных
        };
    };

    int year; // № курса
    std::wstring filename; // имя файла хранящего учебный план
    CurriculumFile* xls; // TODO! освобождать память после того как получу список дисциплин со второго листа и то что нужно с первого (создавать его через new и потом вызвать delete)
    CurriculumDB* db;

	// TODO Question - Хранить сразу для всех курсов или только для 1ого? - мне кажется для 1ого хранить понятнее (и не будет путаницы)
	// TODO Question - как хранить subjects? можно сделать map с ключами - циклами, а value - 2 контейнера (для обязательных и дополнительных дисциплин)

    //Загружает график учебного процесса из учебного плана "Лист 1"
    void loadScheduleFromFile();
    //Загружает дисциплины из учебного плана "Лист 2"
    void loadSubjectsFromFile();

    // Загружает график учебного процесса из базы
    void loadScheduleFromDB();
    // Загружает дисциплины из базы
    void loadSubjectsFromDB();

	// Возращет предмет со строки row
	// При добавлении дисциплины по выбору нужно передать её номер 
    Subject getSubject(int row, std::wstring titleNumber = L"");

    CycleName getCycleName(int row);

    std::wstring getSubSubjectNumber(int row);

    std::pair<double, double> getSubSubjectPercent(int row);

	// Возвращает RowType в зависимости от того чем является строка
    RowType::Value getRowType(int row);

    // Проверяет входит ли semesterNum в список тех семестров для которых есть аттестация в строке listOfSemesters
    bool isSemesterInList(int year, int semesterNum, std::string listOfSemesters);

    void removeEmptySubSubjects();
    // Удаляет пусты циклы из cycles
    void removeEmptyCycles();
    //TODO Тут еще нужен метод который будет получать то что нужно с первого листа.

	//TODO! Можно сделать метод который будет добавлять все под дисциплины после указанной строки,
	// пока не попадется строка которая не является поддисциплиной. 
	// Причем это метод возвращает номер строки на которой закончились поддисциплины - тоесть дальше нужно смотреть от этого места
	// и При всем этом запихивать эти под дисциплины в переданных ему контейнер. (нужно определиться с тем как я храню для курса дисциплины)


    };


#endif
