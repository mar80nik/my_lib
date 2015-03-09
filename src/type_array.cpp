#include <stdafx.h>
#include "type_array.h"

void DoubleArray::Serialize(CArchive& ar)
{
	int i, n; double t;
	if(ar.IsStoring())
	{
		n = GetSize(); ar << n; 
		for(i = 0; i < n; i++) { t = operator[](i); ar << t; }
	}
	else
	{		
		RemoveAll(); ar >> n; 
		for(i = 0; i < n; i++) { ar >> t; Add(t); }
	}
}