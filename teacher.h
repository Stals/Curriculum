#ifndef TEACHER_H
#define TEACHER_H

struct Teacher{
    //id
    std::wstring firstName; // Имя Отчество
    std::wstring lastName;  // Фамилия
    std::wstring position;  // должность
    std::wstring degree;    // ученое зваени, ученая степень
    bool availible[7][2];   // когда доступен
}

#endif // TEACHER_H
