// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>
#include <sstream>

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) { }

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.BitField.GetLength()) { }

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength()) { }

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
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
    return;
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
    return;
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (MaxPower == s.MaxPower) & (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return (MaxPower != s.MaxPower) | (BitField != s.BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TBitField tBitField = BitField;
    tBitField.SetBit(Elem);
    return TSet(tBitField);
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TBitField tBitField = BitField;
    tBitField.ClrBit(Elem);
    return TSet(tBitField);
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    string str;
    getline(istr, str);
    istringstream elems(str);
    TELEM item;
    while (elems >> item)
    {
        s.InsElem(item);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i)) ostr << i << " ";
    }
    return ostr;
}