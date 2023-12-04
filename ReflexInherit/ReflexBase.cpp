#include "ReflexBase.h"

#include "WorkClass.h"

void ReflexObjInit::InitObjFactory()
{
	//CreateObj<WorkClass1>("WorkClass1_1");//Ĭ�Ϲ���   ����Ϊ��WorkClass1-1(������������)
	//CreateObj<WorkClass1>("WorkClass1_2");//Ĭ�Ϲ���   ����Ϊ��WorkClass1-2(������������)
	//CreateObj<WorkClass1,int>("WorkClass1_3",123);//�вι���   ����Ϊ��WorkClass1-3(������������)

	CREATETYPE(WorkClass1) CREATEPARA(WorkClass1_1);
	//CREATETYPE(WorkClass1) CREATEPARA(WorkClass1_2);
	//CREATETYPE(WorkClass1, int) CREATEPARA(WorkClass1_3, 123);

	CREATETYPE(WorkClass2) CREATEPARA(WorkClass2_1);
	//CREATETYPE(WorkClass2) CREATEPARA(WorkClass2_2);
	//CREATETYPE(WorkClass2, std::string) CREATEPARA(WorkClass2_3, "456");

	//ObjFactory::getIns()->initObjClass();
	OBJINITCLASS;
}

void ReflexObjInit::InitObjNotify()
{
	WorkClass1* pWorkClass1_1 = ObjFactory::getIns()->getClass<WorkClass1>("WorkClass1_1");
	ObjNotify::getIns()->subscribeInfo(pWorkClass1_1, MsgType::Type0);
	ObjNotify::getIns()->subscribeInfo(pWorkClass1_1, MsgType::Type1);
	//ObjNotify::getIns()->subscribeInfo(pWorkClass1_1, MsgType::Type2);

	WorkClass2* pWorkClass2_1 = ObjFactory::getIns()->getClass<WorkClass2>("WorkClass2_1");
	ObjNotify::getIns()->subscribeInfo(pWorkClass2_1, MsgType::Type0);
	//ObjNotify::getIns()->subscribeInfo(pWorkClass2_1, MsgType::Type1);
}