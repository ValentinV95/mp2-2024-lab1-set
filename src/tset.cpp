// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)     //Проверка на размер уже встроена в BitField
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
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
    int foo;
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("Element is out of universe or negative");
    else foo = BitField.GetBit(Elem);
    return foo;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    try { BitField.SetBit(Elem);}
    catch (exception const& e)
    {
        throw out_of_range("Element is out of universe or negative");
    }
    return;
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    try { BitField.ClrBit(Elem); }
    catch (exception const& e)
    {
        throw out_of_range("Element is out of universe or negative");
    }
    return;
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return ((MaxPower == s.MaxPower) && (BitField == s.BitField));
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet Res(*this);
    Res.InsElem(Elem);
    return Res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet Res(*this);
    Res.DelElem(Elem);
    return Res;
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

istream &operator>>(istream &istr, TSet &Set) // ввод
{
    TSet Tmp(Set.MaxPower);
    std::string tmpw = "", s;
    int i=0, N;
    istr >> s;
    while (s[i] != '\0')
    {
        if (('0' <= s[i]) && (s[i] <= '9'))
            tmpw += s[i];
        else if (s[i] == ' ' && tmpw!="")
        {
            Tmp.InsElem(std::stoull(tmpw));
            tmpw = "";
        }
        else throw invalid_argument("Input arguments must be integers deliminated by whitespaces");
        i++;
    }
    if(tmpw!="")
        Tmp.InsElem(std::stoi(tmpw));
    Set = Tmp;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &Set) // вывод
{
    std::string s = "";
    int i = 0;
    for (; i < Set.MaxPower; i++)
        if (Set.IsMember(i))
            s += to_string(i) + ' ';
    ostr << s;
    return ostr;
}
