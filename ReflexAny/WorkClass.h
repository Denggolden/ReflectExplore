#pragma once
#include "ReflexBase.h"

typedef struct ST {
	ST() {};
	ST(int a_, std::string str_) { a = a_; str = str_; };
	int a=0;
	std::string str="";
}ST;

class WorkClass
{
public:
	WorkClass();
	WorkClass(int a);
	~WorkClass();
	//如无特殊需要可这样创建成员变量
	//对于基础型
	CreateMemVar1(int, a, 10);//带自定默认值版本（下面依此类推）
	CreateMemVar2(int, b);//不带自定默认值版本（下面依此类推）

	//对于自定义型
	CreateMemVar1(ST, sTa, ST());
	CreateMemVar2(ST, sTb);
};