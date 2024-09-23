// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>
#include <sstream>

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField.GetLength())
{
    MaxPower = s.BitField.GetLength();
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
    if (Elem >= 0 && Elem < MaxPower)
    {
        return BitField.GetBit(Elem);
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem >= 0 && Elem < MaxPower)
    {
        BitField.SetBit(Elem);
    }
    else throw("element out of universe");
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= 0 && Elem < MaxPower)
    {
        BitField.ClrBit(Elem);
    }
    else throw("element out of universe");
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
    TBitField tBitField = BitField | s.BitField;
    return TSet(tBitField);
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
    TBitField tBitField = BitField & s.BitField;
    return TSet(tBitField);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField tBitField = BitField;
    tBitField = ~tBitField;
    return TSet(tBitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int MaxPower;
    istr >> MaxPower;
    TSet res(MaxPower);
    string str = "";
    getline(istr, str);
    istringstream elems(str);
    TELEM elem;
    while (elems >> elem)
    {
        res.InsElem(elem);
    }
    s = res;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << s.GetMaxPower() << '\n';
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i)) ostr << i << " ";
    }
    return ostr;
}
