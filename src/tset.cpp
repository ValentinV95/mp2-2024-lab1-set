// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp <= 0)
        throw invalid_argument("Length should be positive");
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
    if ((Elem >= MaxPower) || (Elem < 0))
        throw out_of_range("Index should be between 0 and power of univers");
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0))
        throw out_of_range("Index should be between 0 and power of univers");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if ((Elem >= MaxPower) || (Elem < 0))
        throw out_of_range("Index should be between 0 and power of univers");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.GetMaxPower();
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return (BitField != s.BitField);
}

TSet TSet::operator+(const TSet& s) // объединение
{
   // if (MaxPower != s.MaxPower)
     //   throw logic_error("Universe should be the same");
    TBitField res(std:: max(MaxPower,s.MaxPower));
    res = BitField | s.BitField;
    return TSet(res);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if ((Elem >= MaxPower) || (Elem < 0))
        throw out_of_range("Index should be between 0 and power of univers");
    TBitField res(MaxPower);
    res.SetBit(Elem);
    return TSet(res);
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if ((Elem >= MaxPower) || (Elem < 0))
        throw out_of_range("Index should be between 0 and power of univers");
    TBitField res(MaxPower);
    res.ClrBit(Elem);
    return TSet(res);
}

TSet TSet::operator*(const TSet& s) // пересечение
{
   // if (MaxPower != s.MaxPower)
     //   throw logic_error("Universe should be the same");
    TBitField res(MaxPower);
    res = BitField & s.BitField;
    return TSet(res);
}

TSet TSet::operator~(void) // дополнение
{
    TBitField res(MaxPower);
    res = ~BitField;
    return TSet(res);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int n;
    istr >> n;
    if (n > s.GetMaxPower())
        throw invalid_argument("Count of element should be less than power of Univers");
    for (int i = 0; i < n; i++)
    {
        int x;
        istr >> x;
        s.InsElem(x);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    int n = 0;
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i))
            n++;
    }
    ostr << n << " ";
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i))
            ostr << i << " ";
    }
    
    return ostr;
}
