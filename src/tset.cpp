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
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw out_of_range("invalid elem");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw out_of_range("invalid elem");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw out_of_range("invalid elem");
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
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw out_of_range("invalid elem");
    }
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ((Elem >= MaxPower) || (Elem < 0)) {
        throw out_of_range("invalid elem");
    }
    TSet res(*this);
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
    int count;
    istr >> count;
    if (count > s.MaxPower || count <= 0) {
        throw out_of_range("count should be >0 and <=maxpower");
    }
    for (int i = 0; i < count; i++) {
        int tmp;
        istr >> tmp;
        if ((tmp >= 0) && (tmp < s.MaxPower)) {
            s.InsElem(tmp);
        }
        else {
            throw out_of_range("invalid elem");
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
