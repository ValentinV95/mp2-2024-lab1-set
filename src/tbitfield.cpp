// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len >= 0)
	{
		BitLen = len;
		MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = 0;
		}
	}
	else throw exception("length is less than zero");

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 && n < BitLen)
	{
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	}
	else throw("index out of range");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 && n < BitLen)
	{
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	}
	else throw("index out of range");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 && n < BitLen)
	{
		return (pMem[GetMemIndex(n)] & GetMemMask(n)) > 0;
	}
	else throw("index out of range");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
		{
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	const TBitField* A = this;
	const TBitField* B = &bf;
	if (A->BitLen > B->BitLen) swap(A, B); // |A| < |B|
	for (int i = 0; i < A->MemLen; i++)
	{
		if ((A->pMem[i] ^ B->pMem[i]) != (TELEM)0) return 0;
	}
	for (int i = A->MemLen; i < B->MemLen; i++)
	{
		if (B->pMem[i] != (TELEM)0) return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 1 - (*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	const TBitField* A = this;
	const TBitField* B = &bf;
	if (A->BitLen > B->BitLen) swap(A, B); // |A| < |B|
	TBitField res = *B;
	for (int i = 0; i < A->MemLen; i++)
	{
		res.pMem[i] |= A->pMem[i];
	}
	if (res.BitLen != res.MemLen * 8 * sizeof(TELEM))
	{
		res.pMem[res.MemLen - 1] &= (((TELEM)1 << (res.BitLen % (8 * sizeof(TELEM)))) - 1);
	}
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(max(BitLen, bf.BitLen));
	size_t tMemLen = min(MemLen, bf.MemLen);
	for (int i = 0; i < tMemLen; i++)
	{
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res = *this;
	for (int i = 0; i < MemLen; i++)
	{
		res.pMem[i] = ~res.pMem[i];
	}
	if (res.BitLen != res.MemLen * 8 * sizeof(TELEM))
	{
		res.pMem[res.MemLen - 1] &= (((TELEM)1 << (res.BitLen % (8 * sizeof(TELEM)))) - 1);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int BitLen;
	istr >> BitLen;
	TBitField res(BitLen);
	for (int i = 0; i < res.BitLen; i++)
	{
		char bit;
		istr >> bit;
		if (bit == '1') res.SetBit(i);
	}
	bf = res;
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << bf.BitLen << '\n';
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << (char)('0' + bf.GetBit(i));
	}
	return ostr;
}