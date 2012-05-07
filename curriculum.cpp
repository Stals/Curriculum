#include "curriculum.h"

Curriculum::Curriculum(int year, wchar_t* filename): xls(filename){
	this->year = year;
	// Заполняем subjects содержимым xls
	getSubjects();
}


// Сохраняет Все дисциплины в subjects
void Curriculum::getSubjects(){
	//Наверно тут я определяю какая строка является строкой предмета после чего говорю её прочитать в другой метод.
    int startFrom = 2; // Чтобы строка "|№|Название дисциплины|" - не считалась дисциплиной
    for(int row = startFrom; row < xls.getTotalRows(1); ++row){
        RowType::Value rowType = getRowType(row);
        switch ( rowType ) {
            case RowType::cycle:
                std::cout<<row+1<<" cycle"<<std::endl;
                break;
            case RowType::subject:
                std::cout<<row+1<<" subject"<<std::endl;
                break;
            case RowType::subSubject:
                std::cout<<row+1<<" subSubject"<<std::endl;
                break;
            case RowType::subSubjectNumber:
                std::cout<<row+1<<" subSubjectNumber"<<std::endl;
                break;
            case RowType::unknown:
                std::cout<<row+1<<" unknown"<<std::endl;
                break;
        }
    }
}


// Добавляет предмет со строки row в subjects
void Curriculum::getSubject(int row){

// Нужно сделать список с цифравми столбцов где находится каждое число.
// Предметы должны доставаться только если они для этого курса.	

}

//TODO - если мы получили дисциплину по выбору - то нам всеравно нужно получить её номер (например ГСЭ.В1)
//Когда я получаю cycle - нужно добавлять в новый key map'a, и наверно сделать что-то похожее если началися под дисциплины.
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

        else if(xls.getCellString(1, row, 0).empty() &&
               (!xls.getCellStdString(1, row, 0).empty()))// Значит это дисциплина по выбору
            return RowType::subSubject;

        else if((xls.getCellInt(1, row, 0) == 0) && !xls.isEmpty(1, row, 0)) //Значит предмет
            return RowType::subject;
    }
    return RowType::unknown;
}

//TODO - если мы получили дисциплину по выбору - то нам всеравно нужно получить её номер (например ГСЭ.В1)
//Когда я получаю cycle - нужно добавлять в новый key map'a, и наверно сделать что-то похожее если началися под дисциплины.

