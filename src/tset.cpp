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
TSet::TSet(const TSet &s) : BitField(s.MaxPower)
{
    try{
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    catch(const char* error_message) {
        MaxPower = s.MaxPower;
        cout << error_message<<endl;
    }


}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength())
{
    try{
        BitField = bf;
        MaxPower = bf.GetLength();
    }
    catch(const char* error_message) {
        MaxPower = bf.GetLength();
        cout << error_message<<endl;
    }
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
    if (BitField.GetBit(Elem) != 0){
        return 1;
    }
    return 0;
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    try{
        BitField = s.BitField;
        MaxPower = s.MaxPower;
        return *this;
    }
    catch(const char* error_message) {
        MaxPower = s.MaxPower;
        return *this;
        cout << error_message<<endl;
    }
}

int TSet::operator==(const TSet &s) const // сравнение
{
    try{
        if ((MaxPower != s.MaxPower) || (BitField != s.BitField)){
            return 0;
        }
        return 1;
    }
    catch(const char* error_message) {
        cout << error_message<<endl;
    }

}

int TSet::operator!=(const TSet &s) const // сравнение
{
    try{
        if ((MaxPower != s.MaxPower) || (BitField != s.BitField)){
            return 1;
        }
        return 0;
    }
    catch(const char* error_message) {
        cout << error_message<<endl;
    }
}

TSet TSet::operator+(const TSet &s) // объединение
{
    try{
        if (MaxPower < s.MaxPower){
            TSet A(s.MaxPower);
            for (int i = 0; i<MaxPower; i++){
                if(IsMember(i)){
                    A.InsElem(i);
                }
            }
            for (int i = 0; i< s.MaxPower; i++){
                if(s.IsMember(i) && !A.IsMember(i)){
                    A.InsElem(i);
                }
            }
            return A;
        }
        else if (MaxPower > s.MaxPower){
            TSet A(MaxPower);
            for (int i = 0; i<s.MaxPower; i++){
                if(s.IsMember(i)){
                    A.InsElem(i);
                }
            }
            for (int i = 0; i< MaxPower; i++){
                if(IsMember(i) && !A.IsMember(i)){
                    A.InsElem(i);
                }
            }
            return A;
        }
        else{
            TSet A(MaxPower);
            for (int i = 0; i< MaxPower; i++){
                if(IsMember(i)||s.IsMember(i)){
                    A.InsElem(i);
                }
            }
            return A;
        }
    }
    catch(const char* error_message) {
        cout << error_message<<endl;
    }

}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem > MaxPower || Elem < 0){
        throw "out of range";
    }
    TSet A(MaxPower);
    A = (*this);
    A.InsElem(Elem);
    return A;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem > MaxPower || Elem < 0){
        throw "out of range";
    }
    TSet A(MaxPower);
    A = (*this);
    A.DelElem(Elem);
    return A;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    if (MaxPower < s.MaxPower) {
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
    }
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
    cout << "Enter a quanity of elements";
    cin >> n;
    for (int i= 0; i< n; i++){
        cin >> t;
        if (t > s.MaxPower){
            cout << "error";
        }
        else{
            s.BitField.SetBit(t);
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i< s.MaxPower; i++){
        if (s.BitField.GetBit(i)){
            cout << i << " ";
        }
    }
    return ostr;
}
