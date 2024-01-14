#pragma once

#include "ReflexBase.h"
#include <vector>

class WorkClass1 :public CBase
{
public:
	WorkClass1();
	WorkClass1(int para);
	~WorkClass1();

	CreateMemVar1(int, A, 10);
	CreateMemVar2(std::vector<int>, vec);

public:
	void InitClass() override;
	void UpDateClass(const MsgInfo& msgInfo) override;
};

class WorkClass2 :public CBase
{
public:
	WorkClass2();
	WorkClass2(std::string para);
	~WorkClass2();

public:
	void InitClass() override;
	void UpDateClass(const MsgInfo& msgInfo) override;
};

class WorkClass3 :public CBase
{
public:
	WorkClass3() {};
	WorkClass3(std::string para) {};
	~WorkClass3() {};
};

class WorkClass4 :public CBase
{
public:
	WorkClass4() {};
	WorkClass4(std::string para) {};
	~WorkClass4() {};
};

class WorkOne :public CBase
{
public:
	WorkOne() {
		this->AddSignalSlotInfoList("signalFun()", SignalSlotType::Signal);
		this->AddSignalSlotInfoList("signalFun(int)", SignalSlotType::Signal);
		this->AddSignalSlotInfoList("slotFun(int,std::string)", SignalSlotType::Slot);
	};
	~WorkOne() {};
public:
	void signalFun() {
		void* pBufferArr[] = { nullptr };
		//法1
		//void* pBufferArr[2] = { &a ,&str };
		//法2
		//void* pBufferArr[2] = {const_cast<void*>(reinterpret_cast<const void*>(&a)) ,const_cast<void*>(reinterpret_cast<const void*>(&str)) };
		MetaObj::getIns()->ActivateSlot(this, "signalFun()", pBufferArr);
	};
	void signalFun(int a) {
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)) };
		MetaObj::getIns()->ActivateSlot(this, "signalFun(int)", pBufferArr);
	};
	void slotFun(int a,std::string str) {
		std::cout << "slotFun(int a,std::string str)" << a<<"  " << str << std::endl;
	};
	//有多少个槽函数就生成多少个类内部的唯一idd
    //并且提供一个方法寻找这个函数对应的唯一idd（连接/注册/绑定的时候需要保存下来）
    //需要一个容器存储 idd到槽函数方法名字符串的映射
    //需要把重载函数考虑进去  还有就是传参
    //需要基类提供一些支持
	//现在有了新的一套就不需要这种方式了  只要关注信号函数即可
	void NotifyClass(int idd, void** arg) override {
		if (idd == 3)
			slotFun(*reinterpret_cast<int*>(arg[0]), *reinterpret_cast<std::string*>(arg[1]));
	}
private:
};

class WorkTwo :public CBase
{
public:
	WorkTwo() {
		this->AddSignalSlotInfoList("slotFun()", SignalSlotType::Slot);
		this->AddSignalSlotInfoList("slotFun(int)", SignalSlotType::Slot);//用于生成需要调用的槽函数ID
		this->AddSignalSlotInfoList("signalFun(int,std::string)", SignalSlotType::Signal);
	};
	~WorkTwo() {};
public:
	void slotFun() {
		std::cout << "slotFun" << std::endl;
	};
	void slotFun(int a) {
		std::cout << "slotFun(int a)" << a << std::endl;
	};
	void signalFun(int a, std::string str) {
		void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)),const_cast<void*>(reinterpret_cast<const void*>(&str)) };
		MetaObj::getIns()->ActivateSlot(this, "signalFun(int,std::string)", pBufferArr);
	};
	//有多少个槽函数就生成多少个类内部的唯一idd
	//并且提供一个方法寻找这个函数对应的唯一idd（连接/注册/绑定的时候需要保存下来）
	//需要一个容器存储 idd到槽函数方法名字符串的映射
	//需要把重载函数考虑进去  还有就是传参
	//需要基类提供一些支持
	//现在有了新的一套就不需要这种方式了  只要关注信号函数即可
	void NotifyClass(int idd, void** arg) override {
		if (idd == 1)
			slotFun();
		else if (idd == 2)
			slotFun(*reinterpret_cast<int*>(arg[0]));
	};
private:
};