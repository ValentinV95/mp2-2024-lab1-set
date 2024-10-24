// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0){
		throw std::out_of_range("Error with position in bitfild");
	}
	BitLen = len;//длина массива
	MemLen = (BitLen + sizeof(TELEM)*8 - 1) / (sizeof(TELEM)*8);//сколько объектов типа TELEM нужно создать для хранения всех BitLen чисел
	pMem = new TELEM[MemLen];//выделение памяти
	memset(pMem, 0, sizeof(TELEM)*MemLen);//начало памяти для обработки; каким байтом заполнить каждый байт;сколько байт заполнить
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{//устанавливаем размеры
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
	BitLen = 0;
	MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{

	if (n >= 0 && n < BitLen) {
		return n / (sizeof(TELEM) * 8);
	}
	else {
		throw std::out_of_range("Error with position in bitfild");
	}
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n >= 0 && n < BitLen) {
		return TELEM(1) << (n % (sizeof(TELEM) * 8));
	}
	else {
		throw std::out_of_range("Error with position in bitfild");
	}
	//Для установки/снятия отдельных битов (номер цифры внутри числа)
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= 0 || n < BitLen) {
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	}
	else {
		throw std::out_of_range("Error with position in bitfild");
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= 0 || n < BitLen) {
		pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	}
	else {
		throw std::out_of_range("Error with position in bitfild");
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= 0 || n< BitLen) 
		return (pMem[GetMemIndex(n)] & GetMemMask(n)) > 0;
	throw std::out_of_range("Error with position in bitfild");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; ++i) {
			pMem[i] = bf.pMem[i];
		}
	}
	return  *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
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
     int maxBL = max(BitLen, bf.BitLen);
	 int minML = min(MemLen, bf.MemLen);
	TBitField mas(maxBL);
	for (int i = 0; i < minML; i++) {
		mas.pMem[i] = pMem[i] | bf.pMem[i];
	}
	if (MemLen < bf.MemLen) {
		for (int i = minML; i < bf.MemLen; i++) {
			mas.pMem[i] = bf.pMem[i];
		}
	}
	else {
		for (int i = minML; i < MemLen; i++) {
			mas.pMem[i] = pMem[i];
		}
	}
	if ((maxBL % (sizeof(TELEM) * 8)) != 0) {
		 TELEM mask = ((TELEM{ 1 }) << (maxBL % (sizeof(TELEM) * 8))) - (TELEM{ 1 });
		mas.pMem[max(MemLen, bf.MemLen) - 1] &= mask;
	}
	return mas;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	 int maxBL = max(BitLen, bf.BitLen);
	 int minML = min(MemLen, bf.MemLen);
	TBitField mas(maxBL);
	for (int i = 0; i < minML; i++) {
		mas.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return mas;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField mas(BitLen);
	for (int i = 0; i < MemLen; i++) {
		mas.pMem[i] = ~pMem[i];
	}
	if ((BitLen % (sizeof(TELEM) * 8)) != 0) {
		 TELEM mask = ((TELEM{ 1 }) << (BitLen % (sizeof(TELEM) * 8))) - (TELEM{ 1 });
		mas.pMem[MemLen - 1] &= mask;
	}
	return mas;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string str;
	istr >> str;
	const int sz = str.size();

	if (sz != bf.BitLen) {
		throw std::out_of_range("Error with position in bitfild");
	}
	for (int i = 0; i < sz; i++) {
		if (str[i] == '1') {
			bf.SetBit(i);
		}
		else if (str[i] == '0') {
			bf.ClrBit(i);
		}
		else {
			throw std::exception("only ones or zeros!");
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.GetBit(i) == 1) {
			ostr << 1;
		}
		else {
			ostr << 0;
		}
	}
	return ostr;
}
