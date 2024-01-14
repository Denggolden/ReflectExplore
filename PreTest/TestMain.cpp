//// declval example
//#include <utility>      // std::declval
//#include <iostream>     // std::cout
//
//struct A {              // abstract class
//	virtual int value() = 0;
//};
//
//class B : public A {    // class with specific constructor
//	int val_;
//public:
//	B(int i, int j) :val_(i* j) {}
//	int value() { return val_; }
//};
//
//int main() {
//	decltype(std::declval<A>().value()) a;  // int a
//	decltype(std::declval<B>().value()) b;  // int b
//	decltype(B(0, 0).value()) c;   // same as above (known constructor)
//	a = b = B(10, 2).value();
//	std::cout << a << '\n';
//	return 0;
//}



//-----------------------�ָ���------------------------------------------------

///**********************************************
//  > File Name		: aop.cpp
//  > Author		    : lunar
//  > Email			: lunar_ubuntu@qq.com
//  > Created Time	: Wed 14 Oct 2020 11:53:12 PM CST
// **********************************************/
//
//#include <functional>
// // write an AOP framework with C++11
//
// // Tip: when decltype receives two args, it returns the second argument when the first one is not compilable.
// // This struct is to check if a class has the member function Before or After.
//#define HAS_MEMBER(member)\
//template<typename T, typename... Args> struct has_member_##member{\
//private:\
//    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
//    template<typename U> static std::false_type Check(...);\
//public:\
//    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
//};\
//
//HAS_MEMBER(Foo)
//HAS_MEMBER(Before)
//HAS_MEMBER(After)
//
////#include <Noncopyable.hpp>
//template <typename Func, typename... Args>
//struct Aspect {
//    Aspect(Func& f) : m_func(std::forward<Func>(f)) {
//
//    }
//
//    template <typename T>
//    typename std::enable_if<has_member_Before<T, Args...>::value&& has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
//        aspect.Before(std::forward<Args>(args)...); //aspect before the core code
//        m_func(std::forward<Args>(args)...); //core code
//        aspect.After(std::forward<Args>(args)...); //aspect after the core code
//    }
//
//    template <typename T>
//    typename std::enable_if<has_member_Before<T, Args...>::value && !has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
//        aspect.Before(std::forward<Args>(args)...);
//        m_func(std::forward<Args>(args)...);
//    }
//
//    template <typename T>
//    typename std::enable_if<!has_member_Before<T, Args...>::value&& has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect) {
//        m_func(std::forward<Args>(args)...);
//        aspect.After(std::forward<Args>(args)...);
//    }
//
//    //to insert multiple aspects
//    template <typename Head, typename... Tail>
//    void Invoke(Args&&... args, Head&& headAspect, Tail&&... tailAspect) {
//        headAspect.Before(std::forward<Args>(args)...);
//        Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
//        headAspect.After(std::forward<Args>(args)...);
//    }
//
//private:
//    Func m_func;
//};
//template <typename T>
//using identity_t = T;
//
//template <typename... AP, typename... Args, typename Func>
//void Invoke(Func&& f, Args&&... args) {
//    Aspect<Func, Args...> asp(std::forward<Func>(f));
//    asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
//}
//
//#include <iostream>
//using namespace std;
//struct AA {
//    void Before() {
//        cout << "AA Before" << endl;
//    }
//
//    void After() {
//        cout << "AA After" << endl;
//    }
//};
//
//struct BB {
//    void Before() {
//        cout << "BB Before" << endl;
//    }
//
//    void After() {
//        cout << "BB After" << endl;
//    }
//};
//
//void core_func() {
//    cout << "core function called" << endl;
//}
//
//int main() {
//    std::function<void()> f = std::bind(core_func);
//    Invoke<AA, BB>(f);
//    return 0;
//}



//-----------------------�ָ���------------------------------------------------

