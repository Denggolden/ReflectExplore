#include "WorkClass.h"

WorkClass1::WorkClass1():CBase()
{std::cout << "WorkClass1::WorkClass1()"<< std::endl;}
WorkClass1::WorkClass1(int para) : CBase()
{std::cout << "WorkClass1::WorkClass1(int para)" << std::endl;}

WorkClass1::~WorkClass1()
{
	std::cout << "WorkClass1::~WorkClass1()" << std::endl;
}

void WorkClass1::InitClass()
{
	std::cout << "WorkClass1::InitClass()" << std::endl;
}

void WorkClass1::UpDateClass(const MsgInfo& msgInfo)
{
	std::cout << "WorkClass1::UpDateClass(const MsgInfo& msgInfo)" << std::endl;
	std::cout << msgInfo << std::endl;
}

WorkClass2::WorkClass2() :CBase()
{std::cout << "WorkClass2::WorkClass2()" << std::endl;}

WorkClass2::WorkClass2(std::string para) : CBase()
{std::cout << "WorkClass2::WorkClass2(std::string para)" << std::endl;}

WorkClass2::~WorkClass2()
{
	std::cout << "WorkClass2::~WorkClass2()" << std::endl;
}

void WorkClass2::InitClass()
{
	std::cout << "WorkClass2::InitClass()" << std::endl;
}

void WorkClass2::UpDateClass(const MsgInfo& msgInfo)
{
	std::cout << "WorkClass2::UpDateClass(const MsgInfo& msgInfo)" << std::endl;
	std::cout << msgInfo << std::endl;
}
