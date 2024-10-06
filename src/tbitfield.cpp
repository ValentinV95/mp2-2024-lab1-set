// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <string>
#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0) {

		BitLen = len; 
		MemLen = (len + (sizeof(TELEM) << 3) - 1) / (sizeof(TELEM) << 3);
		pMem = new TELEM[MemLen];

		memset(pMem, 0, sizeof(TELEM) * MemLen);
	}
	else {
		throw out_of_range("invalid bit field lenght");
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
	//BitLen = MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) { // Проверка на корректность входных данных
		throw out_of_range("bit position out of range");
	}
	else {
		return n / (sizeof(TELEM) * 8);
	}
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) { // Проверка на корректность входных данных
		throw out_of_range("bit position out of range");
	}
	else {
		return (1 << ((n % (sizeof(TELEM) * 8))));
	}
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) { // Проверка на корректность входных данных
		throw out_of_range("bit position out of range");
	}
	else {
		pMem[GetMemIndex(n)] |= (GetMemMask(n));
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) { // Проверка на корректность входных данных
		throw out_of_range("bit position out of range");
	}
	else {
		pMem[GetMemIndex(n)] &= (~GetMemMask(n));
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) { // Проверка на корректность входных данных
		throw out_of_range("bit position out of range");
	}
	if ((pMem[GetMemIndex(n)] & GetMemMask(n)) == (TELEM)0) {
		return 0;
	}
	else {
		return 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		if (MemLen != bf.MemLen) { 
			if (pMem != nullptr) {
				delete[] pMem;
				pMem = new TELEM[bf.MemLen];
			}
		}
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int temp_MemLen = min(MemLen, bf.MemLen);

	for (int i = 0; i < temp_MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	if (bf.MemLen > MemLen) {
		for (int i = MemLen; i < bf.MemLen; i++) {
			if (bf.pMem[i] != 0) {
				return 0;
			}
		}
	}
	else {
		for (int i = bf.MemLen; i < MemLen; i++) {
			if (pMem[i] != 0) {
				return 0;
			}
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
	TBitField result(max(BitLen, bf.BitLen));
	int m = min(MemLen, bf.MemLen);

	for (int i = 0; i < m; i++) {
		result.pMem[i] = (pMem[i] | bf.pMem[i]);
	}

	for (int i = m; i < max(MemLen, bf.MemLen); i++) {
		if (MemLen > bf.MemLen) {
			result.pMem[i] = pMem[i];
		}
		else {
			result.pMem[i] = bf.pMem[i];
		}
	}
	return result;
	// последнее копирование происходит из pmem макс. длины, хвост которого занулен при создании объекта  
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField result(max(BitLen, bf.BitLen));
	for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
		result.pMem[i] = pMem[i] & bf.pMem[i];
	}
	//остальные pMem'ы равны нулю
	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField Result = *this;

	for (int i = 0;i < MemLen-1;i++) {
		Result.pMem[i] = ~Result.pMem[i];
	}
	for (int i = (MemLen-1)*(sizeof(TELEM)*8); i < Result.GetLength(); i++) {
		if (GetBit(i) == 0) Result.SetBit(i);
		else Result.ClrBit(i);
	}

	return Result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string input;
	istr >> input;

	if (input.size() != bf.BitLen) { // проверка на корректность аргумента
		throw out_of_range("different lengths");
	}
	else {
		for (int  i= 0; i < input.size(); i++) {
			if (input[i] == '1') {
				bf.SetBit(i);
			}
			else if (input[i] == '0'){
				bf.ClrBit(i);
			}
			else {
				throw input_error("input value its only 0 or 1")
			}
		}
		//return istr
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i) == 0) {
			ostr << 0;
		}
		else {
			ostr << 1;
		}
	}
	return ostr;
}
