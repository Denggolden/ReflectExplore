#include "ReflexBase.h"

#include "WorkClass.h"

void ReflexObjInit::InitObj()
{
	CreateObj<WorkClass>("WorkClass1");//Ĭ�Ϲ���   ����Ϊ��WorkClass1
	CreateObj<WorkClass>("WorkClass2");//Ĭ�Ϲ���   ����Ϊ��WorkClass2
	CreateObj<WorkClass,int>("WorkClass3",123);//�вι���   ����Ϊ��WorkClass3
}
