// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw logic_error("Length should be positive");
	BitLen = len;
	if (len % 8 == 0)
		MemLen = len / 8 / sizeof(TELEM);
	else
		MemLen = len / 8 / sizeof(TELEM) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] pMem;

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (8 * sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= BitLen) || (n < 0))
		throw out_of_range("Bit should be between 0 and power of univers");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n >= BitLen) || (n < 0))
		throw out_of_range("Bit should be between 0 and power of univers");

	pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n >= BitLen) || (n < 0))
		throw out_of_range("Bit should be between 0 and power of univers");
	if (pMem[GetMemIndex(n)] & GetMemMask(n))
		return 1;
	else
		return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (*this == bf)
		return *this;

	if (BitLen != bf.BitLen)
	{
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
	}

	for (int ind = 0; ind < MemLen; ind++)
		pMem[ind] = bf.pMem[ind];
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int ind = 0; ind < MemLen; ind++)
	{
		if (pMem[ind] != bf.pMem[ind])
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 1;
	for (int ind = 0; ind < MemLen; ind++)
	{
		if (pMem[ind] != bf.pMem[ind])
			return 1;
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	if (BitLen >= bf.BitLen)
		TBitField res = *this;
	else
		TBitField res = bf;

	for (int ind = 0; ind < std::min(MemLen, bf.MemLen); ind++)
		res.pMem[ind] = bf.pMem[ind] | pMem[ind];
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField res(std::max(BitLen, bf.BitLen));

	for (int ind = 0; ind < std::min(MemLen, bf.MemLen); ind++)
		res.pMem[ind] = bf.pMem[ind] & pMem[ind];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen-1; i++)
	{
		res.pMem[i] = ~pMem[i];
	}
	for (int i = (MemLen - 1)*8*sizeof(TELEM); i<BitLen; i++)
		if (!(GetBit(i)))
			res.SetBit(i);

	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int x;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> x;
		if (x == 1)
			bf.SetBit(i);
		else if (x == 0)
			bf.ClrBit(i);
		else
			throw logic_error("Bit should be 1 or 0");
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int ind = 0; ind < bf.BitLen; ind++)
	{
		if (bf.GetBit(ind) == 1)
			ostr << 1 << " ";
		else 
			ostr << 0 << " ";
	}
	ostr << "\n";
	return ostr;
}
