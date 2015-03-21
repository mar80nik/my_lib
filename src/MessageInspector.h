#pragma once

#include "my_lib_globals.h"
#include "ProtectedObject.h"

struct WindowAddress
{
	CWinThread	*pThrd;
	CWnd		*pWND;
	WindowAddress() {pThrd=AfxGetThread();;pWND=NULL;}
	bool operator==(WindowAddress& t) { return (pWND==t.pWND && pThrd==t.pThrd); }
};

class MessagesInspectorSubject
{
	friend class MessagesInspector;
protected:
	static int PIDnum; int PID;
	int ClassID; 
	MessagesInspectorSubject* Prev,*Next;
	MessagesInspector* Inspector;
public:
	CString Name; 
	static MessagesInspector* GlobalInspector;
protected:
	MessagesInspectorSubject();
public:
	virtual ~MessagesInspectorSubject();		
	virtual MessagesInspectorSubject& operator=(MessagesInspectorSubject&) {return (*this);}
	int GetPID() {return PID;}
	virtual BOOL ValidateClass(int classID);
	int GetClass() {return ClassID;};
	BOOL IsRegistered() {return (Inspector!=NULL);}
};

//---------------------------------------
class MessagesInspector
{	
	MessagesInspectorSubject NullMsg;	
public:
	CString Name;
	ProtectedObjectX<int> counter; 

	MessagesInspector();
	int RegisterMsg(MessagesInspectorSubject *);
	int CleanUp();
	~MessagesInspector();;
};
//---------------------------------------
class AbstractMessage: public MessagesInspectorSubject
{
public:
	CTime			Time; 
	CString			Sender, Subject, Body;
	AbstractMessage() 
	{
		Time=CTime::GetCurrentTime();
		GlobalInspector->RegisterMsg(this);
	}
	virtual ~AbstractMessage() 
	{
		int a=5; a++;
	}
	virtual CString Format();
};

class MessageForWindow: public AbstractMessage
{
protected:
	void DispatchMsg(void *t);
public:
	WindowAddress	Reciver;
	UINT			Msg;
	WPARAM			wParam;

	MessageForWindow() {Msg=0; wParam=0; ClassID=WINDOW_MSG_TYPE;}
	MessageForWindow(UINT m, WindowAddress r) {Msg=m; Reciver=r;}
	virtual ~MessageForWindow() 
	{
		int a=5; a++;
	};
	virtual void Dispatch();
	bool operator==(MessageForWindow& t) { return (Msg==t.Msg && Reciver==t.Reciver ); }
};

class AbstractMessagesArray: public CArray<AbstractMessage*> 
{
protected:
	AbstractMessagesArray() {}
	virtual ~AbstractMessagesArray() {RemoveAll();}
	virtual void RemoveAll() 
	{
		for(int i=0;i<GetSize();i++) delete operator[](i);
		CArray<AbstractMessage*>::RemoveAll();
	}
};

class UniformMessagesArray: public MessageForWindow, public AbstractMessagesArray
{
public:
	UniformMessagesArray() {Name="MessagesArray"; ClassID=MSG_ARRAY_TYPE; }
	virtual ~UniformMessagesArray() {RemoveAll();};
	virtual int Add(MessageForWindow* t)
	{
		int ret=-1;
		ASSERT(t->ValidateClass(WINDOW_MSG_TYPE));		
		if(GetSize()==0) 
		{
			(*(MessageForWindow*)this)=(*t); 
			ret=AbstractMessagesArray::Add(t);
		}
		else
		{
            if( *t==*this )
			{
				ret=AbstractMessagesArray::Add(t);
			}
			else ASSERT(0);
		}
		return ret;
	}
	UniformMessagesArray& operator << (MessageForWindow* t) {Add(t); return *this;}
	virtual void RemoveAll() 
	{
		for(int i=0;i<GetSize();i++) delete operator[](i);
        Msg=0; Reciver=WindowAddress(); wParam=0;
	}
};

class ErrorsArray: public MessageForWindow, public AbstractMessagesArray
{
	friend class ErrorInspector;
protected:
	int err_num, warn_num;
	virtual void RemoveAll() {err_num=warn_num=0; return AbstractMessagesArray::RemoveAll();}
public:	
	ErrorsArray() 
	{
		err_num=warn_num=0; Name="ErrorsArray"; ClassID=ERR_ARR_MSG_TYPE;
	};
	virtual int Add(AbstractMessage* t);
	virtual ~ErrorsArray() 	{}
};

enum MessagePriorities {lmprLOW = 0, lmprHIGH = 78};

class LogMessage: public MessageForWindow 
{	
public:
	static WindowAddress LogWindow;
public:	
	enum MessagePriorities {low_pr=0, high_pr=78};

	int priority;
	CStringArray Msgs;

	LogMessage* CreateEntry(const CString& obj, const CString Message=CString(""),int _priority=low_pr);
	LogMessage* CreateEntry(ErrorsArray* err);
	LogMessage& operator <<(const CString &Message);
	void SetPriority(const LogMessage::MessagePriorities &pr) {priority = pr;}
	void SetPriority(const ::MessagePriorities &pr) {priority = pr;}
	LogMessage(::MessagePriorities prio = ::lmprLOW);
	int GetSize() {return (int)(Msgs.GetSize());}
	BOOL HasMessages() {return (GetSize() != 0);}
};

class ControledLogMessage
{
protected:
	LogMessage* msg; 
public:
	CString T;

	ControledLogMessage(::MessagePriorities prio = ::lmprLOW)	
	{msg = NULL;	msg = new LogMessage(prio);}
	~ControledLogMessage()	
	{if (msg != NULL) delete msg;}	
	virtual void Dispatch()	
	{
		if (msg->HasMessages())
		{
			msg->Dispatch(); msg = NULL;
		}
		else
		{
			delete msg; msg = NULL;
		}
	}
	LogMessage& operator <<(const CString &Message)
	{return (*msg) << Message;}
	void SetPriority(const ::MessagePriorities &pr) 
	{msg->SetPriority(pr);}
	int GetSize() const
	{msg->GetSize();}
	BOOL HasMessages() const
	{msg->HasMessages();}
};