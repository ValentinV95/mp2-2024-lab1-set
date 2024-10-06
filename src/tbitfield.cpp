// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#define exc_negative(l) if(l<0) {throw length_error("invalid len arg");} 
#define exc_bit_pos(n) if (n < 0 || n >= BitLen) {throw out_of_range("bit position out of range");}
#define exc_mem_ptr(ptr) if(!ptr){throw exception("invalid memory allocation");}

TBitField::TBitField(int len)
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
	//хвост уже занулен
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	BitLen = 0;
	delete[] pMem;
	pMem = nullptr;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	exc_bit_pos(n);
	return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	exc_bit_pos(n);
	return (TELEM{1}) << (n & (sizeof(TELEM) * 8 - 1));
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
	if (this != &bf) {
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
				delete[] new_pMem;
			}
		}

		memcpy(pMem, bf.pMem, sizeof(TELEM) * MemLen);
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	const int mn_Ml = min(MemLen, bf.MemLen);
	for (int i = 0; i < mn_Ml; i++) {
		if (pMem[i] != bf.pMem[i]) return 0;
	}
	if (bf.MemLen > MemLen) {
		for (int i = MemLen; i < bf.MemLen; i++) {
			if (bf.pMem[i] != TELEM{ 0 }) return 0;
		}
	}
	else {
		for (int i = bf.MemLen; i < MemLen; i++) {
			if (pMem[i] != TELEM{ 0 }) return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return 1-(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	const int mBL_sz = max(BitLen, bf.BitLen);
	const int ML_sz = min(MemLen, bf.MemLen);
	const int mML_sz = max(MemLen, bf.MemLen);
	TBitField new_field(mBL_sz);
	for (int i = 0; i < ML_sz; i++) {
		new_field.pMem[i] = pMem[i] | bf.pMem[i];
	}
	
	if(MemLen > bf.MemLen)
	for (int i = ML_sz; i < mML_sz; i++) {
		new_field.pMem[i] = pMem[i];
	}
	else {
		for (int i = ML_sz; i < mML_sz; i++) {
			new_field.pMem[i] = bf.pMem[i];
		}
	}
	if ((mBL_sz & (sizeof(TELEM) * 8 - 1)) != 0) {
		const TELEM temp_mask = ((TELEM{ 1 } << (mBL_sz & (sizeof(TELEM) * 8 - 1))) - 1);
		new_field.pMem[mML_sz - 1] &= temp_mask;
		/*for (int i = 0; i < mBL_sz & (sizeof(TELEM) * 8 - 1); i++) {
			temp_mask |= ((TELEM{1}) << i);
		}*/
	}
	return new_field;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	const int mBL_sz = max(BitLen, bf.BitLen);
	const int ML_sz = min(MemLen, bf.MemLen);
	TBitField new_field(mBL_sz);
	for (int i = 0; i < ML_sz; i++) {
		new_field.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return new_field;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField new_field(BitLen);
	for (int i = 0; i < MemLen; i++) {
		new_field.pMem[i] = ~pMem[i];
	}

	if ((BitLen & (sizeof(TELEM) * 8 - 1)) != 0) {
		const TELEM temp_mask = ((TELEM{ 1 } << (BitLen & (sizeof(TELEM) * 8 - 1))) - 1);
		new_field.pMem[MemLen - 1] &= temp_mask;
		/*for (int i = 0; i < (BitLen & (sizeof(TELEM) * 8 - 1)); i++) {
			temp_mask |= ((TELEM)1) << i;
		}*/
	}
	return new_field;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{	
	//ввод требует вводить строку длины BitLen
	string temp; istr >> temp;			//для проверки ввода/вывода для tbitfield и tset написаны тесты, их нужно раскомментировать
	if (temp.size() != bf.BitLen) {
		throw length_error("input string size != BitField size");	//Требую того же размера.
	}
	memset(bf.pMem, 0, sizeof(TELEM) * bf.MemLen);
	for (int i = 0; i < bf.MemLen-1; i++) {
		for (int j = 0; j < (bf.BitLen & (sizeof(TELEM) - 1)); j++) {

			if (temp.at(i*(sizeof(TELEM)*8) + j) == '1') {
				bf.SetBit(i * (sizeof(TELEM) * 8) + j);
			}
			else {
				bf.ClrBit(i * (sizeof(TELEM) * 8) + j);
			}

		}
	}
	for (int i = sizeof(TELEM) * 8 * (bf.MemLen-1); i < bf.BitLen; i++) {
		if (temp[i] == '1') {
			bf.SetBit(i);
		}
		else {
			bf.ClrBit(i);
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (auto it = 0; it < bf.BitLen; it++) {
		ostr << bf.GetBit(it);
	}
	return ostr;
}