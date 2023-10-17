// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) {
		throw std::out_of_range("bit is out of range ");
	}
	int bitsInElem = 8 * sizeof(TELEM); 
	this->BitLen = len;
	this->MemLen = len / bitsInElem + (len % bitsInElem > 0);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
	BitLen = 0;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit is out of range ");
	}
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit is out of range ");
	}
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n>BitLen) {
		throw std::out_of_range("bit is out of range");
	}

	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen) {
		throw std::out_of_range("bit is out of range ");
	}

	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) {
		throw std::out_of_range("bit is out of range");
	}
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (bf == *this) return *this;
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen) {
		delete[] pMem;
		MemLen = bf.MemLen;
		pMem = new TELEM[bf.MemLen];
	}
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
	
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	for (int i = 0; i < min(MemLen,bf.MemLen); i++) {
		res.pMem[i] = pMem[i] | bf.pMem[i];
	}
	if (MemLen < bf.MemLen) {
		for (int i = MemLen; i < bf.MemLen; i++) {
			res.pMem[i] = bf.pMem[i];
		}
	}
	if (MemLen > bf.MemLen) {
		for (int i = bf.MemLen; i < MemLen; i++) {
			res.pMem[i] = pMem[i];
		}
	}

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	if (MemLen < bf.MemLen) {
		for (int i = MemLen; i < bf.MemLen; i++) {
			res.pMem[i] = 0;
		}
	}
	if (MemLen > bf.MemLen) {
		for (int i = bf.MemLen; i < MemLen; i++) {
			res.pMem[i] = 0;
		}
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	TBitField tmp(8 * sizeof(TELEM));
	for (int i = 0; i < MemLen; i++) {
		res.pMem[i] = ~(pMem[i]);
	}
	
	for (int i = (MemLen - 1) * 8 * sizeof(TELEM); i < BitLen; i++) tmp.pMem[0] |= GetMemMask(i);
	res.pMem[MemLen - 1] &= tmp.pMem[0];
	
	

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++) {
		int t;
		int fl = 1;
		do {
			istr >> t;
			if ((t != 1) && (t != 0)) {
				cout << "Invalid input. Try again" << endl;
			}
			else fl = 0;
		} while (fl);
		if (t == 1) {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i) == 0) {
			ostr << '0';
		}
		else {
			ostr << '1';
		}
	}
	return ostr;
}

