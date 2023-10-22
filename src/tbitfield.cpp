// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <algorithm>
#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len <= 0){
        throw ("invalid input");
    }
    else{
        BitLen = len;
        if(len%8==0) MemLen = (len/ (8* sizeof(TELEM)));
        else MemLen = (len/ (8* sizeof(TELEM)))+1;
        pMem = new TELEM[MemLen];
        for (int i = 0; i<MemLen;i++){
            pMem[i] = 0;
        }
    }

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM [MemLen];
    for (int i = 0; i < MemLen; i++){
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    MemLen = 0;
    BitLen = 0;
    delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n<0) || (n > BitLen)){throw out_of_range("Bit can't be <0 or >power of univers!");}
    int ind = n/(8*sizeof(TELEM));
    return ind;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n<0) || (n > BitLen)){throw out_of_range("Bit can't be <0 or >power of univers!");}
    const TELEM mask = 1 << (n%(8*sizeof(TELEM)));
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return(BitLen);
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n<0) || (n > BitLen)){throw out_of_range("Bit can't be <0 or >power of univers!");}
    else{
        pMem[GetMemIndex(n)]|= GetMemMask(n);
    }

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n<0) || (n > BitLen)){throw out_of_range("Bit can't be <0 or >power of univers!");}
    else{
        int ind = GetMemIndex(n);
        pMem[ind]&=~(GetMemMask(n));
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n<0) || (n > BitLen)){throw out_of_range("Bit can't be <0 or >power of univers!");}
    else{
        int ind = GetMemIndex(n);
        return (pMem[ind] >> (n & (8*sizeof(TELEM) - 1))) & 1;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this == bf)
        return *this;
    else if (bf.GetLength() != GetLength()) {
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        delete[]pMem;
        pMem = new TELEM[MemLen];
    }
    for (int i = 0;i < BitLen;i++){
        if (bf.GetBit(i)!=0){
            SetBit(i);
        }
        else
            ClrBit(i);
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bf.GetLength() != BitLen) {
        return 0;
    }
    else {
        for (int i = 0; i < BitLen; i++) {
            if (GetBit(i) != bf.GetBit(i))
                return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (bf.GetLength() != BitLen) {
        return 1;
    }
    else {
        for (int i = 0; i < BitLen; i++) {
            if (GetBit(i) != bf.GetBit(i))
                return 1;
        }
    }
    return 0;
}
TBitField TBitField::operator|(const TBitField &bf) { // операция "или"
    TBitField A(max(bf.GetLength(), GetLength()));
    int minMemLen =  min(MemLen, bf.MemLen);
    if (bf.GetLength() > BitLen) {
        for (int i = 0; i< MemLen; i++){
            A.pMem[i] = pMem[i];
        }
    }
    else if (bf.GetLength() < BitLen) {
        for (int i = 0; i< bf.MemLen; i++){
            A.pMem[i] = bf.pMem[i];
        }
    }
    for (int i = 0; i <minMemLen; i++) {
        A.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return A;
}

TBitField TBitField::operator&(const TBitField &bf) { // операция "и"
    TBitField A(max(bf.GetLength(), GetLength()));
    if (bf.GetLength() > BitLen) {
        for (int i = 0; i< MemLen; i++){
            A.pMem[i] = pMem[i];
        }
    }
    else if (bf.GetLength() < BitLen) {
        for (int i = 0; i< bf.MemLen; i++){
            A.pMem[i] = bf.pMem[i];
        }
    }
    for (int i = 0; i < min(MemLen, bf.MemLen); i++) {
        A.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return A;
}

TBitField TBitField::operator~(void) { // отрицание
    TBitField A(GetLength());
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0){
            A.SetBit(i);
        }
        else A.ClrBit((i));
    }
    return A;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) { // ввод
    int t;
    for (int i = 0; i < bf.GetLength(); i++) {
        istr >> t;
        if ((t<0) || (t > bf.GetLength())){throw out_of_range("Bit can't be <0 or >power of univers!");}
        else {
            if (t == 1){
                bf.SetBit(i);
            }
            else if (t==0) bf.ClrBit(i);
            else throw invalid_argument("input must store 1 or 0!");
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) { // вывод
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i) << " ";
    }
    return ostr;
}
