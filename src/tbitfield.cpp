// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len) {
	if (len > 0) {
		if (len % (sizeof(TELEM)*8)) MemLen = len / (sizeof(TELEM)*8) + 1;
		else MemLen = len / (sizeof(TELEM)*8);
		BitLen = len;
		pMem = new TELEM[MemLen];
		memset(pMem, 0, MemLen* sizeof(TELEM));
	}
	else throw std::out_of_range("Size must be pozitive");
}

// конструктор копирования
TBitField::TBitField(const TBitField& bf) {
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField:: ~TBitField() {
	delete[] pMem;
}

// индекс Мем для бита n
int TBitField::GetMemIndex(const int n) const {
	return (n / (8 * sizeof(TELEM)));
}

// битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const {
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

// получить длину (к-во битов)
int TBitField::GetLength(void) const {
	return BitLen;
}

// установить бит
void TBitField::SetBit(const int n) {
	if ((n>=0)&&(n < BitLen)) pMem[GetMemIndex(n)] |= GetMemMask(n);
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
}

// очистить бит
void TBitField::ClrBit(const int n) {
	if ((n >= 0) && (n < BitLen)) pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
}

// получить значение бита
int TBitField::GetBit(const int n) const {
	if ((n >= 0) && (n < BitLen)) {
		return (pMem[GetMemIndex(n)] & GetMemMask(n));
	} 
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
}

// битовые операции

// присваивание
TBitField& TBitField:: operator=(const TBitField& bf) {
	if (*this != bf) {
		if (MemLen != bf.MemLen) {
			MemLen = bf.MemLen;
			TELEM* tmp = new TELEM[MemLen];
			delete[] pMem;
			pMem = tmp;
		};
		memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
		BitLen = bf.BitLen;
	};
	return *this;
}

// сравнение
int TBitField:: operator==(const TBitField& bf) const {
	if (BitLen == bf.BitLen) {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 0;
		}
		return 1;
	}
	return 0;
}

// сравнение
int TBitField:: operator!=(const TBitField& bf) const {
	return (0== (*this == bf));
}

// операция "или"
TBitField TBitField:: operator|(const TBitField& bf) {
	if (bf.BitLen > BitLen) {
		TBitField bf2(bf);
		for (int i = 0; i < MemLen; i++) bf2.pMem[i] |= pMem[i];
		return bf2;
	}
	else {
		TBitField bf2(*this);
		for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] |= bf.pMem[i];
		return bf2;
	};
}

// операция "и"
TBitField TBitField::operator&(const TBitField& bf) {
	if (bf.BitLen > BitLen) {
		TBitField bf2(bf);
		for (int i = 0; i < MemLen; i++) bf2.pMem[i] &= pMem[i];
		return bf2;
	}
	else {
		TBitField bf2(*this);
		for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] &= bf.pMem[i];
		return bf2;
	}
}

// отрицание
TBitField TBitField:: operator~(void) {
	TBitField bf(BitLen);
	for (int i = 0; i < MemLen; i++) bf.pMem[i] = ~pMem[i];
	for (int i= BitLen; i<MemLen*sizeof(TELEM)*8; i++) bf.pMem[GetMemIndex(i)] &= ~GetMemMask(i);
	return bf;
}

// ввод/вывод

// ввод
istream& operator>>(istream& istr, TBitField& bf) {
	bool tmp;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> tmp;
		if (tmp) bf.SetBit(i);
		else bf.ClrBit(i);
	};
	return istr;
}

// вывод
ostream& operator<<(ostream& ostr, const TBitField& bf) {
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	};
	return ostr;
}
