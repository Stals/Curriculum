#ifndef CYCLE_H
#define CYCLE_H
#include <string>

#include "subject.h"
struct CycleName{
	std::wstring shortName;
	std::wstring fullName;
};

// Хранит предметы и необязательные предметы цикла
struct Cycle{
	CycleName name;
	std::vector<Subject> subjects; // Обязательные дисциплины
	std::vector<Subject> subSubjects; // Дисциплины по выбору
	
	Cycle(){}
	Cycle(CycleName cycleName): name(cycleName){}
};
#endif
