// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if (mp < 0){throw out_of_range("Univers must be >0");}
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.MaxPower)
{
        BitField = s.BitField;
        MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength())
{

        BitField = bf;
        MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    int mp = MaxPower;
    return mp;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (BitField.GetBit(Elem) && Elem >= 0 && Elem < BitField.GetLength()){
        return 1;
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= BitField.GetLength()){throw out_of_range("Bit can't be <0 or >power of univers!");}
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= BitField.GetLength()){throw out_of_range("Bit can't be <0 or >power of univers!");}
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
        if (*this == s){
            return *this;
        }
        BitField = s.BitField;
        MaxPower = s.GetMaxPower();
        return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return (BitField != s.BitField);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= BitField.GetLength()){throw out_of_range("Bit can't be <0 or >power of univers!");}
    TSet A(MaxPower);
    A = (*this);
    A.InsElem(Elem);
    return A;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= BitField.GetLength()){throw out_of_range("Bit can't be <0 or >power of univers!");}
    TSet A(*this);
    A.DelElem(Elem);
    return A;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
    /*if (MaxPower < s.MaxPower) {
        TSet A(s.MaxPower);
        for (int i = 0; i < MaxPower; i++) {
            if (IsMember(i)) {
                A.InsElem(i);
            }
        }
        for (int i = 0; i < s.MaxPower; i++) {
            if (!s.IsMember(i) && A.IsMember(i)) {
                A.DelElem(i);
            }
        }
        return A;
    } else if (MaxPower > s.MaxPower) {
        TSet A(MaxPower);
        for (int i = 0; i < s.MaxPower; i++) {
            if (s.IsMember(i)) {
                A.InsElem(i);
            }
        }
        for (int i = 0; i < MaxPower; i++) {
            if (!IsMember(i) && A.IsMember(i)) {
                A.DelElem(i);
            }
        }
        return A;
    } else {
        TSet A(MaxPower);
        for (int i = 0; i < MaxPower; i++) {
            if (IsMember(i) && s.IsMember(i)) {
                A.InsElem(i);
            }
        }
        return A;
    }*/
}


TSet TSet::operator~(void) // дополнение
{
    TSet A(MaxPower);
    A.BitField = ~BitField;
    return A;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int t,n;
    istr >> n;
    for (int i= 0; i< n; i++){
        istr >> t;
        if (t > s.MaxPower){
            throw out_of_range("Bit can't be <0 or >power of univers!");
        }
        else{
            s.InsElem(t);
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    int num_of_ins = 0;
    for (int i = 0; i< s.MaxPower; i++){
        if (s.IsMember(i)){
            num_of_ins++;
        }
    }
    ostr << num_of_ins << " ";
    for (int i = 0; i< s.MaxPower; i++){
        if (s.IsMember(i)){
            ostr << i << " ";
        }
    }
    return ostr;
}
