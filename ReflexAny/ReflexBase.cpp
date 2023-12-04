#include "ReflexBase.h"

#include "WorkClass.h"

void ReflexObjInit::InitObj()
{
	CreateObj<WorkClass>("WorkClass1");//默认构造   类名为：WorkClass1
	CreateObj<WorkClass>("WorkClass2");//默认构造   类名为：WorkClass2
	CreateObj<WorkClass,int>("WorkClass3",123);//有参构造   类名为：WorkClass3
}
