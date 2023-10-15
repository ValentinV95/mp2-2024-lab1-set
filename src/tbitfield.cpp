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
		throw out_of_range("Uncorrect quanity of elements");
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
	/*for (int i = 0; i < BitLen; i++)
	{
		if (bf.GetBit(i))
			SetBit(i);
		else
			ClrBit(i);
	}*/
	memcpy(pMem, bf.GetpMem(), MemLen * sizeof(TELEM));
}
TBitField::TBitField(TELEM* mem, int len)
{
	pMem = mem;
	BitLen = len;
	MemLen = (BitLen + BitsPerEl - 1) >> shr;
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
	if (n >= BitLen)											//реализация
			throw out_of_range("Bit position is out of the bitfield");
	return n >>shr;
}

TELEM TBitField::GetMemMask(const int n) const// битовая маска для бита n
{
	if (n >= BitLen)											//реализацияция
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

void TBitField::SetElem(TELEM Elem, int ind)
{
	if (ind < 0 || ind >= MemLen)
		throw out_of_range("Bit position is out of the bitfield");
	pMem[ind] = Elem;
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
TELEM* TBitField::GetpMem(void) const noexcept //получить указатель на память
{
	return pMem;
}
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание 
{
	if (*this == bf)
		return *this;
	else
		if (BitLen != bf.GetLength())
		{
			TELEM* p = new TELEM[bf.GetLength()];
			delete[] pMem;
			BitLen = bf.GetLength();
			pMem = p;
		}
	/*for (int i = 0; i < bf.GetLength(); i++)
	{
		if (bf.GetBit(i))
			SetBit(i);
		else
			ClrBit(i);
	}*/
	memcpy(pMem, bf.GetpMem(), MemLen * sizeof(TELEM));
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
			if (bf.GetpMem()[i] != pMem[i])
				return 0;
		}
	if (i == bf.GetBit(i)) return 1;
}

int TBitField::operator!=(const TBitField &bf) const noexcept // сравнение
{
	/*int i;
	if (BitLen != bf.GetLength())
		return 1;
	else
		for (i = 0; i < bf.GetLength(); i++)
		{
			if (bf.GetBit(i) != GetBit(i))
				return 1;
		}
	if (i == bf.GetBit(i)) return 0;*/
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TELEM* tmp = new TELEM[max(MemLen, bf.GetMemLen())];
	int i;
	for (i = 0; i < min(MemLen, bf.GetMemLen()); i++)
		tmp[i] = pMem[i] | bf.GetpMem()[i];
	if (BitLen > bf.GetLength())
	{
		for (;i < MemLen; i++)
		{
			tmp[i] = pMem[i];
		}
	}
	else
	{
		for (;i < bf.GetMemLen(); i++)
		{
			tmp[i] = bf.GetpMem()[i];
		}
	}
	TBitField res(tmp, max(BitLen, bf.GetLength()));
	
	//if (BitLen != bf.GetLength())											//реализация
	//	throw out_of_range("operands have different size");					//если
	//else {																//гарантируется,
	//	TBitField tmp(BitLen);												//что
	//	TELEM* res = tmp.GetpMem();											//обрабатываются
	//	TELEM* p = bf.GetpMem();											//только
	//	for (int i = 0; i < MemLen; i++)									//соразмерные
	//		res[i] = pMem[i] | p[i];										//битовые
	//	/*for (int i = 0; i < BitLen; i++)			///кусок
	//		if (GetBit(i) | bf.GetBit(i))			///реализации
	//			tmp.SetBit(i);						///
	//		else									///"в лоб"
	//			tmp.ClrBit(i);*/					///
	//	return tmp;															//поля
	//}
	return res;
}

TBitField TBitField::operator &(const TBitField& bf) // операция "и"
{
	TELEM* tmp = new TELEM[max(MemLen, bf.GetMemLen())];
	int i;
	for (i = 0; i < min(MemLen, bf.GetMemLen()); i++)
		tmp[i] = pMem[i] & bf.GetpMem()[i];
	if (BitLen > bf.GetLength())
	{
		for (; i < MemLen; i++)
		{
			tmp[i] = pMem[i];
		}
	}
	else
	{
		for (; i < bf.GetMemLen(); i++)
		{
			tmp[i] = bf.GetpMem()[i];
		}
	}
	TBitField res(tmp, max(BitLen, bf.GetLength()));
	//if (BitLen != bf.GetLength())											//реализация
	//	throw out_of_range("operands have different size");					//если
	//else {																//гарантируется,
	//	TBitField tmp(BitLen);												//что
	//	TELEM* res = tmp.GetpMem();											//обрабатываются
	//	TELEM* p = bf.GetpMem();											//только
	//	for (int i = 0; i < MemLen; i++)									//соразмерные
	//		res[i] = pMem[i] & p[i];										//битовые
	//	/*for (int i = 0; i < BitLen; i++)			///кусок
	//		if (GetBit(i) & bf.GetBit(i))			///реализации
	//			tmp.SetBit(i);						///
	//		else									///"в лоб"
	//			tmp.ClrBit(i);*/					///
	//	return tmp;															//поля
	//}
	return res;
}

TBitField TBitField::operator~(void)// отрицание
{
	TBitField tmp(BitLen);
	TELEM* res = tmp.GetpMem();
	//for (int i = 0; i < MemLen; i++)
	//	tmp.SetElem(~res[i], i);
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i))
			tmp.ClrBit(i);
		else
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
		if (t) bf.SetBit(i);
		else bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.GetLength(); i++)
	{
		ostr << bool(bf.GetBit(i)) << ' ';
	}
	return ostr;
}
