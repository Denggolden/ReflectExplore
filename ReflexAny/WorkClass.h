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
	//����������Ҫ������������Ա����
	//���ڻ�����
	CreateMemVar1(int, a, 10);//���Զ�Ĭ��ֵ�汾�������������ƣ�
	CreateMemVar2(int, b);//�����Զ�Ĭ��ֵ�汾�������������ƣ�

	//�����Զ�����
	CreateMemVar1(ST, sTa, ST());
	CreateMemVar2(ST, sTb);
};