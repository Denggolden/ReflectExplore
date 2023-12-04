#include"ReflexBase.h"
#include "WorkClass.h"

//��ʼ�������������
void InitAllObj();

int main()
{
	InitAllObj();
	//����ָ������ �����༰����
	WorkClass1* pWorkClass1_1 = ObjFactory::getIns()->getClass<WorkClass1>("WorkClass1_1");
	//pWorkClass1->seta(100);
	//std::cout << "pWorkClass1->geta(): "<< pWorkClass1->geta() <<std::endl;
	//pWorkClass1->setsTa(ST(3,"1233"));
	//ST st = pWorkClass1->getsTa();
	ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type0, "msg---info"));
	ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type1, "msg---info"));
	//ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type2, "msg---info"));

	return 0;
}

void InitAllObj()
{
	ReflexObjInit reflexObjInit;
	reflexObjInit.InitObjFactory();
	reflexObjInit.InitObjNotify();
}
