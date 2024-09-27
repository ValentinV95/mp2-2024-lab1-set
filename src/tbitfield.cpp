// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#define exc_negative(l) if(l<=0) {throw length_error("invalid len arg");} 
#define exc_bit_pos(n) if (n < 0 || n >= BitLen) {throw out_of_range("bit position out of range");}
#define exc_mem_ptr(ptr) if(!ptr){throw exception("invalid memory allocation");}

TBitField::TBitField(int len=1)
{
	exc_negative(len);

	BitLen = len;
	MemLen = len / (sizeof(TELEM)*8) + ((len & (sizeof(TELEM)*8 - 1)) != 0);
	pMem = new TELEM[MemLen];
	exc_mem_ptr(pMem);
	memset(pMem, 0, sizeof(TELEM)*MemLen);
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	exc_mem_ptr(pMem);
	memset(pMem, 0, sizeof(TELEM) * MemLen);
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	BitLen = 0;
	delete pMem;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	exc_bit_pos(n);
	return n / sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	exc_bit_pos(n);
	return 1 << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	exc_bit_pos(n);
	pMem[n / (sizeof(TELEM) * 8)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	exc_bit_pos(n);
	pMem[n / (sizeof(TELEM) * 8)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	exc_bit_pos(n);
	return (pMem[n / (sizeof(TELEM) * 8)] & GetMemMask(n)) > 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (bf.BitLen != BitLen) {
		if (bf.MemLen == MemLen) {
			BitLen = bf.BitLen;
		}
		else {
			MemLen = bf.MemLen;
			BitLen = bf.BitLen;
			TELEM* new_pMem = new TELEM[bf.MemLen];
			exc_mem_ptr(new_pMem);
			swap(new_pMem, pMem);
		}
	}
	
	memcpy(pMem, bf.pMem, sizeof(TELEM) * MemLen);

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
		if (pMem[i] != bf.pMem[i]) return 0;
	}
	if (bf.MemLen > MemLen) {
		for (int i = min(MemLen, bf.MemLen); i < max(MemLen, bf.MemLen); i++) {
			if (bf.pMem[i] != 0) return 0;
		}
	}
	else {
		for (int i = min(MemLen, bf.MemLen); i < max(MemLen, bf.MemLen); i++) {
			if (pMem[i] != 0) return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	const int _pt_sz = max(BitLen, bf.BitLen);
	const int _mt_sz = min(MemLen, bf.MemLen);
	const int _mmt_sz = max(MemLen, bf.MemLen);
	TBitField new_field(_pt_sz);
	for (int i = 0; i < _mt_sz; i++) {
		new_field.pMem[i] = pMem[i] | bf.pMem[i];
	}
	
	if(pMem > bf.pMem)
	for (int i = _mt_sz; i < _mmt_sz; i++) {
		new_field.pMem[i] = pMem[i];
	}
	else {
		for (int i = _mt_sz; i < _mmt_sz; i++) {
			new_field.pMem[i] = bf.pMem[i];
		}
	}
	TELEM temp_mask = 0;
	for (int i = 0; i < _pt_sz & (sizeof(TELEM) * 8 - 1); i++) {
		temp_mask |= 1 << i;
	}
	new_field.pMem[_mmt_sz - 1] &= temp_mask;
	return new_field;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	const int _pt_sz = max(BitLen, bf.BitLen);
	const int _mt_sz = min(MemLen, bf.MemLen);
	const int _mmt_sz = max(MemLen, bf.MemLen);
	TBitField new_field(_pt_sz);
	for (int i = 0; i < _mt_sz; i++) {
		new_field.pMem[i] = pMem[i] & bf.pMem[i];
	}

	if (pMem > bf.pMem)
		for (int i = _mt_sz; i < _mmt_sz; i++) {
			new_field.pMem[i] = pMem[i];
		}
	else {
		for (int i = _mt_sz; i < _mmt_sz; i++) {
			new_field.pMem[i] = bf.pMem[i];
		}
	}
	TELEM temp_mask = 0;
	for (int i = 0; i < _pt_sz & (sizeof(TELEM) * 8 - 1); i++) {
		temp_mask |= 1 << i;
	}
	new_field.pMem[_mmt_sz - 1] &= temp_mask;
	return new_field;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField new_field(BitLen);
	for (int i = 0; i < MemLen; i++) {
		new_field.pMem[i] = ~pMem[i];
	}
	TELEM temp_mask = 0;
	for (int i = 0; i < (BitLen & (sizeof(TELEM) * 8 - 1)); i++) {
		temp_mask |= 1 << i;
	}
	new_field.pMem[MemLen - 1] &= temp_mask;
	return new_field;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string temp; cin >> temp;
	if (temp.size() != bf.BitLen) {
		throw length_error("input string size != BitField size");
	}
	for (int i = 0; i < bf.MemLen - 1; i++) {
		for (int j = 0; j < bf.BitLen & (sizeof(TELEM) - 1); j++) {

			if (temp[i*(sizeof(TELEM)*8) + j] == '1') {
				bf.SetBit(i * (sizeof(TELEM) * 8) + j);
			}
			else {
				bf.ClrBit(i * (sizeof(TELEM) * 8) + j);
			}

		}
	}
	for (int i = sizeof(TELEM) * 8 * (bf.MemLen - 1); i < bf.BitLen; i++) {
		if (temp[i] == '1') {
			bf.GetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	for (int i = bf.BitLen; i < (bf.MemLen * (sizeof(TELEM) - 1)); i++) {
		bf.ClrBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (auto it = 0; it < bf.BitLen; it++) {
		cout << bf.GetBit(it);
	}
	return ostr;
}
