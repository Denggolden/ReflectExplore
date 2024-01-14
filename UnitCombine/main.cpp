//#include <stdio.h>
//
//typedef struct structC {
//	char* name;
//	int age;
//}structC;
//
//void setInfo(struct structC* str, char* name, int age)
//{
//	str->age = age;
//	str->name = name;
//}
//
//void printInfo(struct structC* str)
//{
//	printf("name = %s, age = %d\n", str->name, str->age);
//}
//
//void FUNDef(int a) {
//	printf("Callback FUNDef a=%d\n", a);
//}
//
//void (*ptr)(int a);
//
//void bind_B(void (*FUN)(int)) {
//	ptr = FUN;
//};
//
//void FUN(int a) {
//	printf("Callback a+1=%d\n",a+1);
//}
//
//int main()
//{
//	structC structCC;
//	setInfo(&structCC,"123",1);
//	printInfo(&structCC);
//
//	ptr = FUNDef;
//	ptr(1);
//	bind_B(FUN);
//	ptr(10);
//
//	return 0;
//}

#include <iostream>
#include <string>


int main()
{
	char ch = 'A';
	int i = 10;
	double d = 6.6;
	std::string str = "123";

	std::cout << "char: " << sizeof(char) << std::endl;
	std::cout << "int: " << sizeof(int) << std::endl;
	std::cout << "double: " << sizeof(double) << std::endl;
	std::cout << "std::string: " << sizeof(std::string) << std::endl;
	int allSize =sizeof(char)+ sizeof(int)+ sizeof(double)+ sizeof(std::string);
	std::cout << "allSize: "<< allSize <<std::endl;

	char* pch = new char[allSize];
	memcpy(&pch[0], &ch, sizeof(char));
	memcpy(&pch[sizeof(char)], &i, sizeof(int));
	memcpy(&pch[sizeof(char)+ sizeof(int)], &d, sizeof(double));
	memcpy(&pch[sizeof(char) + sizeof(int)+ sizeof(double)], &str, sizeof(std::string));

	char ch1 = 'B';
	int i1 = 12;
	double d1 = 3.6;
	std::string str1 = "231";

	std::cout << "ch1: " << ch1 << std::endl;
	std::cout << "i1: " << i1 << std::endl;
	std::cout << "d1: " << d1 << std::endl;
	std::cout << "str1: " << str1 << std::endl;

	memcpy(&ch1, &pch[0], sizeof(char));
	memcpy(&i1, &pch[sizeof(char)], sizeof(int));
	memcpy(&d1, &pch[sizeof(char)+ sizeof(int)], sizeof(double));
	memcpy(&str1, &pch[sizeof(char) + sizeof(int)+ sizeof(double)], sizeof(std::string));

	std::cout << "ch1: " << ch1 << std::endl;
	std::cout << "i1: " << i1 << std::endl;
	std::cout << "d1: " << d1 << std::endl;
	std::cout << "str1: " << str1 << std::endl;

	return 0;
}