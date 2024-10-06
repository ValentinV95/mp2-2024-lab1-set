// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>
#define sz					( sizeof(TELEM) << 3 )	//	размер TELEM в битах
#define chck_crct_len(l)	if(len<=0) {throw length_error("Invalid length. Positive numbers only accepted");}	//	Проверка на кооректность ввода длины
#define chck_mem_fail(p)	if (!p) {throw exception("Unsuccessful memory allocation");}	//	Проверка выделения памяти
#define chck_rng(n)			if (n<0 || n>=BitLen) {throw out_of_range("Invailid bit number. Stay inbounds from 0 to Bitfield_length - 1");}	//	Проверка ОДЗ номеров битов

TBitField::TBitField(int len)
{
	chck_crct_len(len);
	BitLen = len;
	MemLen = BitLen / sz + bool(BitLen % sz);
	pMem = new TELEM[MemLen];
	chck_mem_fail(pMem);
	fill(pMem, pMem + MemLen, 0);
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;		//   В строке 50 tbitfield.h написано: бит.поле - набор битов с номерами от 0 до BitLen. Но похоже под BitLen подразумеватся длина поля, тогда последний бит должен быть под номером BitLen - 1?
	MemLen = bf.MemLen;
	pMem = new TELEM(MemLen);
	chck_mem_fail(pMem);
	copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return (n / sz);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return (TELEM(1) << (n % sz));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	chck_rng(n);
	pMem[GetMemIndex(n)] |= GetMemMask(n);
	return;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	chck_rng(n);
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	return;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	chck_rng(n);
	return (bool(pMem[GetMemIndex(n)] & GetMemMask(n)));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		if (MemLen != bf.MemLen)
		{
			delete[] pMem;
			pMem = new TELEM(bf.MemLen);
			chck_mem_fail(pMem);
		}
		MemLen = bf.MemLen;
		BitLen = bf.BitLen;
		copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int i = 0, j = 1;
	int const mnMemLen = min(MemLen, bf.MemLen);	//	Можно сравнивать поля разной длины
	while (j && (i < mnMemLen))				//	Сравнение общих частей
	{
		j = !(pMem[i] ^ bf.pMem[i]);
		i++;
	}
	if (j && (MemLen >= bf.MemLen))			//	Если поля различной длины, проверяется что оставшиеся эл-менты большего поля – нули
		while (i < MemLen)
		{
			j = !(pMem[i]);
			i++;
		}
	else if (j)
		while (i < bf.MemLen)
		{
			j = !(bf.pMem[i]);
			i++;
		}
	return j;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i = 0;
	int const mnMemLen = min(MemLen, bf.MemLen);
	TBitField Res(max(BitLen, bf.BitLen));
	for (; i < mnMemLen; i++)
		Res.pMem[i] = pMem[i] | bf.pMem[i];
	if (MemLen >= bf.MemLen)
		for (; i < MemLen; i++)
		{
			Res.pMem[i] = pMem[i];
		}
	else
		for (; i < bf.MemLen; i++)
		{
			Res.pMem[i] = pMem[i];
		}
	return Res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i = 0;
	int const mnMemLen = min(MemLen, bf.MemLen);
	TBitField Res(max(BitLen, bf.BitLen));
	for (; i < mnMemLen; i++)
		Res.pMem[i] = pMem[i] & bf.pMem[i];
	return Res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField Res(BitLen);
	for (int i = 0; i < Res.MemLen; i++)
	{
		Res.pMem[i] = ~pMem[i];
	}
	Res.pMem[Res.MemLen - 1] &= ((TELEM(1) << (Res.BitLen % sz)) - 1llu);
	return Res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	std::string s;
	int i = 0;
	istr >> s;
	if (s.length() > bf.BitLen)
		throw overflow_error("String is longer than the bit field");
	for (; s[i] != '\0'; i++)
	{
		if (s[i] == '0')
			bf.ClrBit(i);
		else if (s[i] == '1')
			bf.SetBit(i);
		else throw invalid_argument("Input arguments must be 0 or 1 without whitespaces");
	}
	for (; i < bf.BitLen; i++)
		bf.ClrBit(i);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int i = 0;
	std::string s;
	for (; i < bf.BitLen; i++)
		s += std::to_string(bf.GetBit(i));
	ostr << s;
	return ostr;
}
