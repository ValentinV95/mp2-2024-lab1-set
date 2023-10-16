// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw out_of_range("len should be greater than zero");

	BitLen = len;

	MemLen = (BitLen - 1) / (8 * sizeof(TELEM));

	if ((BitLen - 1) % (8 * sizeof(TELEM))) MemLen++;

	pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	MemLen = bf.MemLen;

	BitLen = bf.BitLen;

	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) 
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;

	MemLen = 0;

	BitLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ( (n < 0) || (n >= BitLen) ) throw out_of_range("n should be greater than zero and less than Bitlen");

	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen)) throw out_of_range("n should be greater than zero and less than Bitlen");

	return static_cast<const TELEM>(1 << (n % (8 * sizeof(TELEM))));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen)) throw out_of_range("n should be greater than zero and less than Bitlen");

	int index = GetMemIndex(n);

	const TELEM mask = GetMemMask(n);

	pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) throw out_of_range("n should be greater than zero and less than Bitlen");

	int index = GetMemIndex(n);

	const TELEM mask = GetMemMask(n);

	pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) throw out_of_range("n should be greater than zero and less than Bitlen");

	const int index = GetMemIndex(n);

	const TELEM mask = GetMemMask(n);

	const TELEM res = pMem[index] & mask;

	return (res != static_cast<TELEM>(0));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (*this == bf) return *this;

	delete[] pMem;

	MemLen = bf.MemLen;

	BitLen = bf.BitLen;
	
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 0; //здесь логично считать tbitfield'ы разной длины не равными, поскольку по смыслу это векторы
										   // + tset'ы имеют максимальную мощность, следовательно, логино, что tset'ы разных максимальных мощностей не равны
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] != bf.pMem[i]) return 0;

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (*this == bf) return 0; 
	else return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	if (BitLen > bf.BitLen) res = *this;
	else res = bf;

	for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
		res.pMem[i] = pMem[i] | bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	for (int i = 0; i < std::min(MemLen, bf.MemLen); i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen-1; i++)
		res.pMem[i] = ~pMem[i];

	for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++)
		if (GetBit(i)) res.ClrBit(i); else res.SetBit(i);

	return res;
}

// ввод/вывод

istream& operator>> (istream& in, TBitField& bf) // ввод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		bool tmp;
		in >> tmp;
		if (tmp) bf.SetBit(i);
		else bf.ClrBit(i);
	}
	return in;
}

ostream& operator<<(ostream& out, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i)) out << 1 << " ";
		else out << 0 << " ";
	}

	out << "\n";

	return out;
}
