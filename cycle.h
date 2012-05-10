#ifndef CYCLE_H
#define CYCLE_H
#include <string>

#include "subject.h"
#include "subsubjects.h"
struct CycleName{
	std::wstring shortName;
	std::wstring fullName;
};

// Хранит предметы и необязательные предметы цикла
struct Cycle{
	CycleName name;
	std::vector<Subject> subjects; // Обязательные дисциплины
    std::vector<SubSubjects> subSubjects; // Дисциплины по выбору
	
	Cycle(){}
	Cycle(CycleName cycleName): name(cycleName){}

    bool subSubjectsEmpty(){
        bool isSubSubjectsEmpty = true;
        // Если все эдементы в subSubjects будут пустые - после цикла result все еще будет равен true
        for(std::vector<SubSubjects>::iterator it = subSubjects.begin(); it!= subSubjects.end(); ++it)
            isSubSubjectsEmpty = isSubSubjectsEmpty && it->isEmpty();
        return isSubSubjectsEmpty;
    }

    bool isEmpty(){
        if(subjects.empty() && subSubjectsEmpty())
            return true;
        else return false;
    }
};
#endif
