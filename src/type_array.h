#pragma once

template<class type>
class TypeArray: public CArray<type>
{
public:
	TypeArray()	{}
	TypeArray(const TypeArray& ref)	{ RemoveAll(); Copy(ref); }
	TypeArray& operator << (const type &d) {(*this).Add(d); return (*this);}
	TypeArray& operator =(const TypeArray& arr)
	{
		RemoveAll(); Copy(arr);
		return *this;
	}
	operator type*() {return CArray<type>::GetData();}
	BOOL operator==(const TypeArray &ref)
	{
		if (GetSize() != ref.GetSize()) return FALSE;
		for (int i = 0; i < GetSize(); i++)
		{
			if (operator[](i) != ref[i]) return FALSE;
		}
		return TRUE;
	}
	type get(int n) const
	{
		if (n < GetSize())
		{
			return operator[](n);
		}
		else
		{
			return 0;
		}
	}
	BOOL HasValues() const {return (GetSize() != 0);}
};

class DoubleArray: public TypeArray<double>
{
public:
	DoubleArray() {};
	DoubleArray(const DoubleArray& ref): TypeArray<double>(ref) {};
	virtual void Serialize(CArchive& ar);
};