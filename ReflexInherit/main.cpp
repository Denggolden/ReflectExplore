#include "ReflexBase.h"
#include "WorkClass.h"

#include <sstream>

typedef struct CusData {
	CusData() {};
	CusData(int intValue, std::string str) { IntValue = intValue; Str = str; };
	int IntValue;
	std::string Str;
}CusData;

typedef struct CusDataInfo {
	CusDataInfo() {};
	CusDataInfo(CusData cusData, std::string str) { CusData = cusData; Str = str; };
	CusData CusData;
	std::string Str;
}CusDataInfo;

class WorkThrid :public CBase
{
public:
	WorkThrid() {};
	~WorkThrid() {};
public:
	void signalFun() {
		std::cout << "void signalFun()" << std::endl;
		void* pBufferArr[] = {nullptr };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)()>(&WorkThrid::signalFun), pBufferArr);
	};
	void signalFun(int a) { 
		std::cout << "void signalFun(int a): " << a<<std::endl; 
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(int)>(&WorkThrid::signalFun), pBufferArr);
	};
	void signalFun(std::string str) {
		std::cout << "void signalFun(std::string str): " << str << std::endl;
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&str)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(std::string)>(&WorkThrid::signalFun), pBufferArr);
	};

	void signalFun(CusData cusData) {
		std::cout << "void signalFun(CusData cusData): " << cusData.IntValue<<"  "<< cusData.Str<< std::endl;
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&cusData)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(CusData)>(&WorkThrid::signalFun), pBufferArr);
	};
	void signalFun(CusDataInfo cusDataInfo) {
		std::cout << "void signalFun(CusDataInfo cusDataInfo): " << cusDataInfo.Str<<"  " << cusDataInfo.CusData.IntValue << "  " << cusDataInfo.CusData.Str << std::endl;
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&cusDataInfo)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(CusDataInfo)>(&WorkThrid::signalFun), pBufferArr);
	};

	void signalFun(int a, std::string str) {
		std::cout << "void signalFun(int a, std::string str): " << a<<"   " << str << std::endl;
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)), const_cast<void*>(reinterpret_cast<const void*>(&str)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
		MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(int, std::string)>(&WorkThrid::signalFun), pBufferArr);
	};

	//CREATESIGNAL(WorkThrid,signalFun,float,f)
};

class WorkFour :public CBase
{
public:
	WorkFour() {};
	~WorkFour() {};
public:
	void slotFun() {std::cout << "void slotFun()" << std::endl;};
	void slotFun(int a) { std::cout << "void slotFun(int a): "<< a<<std::endl; };
	void slotFun(std::string str) { std::cout << "void slotFun(std::string str): " <<str<< std::endl; };
	void slotFun(CusData cusData) {
		std::cout << "void slotFun(CusData cusData): " << cusData.IntValue << "  " << cusData.Str << std::endl;
	}
	void slotFun(CusDataInfo cusDataInfo) {
		std::cout << "void slotFun(CusDataInfo cusDataInfo): " << cusDataInfo.Str << "  " << cusDataInfo.CusData.IntValue << "  " << cusDataInfo.CusData.Str << std::endl;
	}
};

//初始化所有所需对象
void InitAllObj();

