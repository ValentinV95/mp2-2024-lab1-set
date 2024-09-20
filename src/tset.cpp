// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h" 
#include <string>

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    //if (mp < 0) // non executable code
    //    throw length_error("set can't be created with max number below zero");
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
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
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("element is out of universe");

    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("element is out of universe");

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw out_of_range("element is out of universe");

    BitField.ClrBit(Elem);
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
    return static_cast<int>((MaxPower == s.MaxPower) && (BitField == s.BitField));
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return static_cast<int>((MaxPower != s.MaxPower) || (BitField != s.BitField));
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res = *this;
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res = *this;
    res.DelElem(Elem);
    return res;
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
    string input, tmp = "";

    getline(istr>>ws, input);

    for (size_t i = 0; i < input.length(); ++i) {
        if (input[i] == ' ') {
            if (tmp != "") {
                s.InsElem(static_cast<TELEM>(strtoull(tmp.c_str(), nullptr, 10)));
                tmp = "";
            }
        }

        else
            tmp += input[i];
    }
    if (tmp != "")
        s.InsElem(static_cast<TELEM>(strtoull(tmp.c_str(), nullptr, 10)));

    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    string output = "";
    for (size_t i = 0; i < s.GetMaxPower(); ++i)
        if (s.BitField.GetBit(i))
            output += to_string(i) + " ";
    ostr << output;

    return ostr;
}
