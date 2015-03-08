#pragma once

UINT StartThreadFunction( LPVOID pParam );
UINT StopThreadFunction( LPVOID pParam );

#include "globals.h"
#include "mytime.h"
#include "MessageInspector.h"
#include <afxmt.h>
#include <afxtempl.h>
#include <cstringt.h>
#include <afxcoll.h> 

class ResourceMSG: public AbstractMessage
{
protected:
	DWORD Body_ID;
public:
	ResourceMSG() {Body_ID=0; ClassID=ResourceMSG_TYPE; Body=CString("look for ID");}
	ResourceMSG* Create(DWORD _body_ID, CString& _sender, HWND _reciver=NULL, CString& _subj=CString("no subject"))
	{
		Body_ID=_body_ID;
		return this;
	};
	operator DWORD() {return Body_ID;}
	virtual ResourceMSG& operator=(const ResourceMSG& T);
};

class ErrorMessage: public AbstractMessage
{
public:
	ErrorMessage():AbstractMessage() {ClassID=ErrorMSG_TYPE; }
	ErrorMessage* Create(CString& _sender, CString& _body, CString& _subj=CString("Error"))
	{
		Sender=_sender; Body=_body; Subject=_subj;
		return this;
	}		
};

class Error: public ResourceMSG
{
public:
	Error():ResourceMSG() {ClassID=ErrorResMSG_TYPE; }
	virtual CString Format();
};

class Warning: public Error
{
public:
	Warning():Error() {ClassID=WarningResMSG_TYPE; }
};

//**************************************************
//**************************************************

#define NO_ERRORS 0

class ErrorInspector
{
protected:
	CList<ErrorsArray*> ErrArr;
public:
	ErrorInspector() {}
	~ErrorInspector();
	void RegisterError(AbstractMessage* _err);
	int Errors();
	int Warnings();
	ErrorsArray* GetErrorsArray();
	void ResetErrors();
	void AttachErrors(ErrorsArray* src) ;
	void AttachErrorsOF(ErrorInspector* src) ;
	ErrorsArray* DetachErrors();
	int DispatchErrors();
};


enum MyThreadMessages {UM_STOP=WM_USER + 76, UM_PAUSE, UM_CONTINUE, UM_START, UM_EXIT_THREAD,UM_EVENTLOG};
enum StopState {None=0, ToDo, Done};

class MyThread;
struct StartParams: public AbstractMessage
{
public:	
	WPARAM wParam; LPARAM lParam; CWinThread *src, *dest;

	StartParams() {wParam=lParam=0; src=dest=NULL; ClassID=START_PARAMS_MSG_TYPE; };
	StartParams* Create(WPARAM wP, LPARAM lP, CWinThread* _dest)
	{
		wParam=wP; lParam=lP; dest=_dest; 
		src=AfxGetThread(); Name="StartParams";
		return this;
	};
	virtual ~StartParams() {}
};

struct StopParams: public AbstractMessage
{
public:
	MyThread* T;	

	StopParams(MyThread* _T=0) {T=_T; Name="StopParams"; ClassID=STOP_PARAMS_MSG_TYPE; };
	virtual ~StopParams() {}

};

/////////////////////////////////////////////////////////////////////////////

class MyThread : public CWinThread, public ErrorInspector
{
	friend UINT StartThreadFunction( LPVOID pParam);
	friend UINT StopThreadFunction( LPVOID pParam);

	DECLARE_DYNCREATE(MyThread)		

	enum _ThreadStatus {STOPED=0, PAUSED, RUNNING, THRD_ERROR};
	enum _ThreadState {Suspended, Resumed};	

	struct ThreadConfig
	{
		friend class MyThread;
	protected:
		_ThreadStatus ThreadStatus;
		_ThreadState ThreadState;	
		StopState Stop, Terminate;
	public:
		int PersonalID;
		CString Name;
		bool UpdateParent;
		bool NotifyUser;
		CString ActionDescription;
		CString PauseComment;
		WindowAddress ParentWindow;

		ThreadConfig();
		_ThreadStatus GetStatus() {return ThreadStatus;};
		StopState GetStop() {return Stop;}
		StopState GetTerminate() {return Terminate;}
		void SetTerminate(StopState t) {Terminate=t;}
		ThreadConfig& operator=(ThreadConfig&);
	};
	struct ParentStatusReport: public AbstractMessage
	{
	public:
		ThreadConfig ConfigCopy;
		ParentStatusReport(ThreadConfig&);
	};

private:
	CEvent *TerminateEvent;	
	CEvent RunEvent;

	void MyResume();
	void MySuspend();	
protected:
	static WindowAddress ConfigParentWindow;

	StartParams startParam1;
	CWinThread* Parent;
	int lastID;		
	ThreadConfig LastConfig;

	virtual void BeforeExecute(WPARAM wParam, LPARAM lParam);
	virtual void Main(WPARAM wParam, LPARAM lParam) {};	
	virtual void AfterExecute();	
public:			
	MyTimer Timer1;  int	WAIT_TIME;
	int pass_In, pass_Out;
	ThreadConfig Config;
	
	MyThread();           // protected constructor used by dynamic creation	
	virtual void Create(CWinThread* parent,int ID, CEvent* evnt);	
	virtual void ContinueThread();	
	virtual void PauseThread();	
	virtual void StartThread(WPARAM,LPARAM);
	virtual void StopThread(bool Wait);	
	virtual int TerminateThread();
	virtual ~MyThread();
	virtual bool PostParentMessage(UINT msg, MessagesInspectorSubject* lParam=NULL);
	virtual int RegisterMsg(MessagesInspectorSubject *);
	CEvent* GetTerminateEvent() {return TerminateEvent;}
	ParentStatusReport* GetParentReport();
	void UpdateParent();
	ThreadConfig ChangeConfig(ThreadConfig cfg);
	void RestoreLastConfig(MyThread::ThreadConfig* cfg=NULL);

	friend CString GetString(enum MyThread::_ThreadState a);
	friend CString GetString(enum MyThread::_ThreadStatus a);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Implementation
	//{{AFX_MSG(MyThread)	
	afx_msg void OnExitThread(WPARAM wParam, LPARAM lParam );	
	afx_msg void OnStart(WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


template<typename T>
class WorkThread: public MyThread
{
public:
	T params;
	WorkThread(int id)
	{
		MyThread::Create(AfxGetApp(),id,NULL);
	};
	~WorkThread(){};

	static UINT proc(void* param);
	virtual HRESULT Start()
	{
		CWinThread *ret;
		ret=AfxBeginThread(proc,(void*)this);		
		return S_OK;
	}
};