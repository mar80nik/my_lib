#pragma once

#include <afxmt.h>
#include <afxtempl.h>
#include <cstringt.h>
#include <afxcoll.h> 
#include <afxmt.h>

#include "MyTime.h"

#define IS ==
#define IS_NOT !=

#define REGISTER_OK 0
#define REGISTER_ERROR -1;
#define REGISTER_ERROR_THREAD_TERMINATE -2
#define REGISTER_ERROR_NULL_MSG -3

#define AssistantID 123456

#define UM_GENERIC_MESSAGE 12000

enum {ERR_MIN=50000, ERR_MAX=59999, WRN_MIN, WRN_MAX=70000};

enum {
	MSG_ARRAY_TYPE=0xaaaaaaaa, ResourceMSG_TYPE=MSG_ARRAY_TYPE, ErrorMSG_TYPE, ErrorResMSG_TYPE, WarningResMSG_TYPE, 
	LOG_MSG_TYPE, START_PARAMS_MSG_TYPE, STOP_PARAMS_MSG_TYPE, ERR_ARR_MSG_TYPE, 
	PARENT_REPORT_MSG, MYTHREAD_MSG_LAST_TYPE,
	GENERIC_MSG_MSG_TYPE, WINDOW_MSG_TYPE
};

struct PerfomanceStaff
{
	MyTimer Timer1;
	bool LogEvents;
	int pass_num;

	PerfomanceStaff() {LogEvents=false;pass_num=0;}	
};