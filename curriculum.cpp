#include "curriculum.h"

Curriculum::Curriculum(int year, wchar_t* filename): xls(filename){
	this->year = year;
	// Заполняем subjects содержимым xls
    loadCurriculumSubjects();
}

//Загружает дисциплины из учебного плана "Лист 2"
void Curriculum::loadCurriculumSubjects(){
    CycleName currentCycleName; // имя текущего цикла
    std::wstring currentSubSubjectNumber; // номер текущих дисциплин по выбору
	
    // Для каждой строки в файле - определим какой тип она имеет и выполним соответствующие дейстия
    int startFrom = 2; // Чтобы строка "|№|Название дисциплины|" - не считалась дисциплиной
    for(int row = startFrom; row < xls.getTotalRows(1); ++row){

        RowType::Value rowType = getRowType(row);
        switch ( rowType ) {

            case RowType::cycle:{
                // Получим имя нового цикла и добавим его в cycles
                std::cout<<row+1<<" cycle"<<std::endl;
                currentCycleName = getCycleName(row);
                Cycle cycle(currentCycleName);
                cycles.push_back(cycle);
                break;
            }
            case RowType::subject:
                // Добавим новый предмет в список обязательных дисциплин текущего цикла
                std::cout<<row+1<<" subject"<<std::endl;
                cycles.back().subjects.push_back( getSubject(row) );
                break;

            case RowType::subSubjectNumber:
                // Получим имя новых дисциплин по выбору
                std::cout<<row+1<<" subSubjectNumber"<<std::endl;
                currentSubSubjectNumber = getSubSubjectNumber(row);
                break;

            case RowType::subSubject: 
                // Добавим новый предмет по выбору в список дисциплин по выбору текущего цикла
                std::cout<<row+1<<" subSubject"<<std::endl;
                cycles.back().subSubjects.push_back( getSubject(row, currentSubSubjectNumber) );
                break;

            case RowType::unknown:
                std::cout<<row+1<<" unknown"<<std::endl;
                break;
        }
    }
}


// Добавляет предмет со строки row в subjects
Subject Curriculum::getSubject(int row, std::wstring titleNumber){

// Нужно сделать список с цифравми столбцов где находится каждое число.
// Предметы должны доставаться только если они для этого курса.	

// Берем только информацию из тех семестров которые входят в наш курс (year).
    Subject subject;

    subject.year = year;

    if(titleNumber.empty())
        // значит это не дополнительная дисциплина - получаем номер из файла
        subject.titleNumber =  xls.getCellWString(1, row, 0);
    else
        // это дополнительная дисциплина - получим номер из titleNumber
        subject.titleNumber = titleNumber;     
    
    subject.title = xls.getCellWString(1, row, 1);    
    subject.totalHours = xls.getCellInt(1,row, 5);
    subject.totalAcademicHours = xls.getCellInt(1,row, 7);
    subject.totalIndependentWorkHours = xls.getCellInt(1,row, 8);
    

    // firstCol - Номер первого столбца для этого курса, 
    // где 9 - номер столбца для первого курса, а 8 - кол-во столбцов между курсами
    int firstCol = 9 + 8 * (year - 1);

    // Получим кол-во часов
    subject.firstSemester.lectureHours =          xls.isEmpty(1, row, firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.firstSemester.labWorkHours =          xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.firstSemester.practiceHours =         xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.firstSemester.independentWorkHours =  xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);

    subject.secondSemester.lectureHours =         xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.secondSemester.labWorkHours =         xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.secondSemester.practiceHours =        xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);
    subject.secondSemester.independentWorkHours = xls.isEmpty(1, row, ++firstCol) ? 0 : xls.getCellInt(1, row, firstCol);

    // Получим информацию о наличии итоговой аттестации
    // Если номер семестра входит в список семестров - значит аттестация на этом семестре по этому предмету присутствует
    subject.firstSemester.exam =        isSemesterInList(year, 1, xls.getCellStdString(1,row, 2));
    subject.firstSemester.test =        isSemesterInList(year, 1, xls.getCellStdString(1,row, 3));
    subject.firstSemester.yearWork =    isSemesterInList(year, 1, xls.getCellStdString(1,row, 4));

    subject.secondSemester.exam =       isSemesterInList(year, 2, xls.getCellStdString(1,row, 2));
    subject.secondSemester.test =       isSemesterInList(year, 2, xls.getCellStdString(1,row, 3));
    subject.secondSemester.yearWork =   isSemesterInList(year, 2, xls.getCellStdString(1,row, 4));

    return subject;
}

