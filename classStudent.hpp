#pragma once
#include "classMenu.hpp"
#include "classDB.hpp"
#include "classStudentChange.hpp"
#include "classMassive.hpp"
#include "classMap.hpp"
#include "classBirthDate.hpp"
#include <limits>
#include "converter.hpp"

class Student
{
private:
    DB_Students *db;

protected:
    bool isDigit(const std::wstring& str)
    {
        if (str.empty())
            return false;
        for (wint_t ch : str)
        {
            if (!iswdigit(ch))
                return false;
        }
        return true;
    }

    bool isAlpha(std::wstring const &str)
    {
        if (str.empty())
            return false;
        for (const auto& c : str)
        {
            if (!iswalpha(c))
                return false;
        }
        return true;
    }

    bool checkSex(std::wstring const &str)
    {
        if (str.empty())
            return false;
        for (const auto& c : str)
        {
            if (!iswalpha(c) || str.length() != 1 || (c != L'М' && c != L'Ж'))
                return false;
        }
        return true;
    }

public:
    Student(DB_Students *_db)
    {
        db = _db;
    };

    void listStudentsRun()
    {
        Menu menu;
        int opt;
        Massive<IntStr> listOfStudents;
        Massive<IntInt> listMenu;
        int count;

        while (true)
        {
            count = 1;
            menu.clear();
            listOfStudents = db->getstudentListFIO();
            if (listOfStudents.size() == 0)
            {
                std::wcout << L"База студентов пустая. Выполните загрузку или введите нового студента." << std::endl;
                std::wcout << L"Нажмите любую клавишу для продолжения...";
                _getch();
                return;
            }

            menu.addTitle("Список студентов");
            menu.addOption("Вернуться в предыдущее меню");
            listMenu.pushBack({0, 0}); // для коррекции возврата выбранного пункта

            for (auto element : listOfStudents)
            {
                menu.addOption(element.second);
                listMenu.pushBack({count, element.first});
                count++;
            }

            opt = menu.run();
            if (opt == 0)
                return;
            {
                StudentChange student(db, listMenu[opt].second);
                student.run();
            }
        }
    }

    void newStudent()
    {
        std::wstring name;
        std::wstring surName;
        std::wstring middleName;
        class birthDate birthDate;
        std::wstring sex;
        std::wstring startYear;
        std::wstring departament;
        std::wstring facultet;
        std::wstring group;
        std::wstring numDoc;

        system("cls");
        std::wcout << L"Ввод данных нового студента:" << std::endl;

        while (true) 
        {
            std::wcout << L"Фамилия: ";
            std::getline(std::wcin, surName);
            // std::wcin.ignore();
            if (!isAlpha(surName))
            {
                std::wcout << L"Некорректная фамилия. Фамилия должна состоять только из букв!" << std::endl;
            }
            else
                break;
        }

        while (true)
        {
            std::wcout << L"Имя: ";
            std::getline(std::wcin, name);
            // std::wcin.ignore();
            if (!isAlpha(name))
            {
                std::wcout << L"Некорректное имя. Имя должно состоять только из букв!" << std::endl;
            }
            else
                break;
        }

        while (true)
        {
            std::wcout << L"Отчество: ";
            std::getline(std::wcin, middleName);
            // std::wcin.ignore();
            if (!isAlpha(middleName))
            {
                std::wcout << L"Некорректное отчество. Отчество должно состоять только из букв!" << std::endl;
            }
            else
                break;
        }

        while (true)
        {
            class birthDate temp;
            std::wstring date;
            std::wstringstream ss;
            unsigned short _day, _month, _year;
            std::wcout << L"Введите дату рождения через пробел (день месяц год): ";
            std::getline(std::wcin, date);
            // std::wcin.ignore();
            ss.str(date);
            ss >> _day >> _month >> _year;
            if (_day != 0 && _month != 0 && _year != 0)
            {
                if (!temp.is_valid(_day, _month, _year))
                {
                    std::wcout << L"Дата рождения введена не корректно!" << std::endl;
                }
                else
                {
                    birthDate.setDate(_day, _month, _year);
                    break;
                }
            }
            else
            {
                std::wcout << L"Дата рождения введена не корректно!" << std::endl;
            }
        }

        while (true)
        {
            std::wcout << L"Пол: ";
            std::getline(std::wcin, sex);
            // std::wcin.ignore();
            if (!checkSex(sex))
            {
                std::wcout << L"Некорректный пол. Пол может быть 'М' или 'Ж'!" << std::endl;
            }
            else
                break;
        }
        while (true) {
            std::wcout << L"Год поступления: ";
            std::getline(std::wcin, startYear);
            // std::wcin.ignore();
            if (isDigit(startYear)) {
                std::string s = wstrtostr(startYear);
                int dat = std::atoi(s.c_str());
                if (dat > 1900 && dat <= 9999)
                    break;
            }
            std::wcout << L"Год должен быть цифрой в дипозоне 1900..9999" << std::endl;
        }
        std::wcout << L"Кафедра: ";
        std::getline(std::wcin, departament);
        // std::wcin.ignore();

        std::wcout << L"Институт (факультет): ";
        std::getline(std::wcin, facultet);
        // std::wcin.ignore();

        std::wcout << L"Группа: ";
        std::getline(std::wcin, group);
        // std::wcin.ignore();

        std::wcout << L"Номер зачетки: ";
        std::getline(std::wcin, numDoc);
        // std::wcin.ignore();

        if (db->addStudent(wstrtostr(name),
                            wstrtostr(middleName),
                            wstrtostr(surName),
                            birthDate.getDate(),
                            wstrtostr(sex),
                            wstrtostr(startYear),
                            wstrtostr(departament),
                            wstrtostr(facultet),
                            wstrtostr(group),
                            wstrtostr(numDoc)) >= 0)
        {
            std::wcout << L"Успешно добавлен.\n Нажмите любую клавишу для продолжения...";
            _getch();
        }
        else
        {
            std::wcout << L"Нажмите любую клавишу для продолжения...";
            _getch();
        }
    }
};
