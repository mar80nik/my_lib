// my_lib.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "my_lib.h"
#include "MessageInspector.h"
#include "MyThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WindowAddress MyThread::ConfigParentWindow = WindowAddress();
WindowAddress LogMessage::LogWindow = WindowAddress();
MessagesInspector* MessagesInspectorSubject::GlobalInspector = NULL;

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// Cmy_libApp

BEGIN_MESSAGE_MAP(Cmy_libApp, CWinApp)
END_MESSAGE_MAP()


// Cmy_libApp construction

Cmy_libApp::Cmy_libApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cmy_libApp object

Cmy_libApp theApp;


// Cmy_libApp initialization

BOOL Cmy_libApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
