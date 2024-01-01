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
	//std::cout << "std::string: " << sizeof(strlen(str.data()))<< std::endl;
	std::cout << "std::string: " << str.length() << std::endl;
	int allSize =sizeof(char)+ sizeof(int)+ sizeof(double)+ str.length();
	std::cout << "allSize: "<< allSize <<std::endl;

	char* pch = new char[allSize];
	memcpy(&pch[0], &ch, sizeof(char));
	memcpy(&pch[sizeof(char)], &i, sizeof(int));
	memcpy(&pch[sizeof(char)+ sizeof(int)], &d, sizeof(double));
	memcpy(&pch[sizeof(char) + sizeof(int)+ sizeof(double)], str.data(), str.length());

	char ch1 = 'B';
	int i1 = 12;
	double d1 = 3.6;
	std::string str1 = "231";
	std::string str2;

	std::cout << "ch1: " << ch1 << std::endl;
	std::cout << "i1: " << i1 << std::endl;
	std::cout << "d1: " << d1 << std::endl;
	std::cout << "str1: " << str1 << std::endl;

	memcpy(&ch1, &pch[0], sizeof(char));
	memcpy(&i1, &pch[sizeof(char)], sizeof(int));
	memcpy(&d1, &pch[sizeof(char)+ sizeof(int)], sizeof(double));
	//memcpy(&str2, &pch[sizeof(char) + sizeof(int)+ sizeof(double)], sizeof(std::string));

	std::cout << "ch1: " << ch1 << std::endl;
	std::cout << "i1: " << i1 << std::endl;
	std::cout << "d1: " << d1 << std::endl;
	//std::cout << "str1: " << str1 << std::endl;

	//char sz[] = { 'h', 'e', 'l', 'l', 'o', '\0', 'w', 'o', 'r', 'l', 'd', '\0' };
	std::string s1(&pch[sizeof(char) + sizeof(int) + sizeof(double)], str.length());
	std::cout << "s1: " << s1 << std::endl;

	return 0;
}

//#include <iostream>
//using namespace std;
//
//int main() {
//	int num = 10; // 要转换的整数值
//
//	char* byteStream = reinterpret_cast<char*>(&num); // 将整数指针转换为字节流指针
//
//	for (size_t i = sizeof(int) - 1; i >= 0; --i) {
//		cout << static_cast<unsigned int>(byteStream[i]) << " "; // 输出每个字节的十六进制表示形式
//	}
//
//	return 0;
//}