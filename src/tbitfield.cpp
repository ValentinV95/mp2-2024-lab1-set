#include "tbitfield.h"

TBitField::TBitField(int len) {
	if (len > 0) {
		if (len % (sizeof(TELEM)*8)) MemLen = len / (sizeof(TELEM)*8) + 1;
		else MemLen = len / (sizeof(TELEM)*8);
		BitLen = len;
		pMem = new TELEM[MemLen];
		memset(pMem, 0, MemLen* sizeof(TELEM));
	}
	else throw std::out_of_range("Size must be pozitive");
};
TBitField::TBitField(const TBitField& bf) {
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
};
TBitField:: ~TBitField() {
	delete[] pMem;
};
int TBitField::GetMemIndex(const int n) const {
	return (n / (8 * sizeof(TELEM)));
};
TELEM TBitField::GetMemMask(const int n) const {
	return 1 << (n % (8 * sizeof(TELEM)));
};
int TBitField::GetLength(void) const {
	return BitLen;
};
void TBitField::SetBit(const int n) {
	if ((n>=0)&&(n < BitLen)) pMem[GetMemIndex(n)] |= GetMemMask(n);
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
};
void TBitField::ClrBit(const int n) {
	if ((n >= 0) && (n < BitLen)) pMem[GetMemIndex(n)] &= ~GetMemMask(n);
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
};
int TBitField::GetBit(const int n) const {
	if ((n >= 0) && (n < BitLen)) {
		if (pMem[GetMemIndex(n)] & GetMemMask(n)) return 1;
		else return 0;
	} 
	else throw std::out_of_range("Index must be at least 0, and not exceed the size");
};
TBitField& TBitField:: operator=(const TBitField& bf) {
	if (*this != bf) if (MemLen != bf.MemLen) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) pMem[i] = bf.pMem[i];
	}
	else {
		memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
		BitLen = bf.BitLen;
	}
	return *this;
};
int TBitField:: operator==(const TBitField& bf) const {
	if (BitLen == bf.BitLen) {
		for (int i = 0; i < MemLen; i++) {
			if (pMem[i] != bf.pMem[i]) return 0;
		}
		return 1;
	}
	return 0;
};
int TBitField:: operator!=(const TBitField& bf) const {
	return (0== (*this == bf));
};
TBitField TBitField:: operator|(const TBitField& bf) {
	if (bf.BitLen > BitLen) {
		TBitField bf2(bf);
		for (int i = 0; i < MemLen; i++) bf2.pMem[i] |= pMem[i];
		return bf2;
	}
	else {
		TBitField bf2(*this);
		for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] |= bf.pMem[i];
		return bf2;
	};
};
TBitField TBitField::operator&(const TBitField& bf) {
	if (bf.BitLen > BitLen) {
		TBitField bf2(bf);
		for (int i = 0; i < MemLen; i++) bf2.pMem[i] &= pMem[i];
		return bf2;
	}
	else {
		TBitField bf2(*this);
		for (int i = 0; i < bf.MemLen; i++) bf2.pMem[i] &= bf.pMem[i];
		return bf2;
	}
};
TBitField TBitField:: operator~(void) {
	TBitField bf(BitLen);
	for (int i = 0; i < MemLen; i++) bf.pMem[i] = ~pMem[i];
	for (int i= BitLen; i<MemLen*sizeof(TELEM)*8; i++) bf.pMem[GetMemIndex(i)] &= ~GetMemMask(i);
	return bf;
};
istream& operator>>(istream& istr, TBitField& bf) {
	int tmp;
	for (int i = 0; i < bf.BitLen; i++) {
		istr >> tmp;
		if (tmp) bf.SetBit(i);
		else bf.ClrBit(i);
	};
	return istr;
};
ostream& operator<<(ostream& ostr, const TBitField& bf) {
	for (int i = 0; i < bf.BitLen; i++) {
		if (bf.pMem[bf.GetMemIndex(i)] & bf.GetMemMask(i)) ostr << 1;
		else ostr << 0;
	};
	return ostr;
};