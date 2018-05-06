#ifndef HDBTOOLCALLBACK_H
#define HDBTOOLCALLBACK_H
/******************************************定义回调函数********************************/
typedef unsigned int WPARAM;
typedef long LPARAM;
#include "hruleeditapi.h"
//公式类的回调函数
bool __cdecl formulaCallback(int nMsgType,WPARAM wParam,LPARAM lParam,int nDBID);
bool __cdecl ruleCallback (int msgType,RULEPARAM *ruleParam);
#endif // HDBTOOLCALLBACK_H