//TODO - если мы получили дисциплину по выбору - то нам всеравно нужно получить её номер (например ГСЭ.В1)
//Когда я получаю cycle - нужно добавлять в новый key map'a, и наверно сделать что-то похожее если началися под дисциплины.


 CycleName Curriculum::getCycleName(int row){
    CycleName cycleName;
    cycleName.shortName = xls.getCellWString(1, row, 1);
    cycleName.fullName = xls.getCellWString(1, row, 2);
    return cycleName;
 }

 std::wstring Curriculum::getSubSubjectNumber(int row){
    std::wstring str = xls.getCellWString(1, row, 1);
    std::wstring::iterator it = str.begin();
    for(; it!= str.end(); ++it){
        if((*it) == L' ')
            break;
    }

    return std::wstring(str.begin(),it);
 }



// Возвращает RowType в зависимости от того чем является строка
/* Принчип:
    Если во 2ом столбце есть текст
        И в 1ом столбце число - значит на этой строке название цикла
        И в 1,3 и 9ом столбцах ничего нету - значит это название дисциплин по выбору (3ую проверяем чтобы не считать поддисциплинами строки "Всего", а 9 чтобы не считать "Проректор" и "Декан")
        И в 1ом cтолбце CellStdString не пустой но WString пустой - значит это дисциплина по выбору
        И в 1ом столбце не число - значит на этой строке дисциплина

    Иначе на этой строке нет того что мы ищем не то что мы ищем
*/
Curriculum::RowType::Value Curriculum::getRowType(int row){
    if(!xls.isEmpty(1, row, 1)){

        if(xls.getCellInt(1, row, 0)) // Значит это цикл предметов
            return RowType::cycle;

        else if(xls.isEmpty(1, row, 0) &&
                xls.isEmpty(1, row, 2) &&
                xls.isEmpty(1, row, 8))
            return RowType::subSubjectNumber;

        else if(xls.getCellWString(1, row, 0).empty() &&
               (!xls.getCellStdString(1, row, 0).empty()))// Значит это дисциплина по выбору
            return RowType::subSubject;

        else if((xls.getCellInt(1, row, 0) == 0) && !xls.isEmpty(1, row, 0)) //Значит предмет
            return RowType::subject;
    }
    return RowType::unknown;
}

//TODO - если мы получили дисциплину по выбору - то нам всеравно нужно получить её номер (например ГСЭ.В1)
//Когда я получаю cycle - нужно добавлять в новый key map'a, и наверно сделать что-то похожее если началися под дисциплины.

// Проверяет входит ли semesterNum в список тех семестров для которых есть аттестация в строке listOfSemesters
bool Curriculum::isSemesterInList(int year, int semesterNum, std::string listOfSemesters){
    // Получим реальное значение семестра (3 год 1 семестр = 5 семестр)
    int realSemesterInt = (semesterNum == 1)? (year * 2) - 1 : (year * 2);
    if(realSemesterInt == 10){
        //NOTE: На 10 семестре экзаменов, зачетов или курсовых быть не должно.
        return false;
    }

    char realSemesterNum = (realSemesterInt + 48); // convert int to char_t
    size_t found = listOfSemesters.find(realSemesterNum);
     if (found != string::npos)
         return true;
     else return false;

}

