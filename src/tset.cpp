// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

#define universe_except throw out_of_range("Invalid element number. Element out of universe.");

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
TSet::TSet(const TBitField& bf) : BitField(bf)
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
    try {
        return BitField.GetBit(Elem);
    }
    catch (exception const& e) {
        universe_except;
    }
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    try {
        BitField.SetBit(Elem);
    }
    catch (exception const& e) {
        universe_except;
    }
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    try { 
        BitField.ClrBit(Elem); 
    }
    catch (exception const& e) {
        universe_except;
    }
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower == s.MaxPower)
        return BitField == s.BitField;
    return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (MaxPower == s.MaxPower)
        return BitField != s.BitField;
    return 1;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet buff = *this;
    buff.InsElem(Elem);
    return buff;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet buff = *this;
    buff.DelElem(Elem);
    return buff;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return ~BitField;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& Set) // ввод
{
    std::string buff = "", s;
    istr >> s;
    for (int i = 0; s[i] != '\0'; i++) {
        if (('0' <= s[i]) && (s[i] <= '9'))
            buff += s[i];
        else if (s[i] == ' ')
        {
            Set.InsElem(stoll(buff));
            buff = "";
        }
        else {
            throw invalid_argument("Input must be a figure.");
        }
    }
    if (buff != "")
        Set.InsElem(stoll(buff));
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    std::string elements = "";
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i))
            elements += to_string(i) + ' ';
    }
    ostr << elements;
    return ostr;
}
