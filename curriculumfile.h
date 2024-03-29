

/* Этот класс является Оберткой над ExcelFormat.
И предоставляет более простой интерфейс для получения информации из файла учебного плана.
Класс хранит sheet'ы и предоставляет методы получения wstring и int оттуда по row и col.
В конструктор класса мы передаем название файла который нужно открыть. 
*/

//NOTE: Если поле в файле содержит числа но при этом является текстовым - его нужно получать методом getCellStdString

#ifndef CURRICULUMFILE_H
#define CURRICULUMFILE_H
    
#include <string>
#include "ExcelFormat.h"

using namespace ExcelFormat;

class CurriculumFile{ // Учебный план
public:
	CurriculumFile(wchar_t* filename);

    // Возвращяет текст ячейки, если там не текст - возвращает пустую строку
    // sheetNum - номер листа в xls файле с которого нужно получить информаци
    // row - строка
    // col - столбец
	std::wstring getCellWString(int sheetNum, int row, int col);
    std::string getCellStdString(int sheetNum, int row, int col);
    // возвращает число из ячейки, если там нет числа - возвращает 0
    // sheetNum - номер листа в xls файле с которого нужно получить информаци
    // row - строка
    // col - столбец
	int getCellInt(int sheetNum, int row, int col);
    double getCellDouble(int sheetNum, int row, int col);
    bool isEmpty(int sheetNum, int row,int col);
    int getTotalRows(int sheetNum);
    size_t getStringLength(int sheetNum, int row, int col);
private:
	// хранит файл учебного плана
	BasicExcel xls; 
	// Хранит листы учебного плана
    BasicExcelWorksheet* sheet0;
    BasicExcelWorksheet* sheet1;
};

#endif
