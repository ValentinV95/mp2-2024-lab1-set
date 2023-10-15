#include "tset.h"

TSet::TSet(int mp): BitField(mp) 
{
	MaxPower = mp;
};
TSet::TSet(const TSet& s): BitField(s.BitField){
	MaxPower = s.MaxPower;
};
TSet::TSet(const TBitField& bf): BitField(bf){
	MaxPower = bf.GetLength();
};
TSet::operator TBitField() {
	TBitField bf = BitField;
	return bf;
};
int TSet::GetMaxPower(void) const {
	return MaxPower;
};
int TSet::IsMember(const int Elem) const {
	if (Elem < MaxPower) if (BitField.GetBit(Elem)) return 1;
	return 0;
};
void TSet::InsElem(const int Elem) {
	if ((Elem>=0)&&(Elem < MaxPower)) BitField.SetBit(Elem);
	else throw out_of_range("Element must belong to the universe of the set");
};
void TSet::DelElem(const int Elem) {
	if ((Elem >= 0) && (Elem < MaxPower)) BitField.ClrBit(Elem);
	else throw out_of_range("Element must belong to the universe of the set");
};
TSet& TSet::operator=(const TSet& s) {
	if (s != *this) {
		BitField = s.BitField;
		MaxPower = s.MaxPower;
	}
	return *this;
};
int TSet::operator== (const TSet& s) const {
	return BitField == s.BitField;
};
int TSet::operator!= (const TSet& s) const {
	return  BitField != s.BitField;
};
TSet TSet:: operator+ (const TSet& s) {
	TSet S(max(s.MaxPower, MaxPower));
	S.BitField = BitField | s.BitField;
	return S;
};
TSet TSet::operator+ (const int Elem) {
	TSet S(*this);
	S.InsElem(Elem);
	return S;
};
TSet TSet::operator- (const int Elem) {
	TSet S(*this);
	S.DelElem(Elem);
	return S;
};
TSet TSet::operator* (const TSet& s) {
	TSet S(max(s.MaxPower, MaxPower));
	S.BitField = BitField & s.BitField;
	return S;
};
TSet TSet::operator~ (void) {
	TSet s(MaxPower);
	s.BitField = ~BitField;
	return s;
};
istream& operator>>(istream& istr, TSet& s) {
	int tmp=0;
	istr >> tmp;
	while ((tmp >= 0) && (tmp < s.MaxPower))
	{
		s.InsElem(tmp);
		istr >> tmp;
	}
	return istr;
};
ostream& operator<<(ostream& ostr, const TSet& s) {
	for (int i = 0; i < s.MaxPower; i++) 
		if ((s.BitField).GetBit(i)) 
			ostr << (s.BitField).GetBit(i) * i << " ";
	return ostr;
};