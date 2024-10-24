// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include "string"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < (MaxPower) || Elem >= 0) {
        return BitField.GetBit(Elem);
    }
    throw std::out_of_range("Error of range");
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < (MaxPower) || Elem >= 0) {
        BitField.SetBit(Elem);
    }
    else {
        throw std::out_of_range("Error of range");
    }
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < (MaxPower) || Elem >= 0) {
        BitField.ClrBit(Elem);
    }
    else {
        throw std::out_of_range("Error of range");
    }
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s)
    {
        return *this;
    }
    BitField = s.BitField;
    MaxPower = s.MaxPower;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet res(BitField | s.BitField);
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || MaxPower <= Elem) {
        throw std::out_of_range("Error of range");
    }
    TSet res = *this;
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    TSet res(~BitField);
    return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int sz;
    istr >> sz;
    int Elem;
    for (int i = 0; i < s.MaxPower; i++) {
        s.DelElem(i);
    }
    for (int i = 0; i < sz; i++) {
        istr >> Elem;
        s.InsElem(Elem);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int sz = 0;
    string res = "";
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i) == 1) {
            sz++;
            res += to_string(i) + " ";
        }
    }
    res = to_string(sz) + " " + res;
    ostr << res;
    return ostr;
}
