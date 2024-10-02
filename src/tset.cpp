// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
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
    if (Elem > MaxPower - 1 || Elem < 0) {
        return 0;
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem > MaxPower - 1 || Elem < 0) {
        throw out_of_range("out of U");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem > MaxPower - 1 || Elem < 0) {
        throw out_of_range("out of U");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower != s.MaxPower) {
        return 0;
    }
    if (BitField != s.BitField) {
        return 0;
    }
    return 1;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet a(BitField | s.BitField);
    return a;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || MaxPower <= Elem) {
        throw out_of_range("out of U");
    }
    TSet b = *this;
    b.InsElem(Elem);
    return b;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || MaxPower <= Elem) {
        throw out_of_range("out of U");
    }
    TSet c = *this;
    c.DelElem(Elem);
    return c;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet d(BitField & s.BitField);
    return d;
}

TSet TSet::operator~(void) // дополнение
{
    TSet f(~BitField);
    return f;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int k;
    istr >> k;
    int v;
    for (int i = 0; i < k; i++) {
        istr >> v;
        s.InsElem(v);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int l = 0;
    string r = "";
    for (int i = 0; i < s.MaxPower; i ++ ) {
        if (s.IsMember(i) == 1) {
            l++;
            r += to_string(i) + " ";
        }
    }
    r = to_string(l) + " " + r;
    ostr << r;
    return ostr;
}
