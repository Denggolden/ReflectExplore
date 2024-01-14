#include "ReflexBase.h"

#include "WorkClass.h"

void ReflexObjInit::InitObjFactory()
{
	WorkClass1* ppppWorkClass1 = new WorkClass1();

	//if (has_member_f1<WorkClass1>::value) {
	//	std::cout << "has_member_f1<WorkClass1>::value" << std::endl;
	//}
	//else
	//{
	//	std::cout << "！ has_member_f1<WorkClass1>::value" << std::endl;
	//}

	registClass111<WorkClass1>("WorkClass1_1", ppppWorkClass1);
	registClass1<WorkClass1,const MsgInfo&>("WorkClass1_1", ppppWorkClass1);
	registClass11<WorkClass1, const MsgInfo&>("WorkClass1_1", ppppWorkClass1);
	//CreateObj<WorkClass1>("WorkClass1_1");//默认构造   类名为：WorkClass1-1(下面依次类推)
	//CreateObj<WorkClass1>("WorkClass1_2");//默认构造   类名为：WorkClass1-2(下面依次类推)
	//CreateObj<WorkClass1,int>("WorkClass1_3",123);//有参构造   类名为：WorkClass1-3(下面依次类推)

	CREATETYPE(WorkClass1)CREATEPARA(WorkClass1_1);
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

void fgg(std::string str) {

};

void ReflexObjInit::Test()
{
	//Invok<WorkClass1, void>("WorkClass1_1","InitClass");

	CallFunEn(WorkClass1,WorkClass1_1,void, InitClass);

	InvokInitClass<WorkClass1, void>("WorkClass1_1");
}
