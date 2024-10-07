// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = len / sizeof(TELEM) + 1;
	pMem = new TELEM[MemLen];
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i{}; i < BitLen; ++i)
	{
		if (bf.GetBit(i)) this->SetBit(i);
		else this->ClrBit(i);
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	delete pMem;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM m(1);
	int n1 = n % (sizeof(TELEM) * 8);
	for (int i = 1; i < n1; ++i)
	{
		m *= 2;
	}
	return m;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int i = pMem[GetMemIndex(n)] & GetMemMask(n);
	return i;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	for (int i = 0; i < BitLen; ++i)
	{
		if (bf.GetBit(i)) this->SetBit(i);
		else this->ClrBit(i);
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int flag{};
	for (int i = 0; i < BitLen; ++i)
	{
		if (bf.GetBit(i) != this->GetBit(i)) flag++;
	}
	//if (flag) flag = 0;
	//else flag = 1;
	return !flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int flag{};
	for (int i = 0; i < BitLen; ++i)
	{
		if (bf.GetBit(i) != this->GetBit(i)) flag++;
	}
	if (flag) flag = 1;
	return flag;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	// need to check if they are the same size
	TBitField temp(BitLen);
	for (int i = 0; i < BitLen; ++i)
	{
		if (temp.GetBit(i) | bf.GetBit(i)) temp.SetBit(i);
		else temp.ClrBit(i);
	}
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	// need to check if they are the same size
	TBitField temp(BitLen);
	for (int i = 0; i < BitLen; ++i)
	{
		if (temp.GetBit(i) & bf.GetBit(i)) temp.SetBit(i);
		else temp.ClrBit(i);
	}
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i = 0; i < BitLen; ++i)
	{
		if (GetBit(i)) temp.SetBit(i);
		else temp.ClrBit(i);
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	return ostr;
}
