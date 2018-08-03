#ifndef HDBTOOLCALLBACK_H
#define HDBTOOLCALLBACK_H
#include "publicdata.h"
/******************************************定义回调函数********************************/
#include "hruleeditapi.h"
//公式类的回调函数
bool __cdecl formulaCallback(int nMsgType,HWPARAM wParam,HLPARAM lParam,int nDBID);
bool __cdecl ruleCallback (int msgType,RULEINFO *ruleParam);
bool __cdecl pluginCallback(int nMsgType,HWPARAM wParam,HLPARAM lParam);
#endif // HDBTOOLCALLBACK_H
