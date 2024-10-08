// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

void range_except(int n, int size) {
	if (n < 0 || n >= size)
		throw out_of_range("Bit index out of range");
}

const int digit = 8 * sizeof(TELEM);

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw length_error("Bitfield length cannot be negative");
	BitLen = len;
	MemLen = (BitLen + digit - 1) / digit;
	pMem = new TELEM[MemLen];
	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	std::copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField()
{
	delete pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / digit);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return TELEM(1) << (n % digit);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	range_except(n, BitLen);
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	range_except(n, BitLen);
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	range_except(n, BitLen);
	return bool(pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this == &bf)
		return *this;
	if (BitLen != bf.BitLen) {
		MemLen = bf.MemLen;
		BitLen = bf.BitLen;
		delete pMem;
		pMem = new TELEM[MemLen];
	}
	std::copy(bf.pMem, bf.pMem + MemLen, pMem);
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	for (int i = 0; i < min(MemLen, bf.MemLen); i++) 
		if (pMem[i] != bf.pMem[i])
			return 0;

	for (int i = min(MemLen, bf.MemLen); i < MemLen; i++)
		if (pMem[i] != 0)
			return 0;

	for (int i = min(MemLen, bf.MemLen); i < bf.MemLen; i++)
		if (bf.pMem[i] != 0)
			return 0;

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField uni(max(BitLen, bf.BitLen));

	for (int i = 0; i < min(MemLen, bf.MemLen); i++)
		uni.pMem[i] = pMem[i] | bf.pMem[i];

	for (int i = min(MemLen, bf.MemLen); i < MemLen; i++)
		uni.pMem[i] = pMem[i];

	for (int i = min(MemLen, bf.MemLen); i < bf.MemLen; i++)
		uni.pMem[i] = bf.pMem[i];

	return uni;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField inter(max(BitLen, bf.BitLen));
	for (int i = 0; i < min(MemLen, bf.MemLen); i++)
		inter.pMem[i] = pMem[i] & bf.pMem[i];
	return inter;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField nega(BitLen);
	for (int i = 0; i < MemLen; i++)
		nega.pMem[i] = ~pMem[i];
	nega.pMem[MemLen - 1] &= (GetMemMask(BitLen - 1) << 1) - TELEM(1);
	return nega;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	std::string s;
	istr >> s;
	for (int i = 0; i < bf.BitLen; i++) {
		if (s[i] == '1')
			bf.SetBit(i);
		else if (s[i] == '0')
			bf.ClrBit(i);
		else
			throw range_error("Values must be 0 or 1.");
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int it = 0; it < bf.BitLen; it++) {
		ostr << bf.GetBit(it);
	}
	return ostr;
}