//#include <iostream>
//#include <new>
//#include <string>
//#include <type_traits>
//
////// Partial specialization for true.
////template<typename _Tp>
////struct std::enable_if<true, _Tp> {
////	typedef _Tp type;
////};
////
////template<typename any>
////inline typename std::enable_if<std::is_unsigned<any>::value, any>::type lowbit(const any& x) {
////	return x & -x;
////}
//
//
//template <typename T>
//struct MEB {
//	using type = T;
//};
//
////// STRUCT TEMPLATE enable_if
////template <bool _Test, class _Ty = void>   //�����汾
////struct std::enable_if {}; // no member "type" when !_Test
//
////template <class _Ty> //ƫ�ػ��汾:��ô��⣺��ֻ�����ƫ�ػ��汾���ڣ��Ŵ���һ�����ֽ���type�����ͱ��������ͣ���
////struct std::enable_if<true, _Ty> { // type is _Ty for _Test
////	using type = _Ty;
////};
//
////using enable_if_t = typename enable_if<_Test, _Ty>::type;
////enable_if<_Test, _Ty>::type ������ enable_if_t
//
////�ܵ�˵�� typename std::enable_if_t<std::is_same_v<U, char>,char> 
////�е�һ������ std::is_same_v<U, char>��ʾ��������Ƿ����
////�еڶ������� char ��ʾ���� typename std::enable_if_t<std::is_same_v<U, char>,char> ���ڵĻ�����������ʲô
//
////һ����Ϊ��������
//template<typename T>
//struct Check1
//{
//	//���T��������int�����庯�� int read(void* = nullptr)
//	template<typename U = T>
//	U read(typename std::enable_if_t<std::is_same_v<U, int> >* = nullptr) {
//		return 42;
//	}
//
//	//���T��������double�����庯�� double read(void* = nullptr)
//	template<typename U = T>
//	U read(typename std::enable_if_t<std::is_same_v<U, double> >* = nullptr) {
//		return 3.14;
//	}
//
//	//���T��������char�����庯�� char read(char*)
//	template<typename U = T>
//	U read(typename std::enable_if_t<std::is_same_v<U, char>,char> c) {
//		return c;
//	}
//};
//
////������Ϊģ�����
////��ס typename std::enable_if_t<std::is_same_v<U, int>, int> �Ǹ�����
//template<typename T>
//struct Check2
//{
//	//���T��������int�����庯�� int read()
//	template<typename U = T, typename std::enable_if_t<std::is_same_v<U, int>, int> = 0>
//	U read() {
//		return 42;
//	}
//
//	//���T��������double�����庯�� double read()
//	template<typename U = T, typename std::enable_if_t<std::is_same_v<U, double>, int> = 0>
//	U read() {
//		return 3.14;
//	}
//};
//
//
////������Ϊ����ֵ����
//template <typename T>
//typename std::enable_if<(sizeof(T) > 2)>::type funceb() {
//	//...
//}
//template<typename T>
//struct Check3
//{
//	//���T��������int�����庯�� int read()
//	template<typename U = T>
//	typename std::enable_if_t<std::is_same_v<U, int>, U> read() {
//		return 42;
//	}
//
//	//���T��������double�����庯�� double read()
//	template<typename U = T>
//	typename std::enable_if_t<std::is_same_v<U, double>, U> read() {
//		return 3.14;
//	}
//};
//
//
//int main()
//{
//	MEB<int>::type abc = 15;
//
//	//std::enable_if<(3>2)>::type * mypoint1 = nullptr;  //�൱�� void *mypoint1 = nullptr
//	//����std::enable_if��һ������Ϊtrue����ִ��ƫ�ػ��汾��֧����type���ͣ��ҷ����汾�Ѿ������˵ڶ���������Ĭ��ֵΪvoid����typeΪvoid
//
//	//std::enable_if<(3<2)>::type* mypoint1 = nullptr;
//	//����std::enable_if��һ������Ϊfalse��ִ�з����汾��֧�������汾��û��type���ͣ����Ա���
//
//	funceb<int>();
//	//funceb<bool>();
//	//����std::enable_if��һ������Ϊfalse��ִ��ƫ�ػ��汾������������Ҳ����˵���������ڶ��壬���Ա���
//
//	Check1<int> ch1; ch1.read();
//	Check1<double> ch2;	ch2.read();
//	char c = ' ';
//	Check1<char> ch3;	ch3.read(c);
//
//	//Check3<int> ch1; ch1.read();
//	//Check3<double> ch2;	ch2.read();
//	//Check3<char> ch3;	ch3.read();//����
//
//	return 0;
//}



//-----------------------�ָ���------------------------------------------------

