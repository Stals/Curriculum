
/*Класс хранит sheet'ы и предоставляет методы получения wstring и int оттуда по row и col.
В конструктор класса мы передаем название файла который нужно открыть. 
После чего мы используя этот класс заполняем контейнер дисциплин
~ как?
    читаем ++row col = 2, и если там не пусто тогда это предмет.
    Но если там есть текст - и слева не число больше 10.    
    Если слева текст меньше 10 - это доп предмет. А если больше - это начало следущего цикла.
*/
#ifndef CURRICULUM_H
#define CURRICULUM_H
    
#include <string>
#include "ExcelFormat.h"

using namespace ExcelFormat;

class Curriculum{ // Учебный план
public:
	Curriculum(wchar_t* filename);

	// returns cell text, if there is no text - returns empty wstring
	std::wstring getCellString(int sheetNum, int row, int col);
	// returns itn from the cell else - returns 0 if there is not an int 
	int getCellInt(int sheetNum, int row, int col);

private:
	// хранит файл учебного плана
	BasicExcel xls; 
	// Хранит листы учебного плана
	BasicExcelWorksheet* sheet0;
	BasicExcelWorksheet* sheet1;
};

#endif
