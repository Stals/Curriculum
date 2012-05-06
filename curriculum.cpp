#include "curriculum.h"

Curriculum::Curriculum(wchar_t* filename){
	xls.Load(filename);
    sheet0 = xls.GetWorksheet(0);
    sheet1 = xls.GetWorksheet(1);
}


Curriculum::~Curriculum(){

}

std::wstring Curriculum::getCellString(int sheetNum, int row, int col){
	BasicExcelWorksheet* selectedSheet = (sheetNum == 0)? sheet0 : sheet1; 
	BasicExcelCell* cell = selectedSheet->Cell(row, col);
    const wchar_t* cellText = cell->GetWString();

    if(cellText == NULL)
        return L""; // return empty string
    else
        return cellText; // convertint to wstring

}


int Curriculum::getCellInt(int sheetNum, int row, int col){
	BasicExcelWorksheet* selectedSheet = (sheetNum == 0)? sheet0 : sheet1; 
	BasicExcelCell* cell = selectedSheet->Cell(row, col);
	int cellInt = cell->GetInteger();
	return cellInt;
}
