// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0) {
		BitLen = len;
	}
	else {
		throw exception("can't be <=0.");
	}
	MemLen = (BitLen - 1) / (8 * sizeof(TELEM)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = (TELEM)0;
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
	MemLen = 0;
	BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) {
		throw exception("out of range");
	}
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) {
		throw exception("out of range");
	}
	const TELEM res = (1 << (n % (8 * sizeof(TELEM))));
	return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int ind1 = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);
	pMem[ind1] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	int ind1 = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);
	pMem[ind1] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	const int ind1 = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);
	const TELEM res = pMem[ind1] & mask;
	return (res != (TELEM)0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
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
	return (*this == bf) ? 0 : 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	const TBitField *tmp = this;
	const TBitField *max = (this->BitLen >= bf.BitLen) ? tmp : &bf;
	const TBitField *min = (this->BitLen < bf.BitLen) ? tmp : &bf;
	int i;
	if (max == nullptr || min == nullptr) {
		throw exception("what");
	}
	const int maxBitLen = max->BitLen;
	const int minMemLen = min->MemLen;
	TBitField res(maxBitLen);
	for (i = 0; i < minMemLen; i++) {
		res.pMem[i] = max->pMem[i] | min->pMem[i];
	}
	for (; i < res.MemLen; i++) {
		res.pMem[i] = max->pMem[i];
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxBitLen;
	int minMemLen;
	if (this->BitLen > bf.BitLen) {
		maxBitLen = this->BitLen;
		minMemLen = bf.MemLen;
	}
	else {
		maxBitLen = bf.BitLen;
		minMemLen = this->MemLen;
	}
	TBitField res(maxBitLen);
	for (int i = 0; i < minMemLen; i++) {
		res.pMem[i] = this->pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(this->BitLen);
	const int fullLen = MemLen - 1;
	for (int i = 0; i < fullLen; i++) {
		res.pMem[i] = ~(this->pMem[i]);
	}
	const int curbit = 8 * sizeof(TELEM) * fullLen;
	for (int i = curbit; i < BitLen; i++) {
		if (this->GetBit(i) == 0) {
			res.SetBit(i);
		}
		else {
			res.ClrBit(i);
		}
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int size;
	int buf;
	istr >> size;
	TBitField res(size);
	for (int i = 0; i < size; i++) {
		istr >> buf;
		if (buf != 0) {
			res.SetBit(i);
		}
	}
	bf = res;
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << bf.BitLen << endl;
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i) << endl;
	}
	return ostr;
}