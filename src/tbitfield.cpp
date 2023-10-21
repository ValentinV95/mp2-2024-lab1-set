// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cstring>

TBitField::TBitField(int len) : BitLen(len)
{
	if(len <= 0)
		throw std::length_error("bad bitfield len");
	MemLen = 1 + ((BitLen - 1) / (8*sizeof(TELEM)) );
	pMem = new TELEM[MemLen];
	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) : BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
	MemLen = 0;
	BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8*sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (8*sizeof(TELEM) - 1)); // вместо n % sizeof(TELEM)
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n > BitLen)
		throw std::out_of_range("bit index out of bitfield range");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n > BitLen)
		throw std::out_of_range("bit index out of bitfield range");
	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n > BitLen)
		throw std::out_of_range("bit index out of bitfield range");
	return (pMem[GetMemIndex(n)] >> (n & (8*sizeof(TELEM) - 1))) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		TELEM* tmp = new TELEM[MemLen];
		delete[] pMem;
		pMem = tmp;
	}

	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	
	for (int i = 0; i < MemLen - 1; i++)
		if (pMem[i] != bf.pMem[i])
			return 0;

	for (int i = 0; i < (BitLen & (8*sizeof(TELEM) - 1)); i++)
		if (GetBit(MemLen - 1 + i) != bf.GetBit(MemLen - 1 + i))
			return 0;

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !TBitField::operator==(bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField tmp(*this);
		for (int i = 0; i < MemLen; i++)
			tmp.pMem[i] |= bf.pMem[i];
		return tmp;
	}

	else
	{
		TBitField tmp(bf);
		for (int i = 0; i < MemLen; i++)
			tmp.pMem[i] |= pMem[i];
		return tmp;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField tmp(*this);
		for (int i = 0; i < MemLen; i++)
			tmp.pMem[i] &= bf.pMem[i];
		return tmp;
	}

	else
	{
		TBitField tmp(bf);
		for (size_t i = 0; i < MemLen; i++)
			tmp.pMem[i] &= pMem[i];
		return tmp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (int i = 0; i < MemLen - 1; i++)
		tmp.pMem[i] = ~pMem[i];

	for (int i = 0; i < (BitLen & (8*sizeof(TELEM) - 1)); i++)
		tmp.pMem[MemLen - 1] ^= GetMemMask(i);

	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		bool tmp = 0;
		if (istr >> tmp)
			bf.pMem[bf.GetMemIndex(i)] |= (tmp << (i & (8 * sizeof(TELEM) - 1)));
		else
			throw std::invalid_argument("invalid input in TBitField");
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i) << " ";

	return ostr;
}
