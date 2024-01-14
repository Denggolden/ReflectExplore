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
		//��1
		//void* pBufferArr[2] = { &a ,&str };
		//��2
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
	//�ж��ٸ��ۺ��������ɶ��ٸ����ڲ���Ψһidd
    //�����ṩһ������Ѱ�����������Ӧ��Ψһidd������/ע��/�󶨵�ʱ����Ҫ����������
    //��Ҫһ�������洢 idd���ۺ����������ַ�����ӳ��
    //��Ҫ�����غ������ǽ�ȥ  ���о��Ǵ���
    //��Ҫ�����ṩһЩ֧��
	//���������µ�һ�׾Ͳ���Ҫ���ַ�ʽ��  ֻҪ��ע�źź�������
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
		this->AddSignalSlotInfoList("slotFun(int)", SignalSlotType::Slot);//����������Ҫ���õĲۺ���ID
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
	//�ж��ٸ��ۺ��������ɶ��ٸ����ڲ���Ψһidd
	//�����ṩһ������Ѱ�����������Ӧ��Ψһidd������/ע��/�󶨵�ʱ����Ҫ����������
	//��Ҫһ�������洢 idd���ۺ����������ַ�����ӳ��
	//��Ҫ�����غ������ǽ�ȥ  ���о��Ǵ���
	//��Ҫ�����ṩһЩ֧��
	//���������µ�һ�׾Ͳ���Ҫ���ַ�ʽ��  ֻҪ��ע�źź�������
	void NotifyClass(int idd, void** arg) override {
		if (idd == 1)
			slotFun();
		else if (idd == 2)
			slotFun(*reinterpret_cast<int*>(arg[0]));
	};
private:
};