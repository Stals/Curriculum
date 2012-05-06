#include "curriculumfile.h"

CurriculumFile::CurriculumFile(wchar_t* filename){
	xls.Load(filename);
    sheet0 = xls.GetWorksheet(0);
    sheet1 = xls.GetWorksheet(1);
}


std::wstring CurriculumFile::getCellString(int sheetNum, int row, int col){
    BasicExcelWorksheet* selectedSheet = (sheetNum == 0)? sheet0 : sheet1;
    BasicExcelCell* cell = selectedSheet->Cell(row, col);
    const wchar_t* cellText = cell->GetWString();

    if(cellText == NULL)
        return L""; // return empty string
    else
        return cellText; // convertint to wstring

}


int CurriculumFile::getCellInt(int sheetNum, int row, int col){
    BasicExcelWorksheet* selectedSheet = (sheetNum == 0)? sheet0 : sheet1;
    BasicExcelCell* cell = selectedSheet->Cell(row, col);
    int cellInt = cell->GetInteger();
    return cellInt;
}

bool CurriculumFile::isEmpty(int sheetNum, int row, int col){
    bool isEmpty = (getCellString(sheetNum, row, col).empty() && (getCellInt(sheetNum, row, col) == 0) );
    if(isEmpty)
        return true;
    else
        return false;

}

