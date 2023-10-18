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
		throw invalid_argument("Uncorrect quanity of elements");
	BitLen = len;
	MemLen = (BitLen + BitsPerEl - 1) >> shr;
	pMem = new TELEM[MemLen];
	memset(pMem, 0, sizeof(TELEM) * MemLen);
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.GetLength();
	MemLen = bf.GetMemLen();
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
	pMem = nullptr;
	BitLen = 0;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const// индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen))											//реализация
			throw out_of_range("Bit position is out of the bitfield");
	return n >>shr;
}

TELEM TBitField::GetMemMask(const int n) const// битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen))											//реализацияция
		throw out_of_range("Bit position is out of the bitfield");
	return 1 << (n % BitsPerEl);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const noexcept// получить длину (к-во битов)
{
  return BitLen;
}
int TBitField::GetMemLen(void) const noexcept// получить длину (к-во битов)
{
	return MemLen;
}

void TBitField::SetBit(const int n)// установить бит
{
	if (n<0 || n >= BitLen)
		throw out_of_range("Bit position is out of the bitfield");
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)// очистить бит
{
	if (n < 0 || n >= BitLen)
		throw out_of_range("Bit position is out of the bitfield");
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const// получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw out_of_range("Bit position is out of the bitfield");
	return static_cast <int>(static_cast <bool> (pMem[GetMemIndex(n)] & GetMemMask(n)));
}
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание 
{
	if (*this == bf)
		return *this;
	else
		if (MemLen != bf.GetMemLen())
		{
			TELEM* p = new TELEM[bf.GetMemLen()];
			delete[] pMem;
			pMem = p;
			MemLen = bf.GetMemLen();
		}
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
	BitLen = bf.GetLength();
	return *this;
}

int TBitField::operator==(const TBitField &bf) const noexcept// сравнение
{
	int i;
	if (BitLen != bf.GetLength())
		return 0;
	else
		for (i = 0; i < bf.GetMemLen(); i++)
		{
			if (bf.pMem[i] != pMem[i])
				return 0;
		}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const noexcept // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField res(max(BitLen, bf.GetLength()));
	if (BitLen < bf.BitLen)
	{
		TBitField res(bf);
		for (int i = 0; i < MemLen; i++)
			res.pMem[i] = pMem[i] | bf.pMem[i];
		return res;
	}
	else
	{
		TBitField res(*this);
		for (int i = 0; i < bf.GetMemLen(); i++)
			res.pMem[i] = pMem[i] | bf.pMem[i];
		return res;
	}
}

TBitField TBitField::operator &(const TBitField& bf) // операция "и"
{
	TBitField res(max(BitLen, bf.BitLen));

	for (int i = 0; i < min(MemLen, bf.GetMemLen()); i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	return res;

}

TBitField TBitField::operator~(void)// отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen - 1; i++)
		tmp.pMem[i] = ~pMem[i];
	for (int i = (MemLen - 1)*BitsPerEl; i < BitLen; i++)
		if (!GetBit(i))
			tmp.SetBit(i);
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int t;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> t;
		if (t == 1) bf.SetBit(i);
		else if (t == 0) bf.ClrBit(i);
		else throw out_of_range("Not suitable bit value");
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		ostr << bf.GetBit(i) << ' ';
	}
	return ostr;
}
