// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h" // пусть нумерация множества начинается с нуля

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    if (mp < 0) // все равно вызовется тот самый exception из bitfield
        throw exception("set can't be created with max number below zero");
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
        throw exception("element is out of universe");

    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw exception("element is out of universe");

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw exception("element is out of universe");

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
    return int((MaxPower == s.MaxPower) && (BitField == s.BitField)); // доступно сравнение множеств с разным универсом
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return int((MaxPower != s.MaxPower) || (BitField != s.BitField)); // доступно сравнение множеств с разным универсом
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw exception("element is out of universe");

    TSet res = *this;
    res.BitField.SetBit(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw exception("element is out of universe");

    TSet res = *this;
    res.BitField.ClrBit(Elem);
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
    int Elem;
    istr >> Elem;
    s.InsElem(Elem); // включить элемент в множество
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (size_t i = 0; i < s.MaxPower; ++i)
        if (s.BitField.GetBit(i))
            ostr << i << " ";

    return ostr;
}