//#include <string>
//#include <iostream>
//
//using namespace std;
//
//
//class Box {
//public:
//	string str() {
//		return "yes";
//	}
//};
//
//class Bin {
//public:
//	string str1() {
//		return "no";
//	}
//};
//
//template <typename U>
//struct class_str {
//
//	template <typename T, string(T::*)() = &T::str>
//	static constexpr bool check(T*) { return true; };
//
//	static constexpr bool check(...) { return false; };
//
//	static constexpr bool ret = check(static_cast<U*>(0));
//};
//
//// ������string str()�����ķ�std::string��
//template<typename T,
//	typename std::enable_if<std::is_class<T>::value && !std::is_same<T, string>::value, T>::type* = nullptr,
//	typename std::enable_if<!class_str<T>::ret, T>::type* = nullptr>
//	std::string str(T& t) {
//	cout << "1.---------------------" << endl;
//	return "null";
//};
//
//// std::string��
//template<typename T,
//	typename std::enable_if<std::is_class<T>::value&& std::is_same<T, string>::value, T>::type* = nullptr>
//	std::string str(T& t) {
//	cout << "2.---------------------" << endl;
//	return t;
//};
//
//// ����string str()�����ķ�std::string��
//template<typename T,
//	typename std::enable_if<std::is_class<T>::value && !std::is_same<T, string>::value, T>::type* = nullptr,
//	typename std::enable_if<class_str<T>::ret, T>::type* = nullptr>
//	std::string str(T& t) {
//	cout << "3.---------------------" << endl;
//	return t.str();
//};
//
//// ��ֵ��
//template<typename T,
//	typename std::enable_if<!std::is_class<T>::value&& std::is_arithmetic<T>::value, T>::type* = nullptr>
//	std::string str(T& t) {
//	cout << "4.---------------------" << endl;
//	return std::to_string(t);
//};
//
//int main() {
//	string s = "sddds";
//	cout << str<string>(s) << endl;
//
//	bool j = true;
//	cout << str<bool>(j) << endl;
//
//	int i = 1000;
//	cout << str<int>(i) << endl;
//
//	float f = 10.6f;
//	cout << str<float>(f) << endl;
//
//	Box b1;
//	cout << str<Box>(b1) << endl;
//
//	Bin b2;
//	cout << str<Bin>(b2) << endl;
//
//	return 1;
//}


//-----------------------�ָ���------------------------------------------------

//#include <stdio.h>
//#include <stdlib.h>
//#include <typeinfo>
//
//class Parent
//{
//public:
//    virtual void tt() {};
//};
//
//class Son : public Parent
//{
//};
//
//class Test_A {
//};
//
//class Test_B : public Test_A {
//};
//
//int main()
//{
//    Parent* p1 = new Son();
//    Son* s1 = new Son();
//    Test_A* ta = new Test_A();
//    printf("ptr type Son: %s, Parent: %s, Test_A: %s\n", typeid(s1).name(), typeid(p1).name(), typeid(ta).name());
//    printf("obj Son: %s, Parent: %s, Test_A: %s\n", typeid(*s1).name(), typeid(*p1).name(), typeid(*ta).name());
//    printf("type Son: %s, Parent: %s, Test_A: %s\n", typeid(Son).name(), typeid(Parent).name(), typeid(Test_A).name());
//    printf("successtion: %d, %d\n", typeid(Son).before(typeid(Parent)), typeid(Parent).before(typeid(Son)));
//
//    Test_A* tb = new Test_B();
//    printf("type tb: %s��\n", typeid(tb).name());
//
//
//}


//-----------------------�ָ���------------------------------------------------
#include<iostream>
//template <typename T>
//T fun();

template<typename Ret>
Ret fun()
{
	std::cout << "template<typename>" << std::endl;
}

template<typename Ret, typename Arg1>
Ret fun()
{
	std::cout << "template<typename Arg1>" << std::endl;
}

template<typename Ret, typename Arg1, typename Arg2>
Ret fun()
{
	std::cout << "template<typename Arg1, typename Arg2>" << std::endl;
}

template<typename Ret,typename... Args>
struct MyStruct
{
	void Call() {
		fun<Ret,Args...>();
	}
};

int main()
{
	MyStruct<void> m;
	m.Call();
	MyStruct<void,int> mm;
	mm.Call();
	MyStruct<void,int, int> mmm;
	mmm.Call();
	//std::cout << fun<int>() << std::endl;//��ʽʵ����
	return 0;
}