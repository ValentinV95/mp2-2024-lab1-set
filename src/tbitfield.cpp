// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

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
    if (bf.MemLen!=MemLen){throw "out of range";}
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
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
    if ((n<0) || (n > BitLen)){throw "out of range";}
    int ind = n/(8*sizeof(TELEM));
    return ind;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if ((n<0) || (n > BitLen)){throw "out of range";}
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
    if ((n > BitLen) || (n < 0)){
        throw "out of range";
    }
    else{
        int ind = GetMemIndex(n);
        const TELEM mask = GetMemMask(n);
        pMem[ind]|= mask;
    }

}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0){
        throw "out of range";
    }
    else{
        int ind = GetMemIndex(n);
        pMem[ind]&=~(1 << (n%(8*sizeof(TELEM))));
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen || n < 0){
        throw ("out of range");
    }
    else{
        int ind = GetMemIndex(n);
        return ((pMem[ind]) & (1 << (n%(8*sizeof (TELEM)))));
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (bf.GetLength() != GetLength()){
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        delete[]pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i< BitLen;i++){
            if (bf.GetBit(i)!=0){
                SetBit(i);
            }
            else
                ClrBit(i);
        }
    }
    else {
        for (int i = 0;i < BitLen;i++){
            if (bf.GetBit(i)!=0){
                SetBit(i);
            }
            else
                ClrBit(i);
        }
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
    if (bf.GetLength() > BitLen) {
        TBitField A(bf.GetLength());
        for (int i = 0; i< BitLen; i++){
            A.pMem[i] = pMem[i];
        }
        for(int i = 0; i< bf.BitLen; i++){
            A.pMem[i] |= bf.pMem[i];
        }
        return A;
    }
    else if (bf.GetLength() < BitLen) {
        TBitField A(GetLength());
        for (int i = 0; i< bf.BitLen; i++){
            A.pMem[i] = bf.pMem[i];
        }
        for(int i = 0; i< BitLen; i++){
            A.pMem[i] |= pMem[i];
        }
        return A;
    }
    else {
        TBitField A(bf.GetLength());
        for (int i = 0; i < (BitLen); i++) {
            A.pMem[i] = pMem[i] | bf.pMem[i];
        }
        return A;
    }

}

TBitField TBitField::operator&(const TBitField &bf) { // операция "и"
    if (bf.GetLength() > BitLen) {
        TBitField A(bf.GetLength());
        for (int i = 0; i< BitLen; i++){
            A.pMem[i] = pMem[i];
        }
        for(int i = 0; i< bf.BitLen; i++){
            A.pMem[i] &= bf.pMem[i];
        }
        return A;
    }
    else if (bf.GetLength() < BitLen) {
        TBitField A(GetLength());
        for (int i = 0; i< bf.BitLen; i++){
            A.pMem[i] = bf.pMem[i];
        }
        for(int i = 0; i< BitLen; i++){
            A.pMem[i] &= pMem[i];
        }
        return A;
    }
    else {
        TBitField A(GetLength());
        for (int i = 0; i < (BitLen); i++) {
            A.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return A;
    }
}

TBitField TBitField::operator~(void) { // отрицание
    TBitField A(GetLength());
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) != 0){
            A.SetBit(i);
        }
    }
    return A;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) { // ввод
    int n, t, ind;
    cout << "Enter a quanity of elements";
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> t;
        if (t > bf.GetLength()) {
            cout << "Error";
        } else {
            ind = t / (8 * sizeof(TELEM));
            bf.pMem[ind] | 1 << (n % (8 * sizeof(TELEM)));
        }
    }
    return istr;
}


ostream &operator<<(ostream &ostr, const TBitField &bf) { // вывод
    int ind;
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i) != 0) {
            ostr << 1 << " ";
        }
        else
            ostr << 0 << " ";

    }
    return ostr;
}
