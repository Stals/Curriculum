#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>

struct WeekType{
	enum Value{
		theoretical,		 // Теоритическое обучение
		exam,				 // Экзаменационные сессии
		educationalPractice, // Учебная практика
		otherPractice,		 // Другие практики
		diplomaWork,		 // Дипломная работа или проекты
		stateExam,			 // Государственные экзамены
		holiday,			 // Каникулы
		finalExam			 // Итоговая аттестация
	};
};

class Schedule{
public:
	// Хранит 52 типов недель 
	std::vector<WeekType::Value> weekTypes; 

	WeekType::Value getWeekType(int weekNum){
		return weekTypes[weekNum];
	}
};

#endif // SCHEDULE_H
