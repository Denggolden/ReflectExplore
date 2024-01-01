#pragma once

#include "ReflexBase.h"
#include <vector>

class WorkClass1 :public CBase
{
public:
	WorkClass1();
	WorkClass1(int para);
	~WorkClass1();

	CreateMemVar1(int,A,10);
	CreateMemVar2(std::vector<int>,vec);

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