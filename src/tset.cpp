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
    TBitField tmp(BitField);
    return tmp;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if ((Elem >= MaxPower) || (Elem < 0))
    {
        throw exception("uncorrect element");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0))
    {
        throw out_of_range("uncorrect element");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
       if ((Elem >= MaxPower) || (Elem < 0))
    {
        throw out_of_range("uncorrect element");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (*this == s)
    {
        return (*this);
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return (*this);
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (s.BitField == BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(s.BitField == BitField);
}

TSet TSet::operator+(const TSet& s) // объединение
{
   TSet tmp(std::max(s.MaxPower, MaxPower));
    tmp.BitField = BitField | s.BitField;
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
     if ((Elem >= MaxPower) || (Elem < 0))
    {
        throw out_of_range("uncorrect element");
    }
    TSet tmp(BitField);
    tmp.InsElem(Elem);
    return tmp;

}

TSet TSet::operator-(const int Elem) // разность с элементом
{
     if ((Elem >= MaxPower) || (Elem < 0))
    {
        throw out_of_range("uncorrect element");
    }
    TSet tmp(*this);
    tmp.DelElem(*this;
    return tmp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
 
    TSet tmp(s);
    tmp.BitField = BitField & s.BitField;
    return tmp;
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(*this);
    tmp.BitField = ~tmp.BitField;
    return tmp;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int a;
    istr >> a;
    if ((a <= 0) || (a > s.MaxPower))
        throw out_of_range("out of range");
    for (int i = 0; i < a; i++)
    {
        int elem;
        istr >> elem;
        if ((elem < 0) || (elem >= s.MaxPower))
            throw out_of_range("out of range");
        s.InsElem(elem);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.IsMember(i))
            ostr << i << ' ';
    }
    ostr << "\n";
    return ostr;
}
