#pragma once

#include "ReflexBase.h"

class WorkClass1 :public CBase
{
public:
	WorkClass1();
	WorkClass1(int para);
	~WorkClass1();

protected:
   void InitClass() override;
	void UpDateClass(const MsgInfo& msgInfo) override;
};

class WorkClass2 :public CBase
{
public:
	WorkClass2();
	WorkClass2(std::string para);
	~WorkClass2();

protected:
	void InitClass() override;
	void UpDateClass(const MsgInfo& msgInfo) override;
};

