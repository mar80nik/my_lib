// my_lib.h : main header file for the my_lib DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cmy_libApp
// See my_lib.cpp for the implementation of this class
//

class Cmy_libApp : public CWinApp
{
public:
	Cmy_libApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
