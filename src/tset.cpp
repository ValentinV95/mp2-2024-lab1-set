// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

#define exc_el_pos(n) if (n < 0 || n >= MaxPower) {throw out_of_range("element out of U");}
#define exc_el_pos_out(n,s) if (n < 0 || n >= s.MaxPower) {throw out_of_range("element out of U");}

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
    return ((Elem < 0 || Elem >= MaxPower) ? 0 : BitField.GetBit(Elem));
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    exc_el_pos(Elem);
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    exc_el_pos(Elem);
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if(MaxPower == s.MaxPower)
    return s.BitField == BitField;
    return 0; // mp != mp(s)
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(s == *this);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);  // re set( A U B )
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    exc_el_pos(Elem);
    TSet _uwu_ = *this;
    _uwu_.InsElem(Elem);
    return _uwu_;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    exc_el_pos(Elem);
    TSet _uwu_ = *this;
    _uwu_.DelElem(Elem);
    return _uwu_;
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
    size_t size; cin >> s;          //Первое число - кол-во элементов, следующие - элементы
    int tmp;
    while (size--) {
        cin >> tmp;
        exc_el_pos_out(tmp,s);
        s.InsElem(tmp);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод      //1-е число - колво , след - элементы
{
    string out = "";
    size_t size = 0;
    for (int i = 0; i < s.BitField.GetLength(); i++) {
        if (s.BitField.GetBit(i)) {
            size++;
            out += to_string(i) + " ";
        }
    }
    out = to_string(size) + " " + out;
    cout << out;
    return ostr;
}
