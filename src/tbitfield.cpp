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
		MemLen = (len + 8 * sizeof(TELEM) - 1) / (8 * sizeof(TELEM));
		pMem = new TELEM[MemLen];
		fill(pMem, pMem + MemLen, 0);
	}
	else {
		throw exception("bit field lenght can't be below or equal to zero");
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1u << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen) // Проверка на корректность входных данных
		throw exception("bit position can't be lower than a bit field length");
	if (n < 0)
		throw exception("bit powition cam't be below zero");

	pMem[GetMemIndex(n)] |= (GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen) // Проверка на корректность входных данных
		throw exception("bit position can't be lower than a bit field length");
	if (n < 0)
		throw exception("bit powition cam't be below zero");

	pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen) // Проверка на корректность входных данных
		throw exception("bit position can't be lower than a bit field length");
	if (n < 0)
		throw exception("bit powition cam't be below zero");

	return int((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf) 
		return *this;

	if (MemLen != bf.MemLen) { // почти во всех случаях, память lvalue очищается
		delete[] pMem;
		pMem = new TELEM[bf.MemLen];
	}
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int tmpMemLen = max(MemLen, bf.MemLen); // бинарные операции с полями разных длин совместимы

	for (size_t i = 0; i < tmpMemLen; ++i) { // залезаю в хвост
		if (i >= MemLen && bf.pMem[i] != TELEM(0)) 
			return 0;
		if (i >= bf.MemLen && pMem[i] != TELEM(0)) 
			return 0;
		if (pMem[i] != bf.pMem[i])          
			return 0;
	}

	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	int tmpMemLen = max(MemLen, bf.MemLen); // бинарные операции с полями разных длин совместимы

	for (size_t i = 0; i < tmpMemLen; ++i) { // залезаю в хвост
		if (i >= MemLen && bf.pMem[i] != TELEM(0))
			return 1;
		if (i >= bf.MemLen && pMem[i] != TELEM(0))
			return 1;
		if (pMem[i] != bf.pMem[i])
			return 1;
	}

	return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(max(BitLen, bf.BitLen)); // бинарные операции с полями разных длин совместимы
	int tmpMemLen = min(MemLen, bf.MemLen);
	size_t i;

	for (i = 0; i < tmpMemLen; ++i) // залезаю в хвост
		res.pMem[i] = this->pMem[i] | bf.pMem[i];

	if (MemLen > tmpMemLen)
		for (; i < MemLen; ++i)
			res.pMem[i] = this->pMem[i];
	else if (bf.MemLen > tmpMemLen)
		for (; i < bf.MemLen; ++i)
			res.pMem[i] = bf.pMem[i];

	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(max(BitLen, bf.BitLen)); // бинарные операции с полями разных длин совместимы
	int tmpMemLen = min(MemLen, bf.MemLen);

	for (size_t i = 0; i < tmpMemLen; ++i) // залезаю в хвост
		res.pMem[i] = this->pMem[i] & bf.pMem[i];

	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res = *this;

	for (size_t i = 0; i < MemLen; ++i)
		res.pMem[i] = ~res.pMem[i];

	// & tailMask = 0b00011111
	//               ^^^
	//               tail

	TELEM tailMask = 0;
	tailMask = ~((~tailMask) << (BitLen % (8 * sizeof(TELEM))));
	res.pMem[MemLen - 1] &= tailMask;

	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{ // выполнен как ввод константы int для создания объекта, удаляет входящий объект
	istr >> bf.BitLen;
	bf = TBitField(bf.BitLen);

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (size_t i = 0; i < bf.BitLen; ++i) // побитовый вывод
		ostr << bf.GetBit(i);

	return ostr;
}
