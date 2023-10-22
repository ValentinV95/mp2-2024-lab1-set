// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp): BitField(mp) 
{
	MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s): BitField(s.BitField){
	MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf): BitField(bf){
	MaxPower = bf.GetLength();
}

// преобразование типа к битовому полю
TSet::operator TBitField() {
	return BitField;
}

// получить макс. к-во эл-тов
int TSet::GetMaxPower(void) const {
	return MaxPower;
}

// элемент множества?
int TSet::IsMember(const int Elem) const {
	if ((Elem >= 0) && (Elem < MaxPower)) {
		return (BitField.GetBit(Elem));
	}
	throw out_of_range("Element must belong to the universe of the set");
}

// включение элемента множества
void TSet::InsElem(const int Elem) {
	if ((Elem>=0)&&(Elem < MaxPower)) BitField.SetBit(Elem);
	else throw out_of_range("Element must belong to the universe of the set");
}

// исключение элемента множества
void TSet::DelElem(const int Elem) {
	if ((Elem >= 0) && (Elem < MaxPower)) BitField.ClrBit(Elem);
	else throw out_of_range("Element must belong to the universe of the set");
}

// теоретико-множественные операции

// присваивание
TSet& TSet::operator=(const TSet& s) {
	if (s != *this) {
		BitField = s.BitField;
		MaxPower = s.MaxPower;
	}
	return *this;
}

// сравнение
int TSet::operator== (const TSet& s) const {
	return BitField == s.BitField;
}

// сравнение
int TSet::operator!= (const TSet& s) const {
	return  BitField != s.BitField;
}

// объединение
TSet TSet:: operator+ (const TSet& s) {
	TSet S(max(s.MaxPower, MaxPower));
	S.BitField = BitField | s.BitField;
	return S;
}

// объединение с элементом
TSet TSet::operator+ (const int Elem) {
	TSet S(*this);
	S.InsElem(Elem);
	return S;
}

// разность с элементом
TSet TSet::operator- (const int Elem) {
	if ((Elem >= 0) && (Elem < MaxPower)) {
		TSet S(*this);
		S.DelElem(Elem);
		return S;
	}
	throw out_of_range("Element must belong to the universe of the set");
}

// пересечение
TSet TSet::operator* (const TSet& s) {
	TSet S(max(s.MaxPower, MaxPower));
	S.BitField = BitField & s.BitField;
	return S;
}

// дополнение
TSet TSet::operator~ (void) {
	TSet s(~BitField);
	return s;
}

// перегрузка ввода/вывода

// ввод
istream& operator>>(istream& istr, TSet& s) {
	int count;
	istr >> count;
	if ((count >= 0) && (count < s.MaxPower)) {
		int tmp = 0;
		while (count>0)
		{
			istr >> tmp;
			if ((tmp >= 0) && (tmp < s.MaxPower)) {
				s.InsElem(tmp);
				count--;
			}
			else throw out_of_range("Element must belong to the universe of the set");
		}
		return istr;
	};
	throw out_of_range("Count must must belong to the universe of the set");
}

//вывод
ostream& operator<<(ostream& ostr, const TSet& s) {
	for (int i = 0; i < s.MaxPower; i++) 
		if (s.IsMember(i)) 
			ostr << i << " ";
	return ostr;
};