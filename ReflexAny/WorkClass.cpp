#include "WorkClass.h"
#include <iostream>

WorkClass::WorkClass()
{
	std::cout << "WorkClass::WorkClass() " << std::endl;
}

WorkClass::WorkClass(int a)
{
	std::cout << "WorkClass::WorkClass(int a) " << std::endl;
}

WorkClass::~WorkClass()
{
	std::cout << "WorkClass::~WorkClass()" << std::endl;
}