int main()
{
	//SInvokk sInvokk;
	//sInvokk.Invoke<SM::SFun1, void>();
	//sInvokk.Invoke<SM::SFun2, void, int>(1);
	//sInvokk.Invoke<SM::SFun3, int>();
	//std::cout<< sInvokk.Invoke<SM::SFun4, int, int>(1)<<std::endl;

	//InitAllObj();
	////访问指定对象 根据类及类名
	//WorkClass1* pWorkClass1_1 = ObjFactory::getIns()->getClass<WorkClass1>("WorkClass1_1");
	////pWorkClass1->seta(100);
	////std::cout << "pWorkClass1->geta(): "<< pWorkClass1->geta() <<std::endl;
	////pWorkClass1->setsTa(ST(3,"1233"));
	////ST st = pWorkClass1->getsTa();
	//ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type0, "msg---info"));
	//ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type1, "msg---info"));
	////ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type2, "msg---info"));

	//WorkOne* pWorkOne = new WorkOne();
	//WorkTwo* pWorkTwo = new WorkTwo();
	//WorkTwo* pWorkTwo1 = new WorkTwo();

	//MetaObj::getIns()->Connect(pWorkOne, signal(signalFun()), pWorkTwo, slots(slotFun()));
	//MetaObj::getIns()->Connect(pWorkOne, signal(signalFun(int)), pWorkTwo, slots(slotFun(int)));
	//MetaObj::getIns()->Connect(pWorkOne, signal(signalFun(int)), pWorkTwo1, slots(slotFun(int)));
	//MetaObj::getIns()->Connect(pWorkTwo, signal(signalFun(int,std::string)), pWorkOne, slots(slotFun(int,std::string)));
	////pWorkOne->signalFun();
	//pWorkOne->signalFun(123);
	//pWorkTwo->signalFun(123,"string");

	WorkThrid* pWorkThrid = new WorkThrid();
	WorkThrid* pWorkThrid1 = new WorkThrid();
	WorkFour* pWorkFour = new WorkFour();

	//Connect(pWorkThrid, &WorkThrid::signalFun,pWorkFour ,&WorkFour::slotFun);

	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)()>(&WorkThrid::signalFun),
		pWorkThrid1,[&]() {
			std::cout << "[&]()" << std::endl;
		});
	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)()>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)()>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)(int)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(int)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)(int)>(&WorkThrid::signalFun),
		pWorkThrid, [&](int a) {
			std::cout << "[&](int a):"<<a<<std::endl;
		});

	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)(std::string)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(std::string)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid, static_cast<void (WorkThrid::*)(std::string)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(std::string)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid1, static_cast<void (WorkThrid::*)(std::string)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(std::string)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid1, static_cast<void (WorkThrid::*)(CusData)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(CusData)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid1, static_cast<void (WorkThrid::*)(CusDataInfo)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(CusDataInfo)>(&WorkFour::slotFun));

	MetaObj::getIns()->Connect(pWorkThrid1, static_cast<void (WorkThrid::*)(int, std::string)>(&WorkThrid::signalFun),
		pWorkThrid1, [&](int a,std::string str) {
			std::cout << "[&](int a,std::string str):" << a<<"   "<<str << std::endl;
		});

	pWorkThrid->signalFun();
	pWorkThrid->signalFun(369);
	pWorkThrid->signalFun("OppVp");

	std::cout << "===========================pWorkThrid1=================================" << std::endl;
	pWorkThrid1->signalFun();
	pWorkThrid1->signalFun(963);
	pWorkThrid1->signalFun("VIP");
	pWorkThrid1->signalFun(CusData(10098,"123Oppu"));
	pWorkThrid1->signalFun(CusDataInfo(CusData(10098, "123Oppu"),"pkvs"));
	std::cout << "===========================DisConnectImpl=================================" << std::endl;
	void* pBufferArr[] = { nullptr };
	MetaObj::getIns()->DisConnectImpl(pWorkThrid1, static_cast<void (WorkThrid::*)(CusDataInfo)>(&WorkThrid::signalFun), pBufferArr);
	pWorkThrid1->signalFun("VIP");
	pWorkThrid1->signalFun(CusData(10098, "123Oppu"));
	pWorkThrid1->signalFun(CusDataInfo(CusData(10098, "123Oppu"), "pkvs"));

	std::cout << "===========================Connect=================================" << std::endl;
	MetaObj::getIns()->Connect(pWorkThrid1, static_cast<void (WorkThrid::*)(CusDataInfo)>(&WorkThrid::signalFun),
		pWorkFour, static_cast<void (WorkFour::*)(CusDataInfo)>(&WorkFour::slotFun));

	pWorkThrid1->signalFun("VIP");
	pWorkThrid1->signalFun(CusData(10098, "123Oppu"));
	pWorkThrid1->signalFun(CusDataInfo(CusData(10098, "123Oppu"), "pkvs"));

	pWorkThrid1->signalFun(1024,"Happy!!!!");

	////对于非虚函数，返回其在内存的真实地址
	//printf("A::set(): %p\n", static_cast<void (WorkThrid::*)()>(&WorkThrid::signalFun));
	//对于虚函数， 返回其在虚函数表的偏移位置
	//printf("B::print(): %p\n", static_cast<void (WorkThrid::*)()>(&WorkThrid::print));

  //   WorkFour* pWorkFour = new WorkFour();
	 //PtrBase* pPtrBase = new PtrSlot0(pWorkFour,
		// static_cast<void (WorkFour::*)()>(&WorkFour::slotFun));

	return 0;
}

void InitAllObj()
{
	ReflexObjInit reflexObjInit;
	reflexObjInit.InitObjFactory();
	//reflexObjInit.InitObjNotify();
	reflexObjInit.Test();
}
