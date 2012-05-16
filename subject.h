#ifndef SUBJECT_H
#define SUBJECT_H

// unsigned short mb? до 6к значения

// Хранит кол-во часов дисциплины в семестре, а также наличие итоговой аттестации
struct Semester{
	// Кол-во часов в этом семетре
	int lectureHours; // кол-во часов лекций
	int labWorkHours; // кол-во часов лабораторных работ
	int practiceHours; // кол-во часов практики
	int independentWorkHours; // кол-во часов контролируемой самомтоятельной работы
	
	// наличие итоговой аттестации
	bool test; // наличие зачета
	bool exam; // наличие экзамена
	bool yearWork; // наличие курсовой //TODO rename

	Semester():lectureHours(0), labWorkHours(0), practiceHours(0), independentWorkHours(0),
	 test(false), exam(false), yearWork(false){}

    // Возвращает true, если в этом семестре эта дисциплина не проводистся
    bool isEmpty(){
        int totalHours = lectureHours + labWorkHours + practiceHours + independentWorkHours;
        bool attestation = exam || test || yearWork;
        // Если Общее время для это семестра = 0 и отсутствуют аттестации
        if(totalHours == 0 && !attestation)
            return true;
        else return false;
    }
};

struct Subject{
	/* Информация добавляемая во время загрузки Учебного плана */	

	// id дисциплины?
	// навание цикла // например ГСЭ (Цикл общих гуманитарных и социально-экономических дисциплин)

	int year; // № курса // не факт что нужно ...
	std::wstring titleNumber; // номер дисциплины (например ГСЭ.Ф.1)
	std::wstring title; // название дисциплины

	// Общее кол-во часов // не факт что нужно    
	int totalHours; // общее число часов
	int totalAcademicHours; // общее число академических часов
	int totalIndependentWorkHours; //общее число часов самостоятельной работы
	
	Semester firstSemester;
	Semester secondSemester;

	/* Информация добавляемая после загрузки Учебного плана */	
	bool computerClassNecessary; // необходима ли компьютерная аудитория
	bool projectorNecessary; // необходим ли проектор 
    std::wstring teacherName; // or mb id ?
    int laboriousness; // трудоемкость дисциплины

    // Возвращает true если ни в одном из семестров (на этом курсе) эта дисциплина не проводится
    bool isEmpty(){
        if(firstSemester.isEmpty() && secondSemester.isEmpty())
            return true;
        else return false;
    }
};

//Лекций	
//Лабораторных	
//Практических	
//Контролируемая самостоятельная работа



//Нужно знать именно для этого курса кол-во часов разных и на какиих семестрах будет зачеты и тп. 

//Для каждого семестра на этом курсе на котором есть эта дисциплина - 
//Нужно хранить кол-во часов Лекций, Лабораторных, Практических и самостоятельной работы,
//а также наличие зачета, экзамена или курсовой работы.

//Часть информации мы получаем из учебного плана,  а часть оставляем пустой до этапа добавления информации.

#endif
