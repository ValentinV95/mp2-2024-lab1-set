// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    TBitField bf(BitField);
    return bf;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem > MaxPower || Elem < 0) {
        throw out_of_range("elem is out of universe");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem > MaxPower || Elem < 0) {
        throw out_of_range("elem is out of universe");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem > MaxPower || Elem < 0) {
        throw out_of_range("elem is out of universe");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (&s == this) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(BitField == s.BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int maxmp = std::max(MaxPower, s.MaxPower);
    TSet tmp(maxmp);
    for (int i = 0; i < MaxPower; i++) {
        if (this->IsMember(i) != 0) {
            tmp.InsElem(i);
        }
    }
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i) != 0) {
            tmp.InsElem(i);
        }
    }
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(MaxPower);
    tmp.BitField = BitField;
    tmp.InsElem(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(BitField);
    tmp.DelElem(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int minmp = std::min(MaxPower, s.MaxPower);
    int maxmp = std::max(MaxPower, s.MaxPower);
    TSet tmp(maxmp);
    for (int i = 0; i < minmp; i++) {
        if ((this->IsMember(i) != 0) && (s.IsMember(i) != 0)) {
            tmp.InsElem(i);
        }
    }
    return tmp;
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(MaxPower);
    for (int i = 0; i < MaxPower; i++) {
        if (this->IsMember(i) != 0) {
            tmp.DelElem(i);
        }
        else {
            tmp.InsElem(i);
        }
    }
    return tmp;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    cout << "To stop entering the elements of the set with the last number, enter any negative" << endl;
    for (int i = 0; i < s.GetMaxPower(); i++) {
        int t;
        int fl = 1;

        do {
            istr >> t;
            if (t >= s.GetMaxPower()) {
                cout << "Invalid input. Try again" << endl;

            }
            else fl = 0;
        } while (fl);


        if (t >= 0) {
            s.InsElem(t);
        }
        else {
            break;
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    char ch = '{';
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << ch << ' ' << i;
            ch = ',';
        }

    }
    ostr << '}';
    return ostr;
}
