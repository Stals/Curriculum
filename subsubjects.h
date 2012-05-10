#ifndef SUBSUBJECTS_H
#define SUBSUBJECTS_H
#include <string>
#include <utility>

#include "subject.h"

// Хранит 2 дисциплины из которых нужно выбрать, выбор,
// а также кол-во процентов часов аудиторных и самостоятельных работ)
struct SubSubjects{
    std::wstring subjectsNumber;
    int choice; // равен 1 если выбран subject_1, 2 если subject_2, иначе = 0
    double lecturePercent;
    double independentWorkPercent;
    std::vector<Subject> subjects;

    SubSubjects(std::wstring subjectsNumber, std::pair<double, double> percents):
        choice(0), lecturePercent(0), independentWorkPercent(0){
        this->subjectsNumber = subjectsNumber;
        lecturePercent = percents.first;
        independentWorkPercent = percents.second;
    }

    bool isEmpty(){
        bool result = true;
        // Если все эдементы в subSubjects будут пустые - после цикла result все еще будет равен true
        for(std::vector<Subject>::iterator it = subjects.begin(); it!= subjects.end(); ++it)
            result = result && it->isEmpty();
        return result;
    }
};


#endif // SUBSUBJECTS_H
