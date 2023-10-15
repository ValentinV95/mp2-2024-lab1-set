// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
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
    TBitField tmp(BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem > MaxPower) || (Elem < 0)) {
        throw exception("invalid elem");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem > MaxPower) || (Elem < 0)) {
        throw exception("invalid elem");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem > MaxPower) || (Elem < 0)) {
        throw exception("invalid elem");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (*this == s) {
        return (*this);
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return (*this);
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (s.BitField == BitField) {
        return 1;
    }
    else{
        return 0;
    }
    
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    if (s.BitField != BitField) {
        return 1;
    }
    else {
        return 0;
    }
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet res(max(s.MaxPower, MaxPower));
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ((Elem > MaxPower) || (Elem < 0)) {
        throw exception("invalid elem");
    }
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ((Elem > MaxPower) || (Elem < 0)) {
        throw exception("invalid elem");
    }
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet res(s);
    res.BitField = BitField & s.BitField;
    return res;
}

TSet TSet::operator~(void) // дополнение
{
    TSet res(*this);
    res.BitField = ~res.BitField;
    return res;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int tmp;
    for (int i = 0; i < s.MaxPower; i++) {
        istr >> tmp;
        if ((tmp >= 0) && (tmp <= s.MaxPower)) {
            s.InsElem(tmp);
        }
        else {
            cout << "invalid elem, try again enter 0<elem<=MaxPower" << endl;
            i--;
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i) == 1) {
            ostr << i << ",";
        }
    }
    return ostr;
}
