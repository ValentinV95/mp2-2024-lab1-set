// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw std::invalid_argument("Length don't must negative");
    }
    BitLen = len;
    MemLen = (len + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen];
    memset( pMem, 0, sizeof(TELEM) * MemLen );
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
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ( n >= 0 && n < BitLen) {
        return n / (sizeof(TELEM) * 8);
    }
    throw out_of_range(" n out of range ");
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ( n >= 0 && n < BitLen) {
        return TELEM(1) << (n % (sizeof(TELEM) * 8));
    }
    throw out_of_range( " n out of range " );
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ( n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return pMem[GetMemIndex(n)] & GetMemMask(n) ? 1 : 0;
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
    if (BitLen != bf.BitLen){
        return 0;
    }
    for ( int i = 0; i < MemLen; i++) {
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
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField temp(len);
    int minmemlen = min( MemLen, bf.MemLen );
    for ( int i = 0; i < minmemlen; i++){
        temp.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if ( MemLen > bf.MemLen ) {
        for ( int i = minmemlen; i < MemLen; i++){
            temp.pMem[i] = pMem[i];
        }
    }
    else {
        for ( int i = minmemlen; i < bf.MemLen; i++){
            temp.pMem[i] = bf.pMem[i];
        }
    }
    if ( len % (sizeof(TELEM) * 8)){
        TELEM mask = (TELEM(1) << ((len % (sizeof(TELEM) * 8)))) - TELEM(1);
        temp.pMem[max(MemLen, bf.MemLen) - 1 ] &= mask;
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    int minmemlen = min( MemLen, bf.MemLen );
    TBitField temp(len);
    
    for (int i = 0; i < minmemlen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    int maxmemlen = max( MemLen, bf.MemLen );
    for ( int i = minmemlen; i < maxmemlen; i++){
        temp.pMem[i] = TELEM(0);
    }
    
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen);
    for ( int i = 0; i < MemLen; i++){
        temp.pMem[i] = ~pMem[i];
    }
    if ( BitLen % (sizeof(TELEM) * 8)){
        TELEM mask = (TELEM(1) << ((BitLen % (sizeof(TELEM) * 8)))) - TELEM(1);
        temp.pMem[MemLen - 1 ] &= mask;
    }
    return temp;
}

// ввод/вывод

std::istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    int i = 0;
    while (i < bf.BitLen ) {
        if (ch == '0') {
            bf.ClrBit(i++);
        } else if (ch == '1') {
            bf.SetBit(i++);
        } else {
            break;
        }
    }
    return istr;
}

std::ostream &operator<<(ostream &ostr, const TBitField &bf)// вывод
{
    for (int i = 0; i < bf.BitLen; ++i) {
        if (bf.GetBit(i)) {
            ostr << '1' ;
        } else {
            ostr << '0';
        }
    }
    return ostr;
}
