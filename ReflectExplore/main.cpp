#include <iostream>
#include <functional>
#include <list>

//template< typename src_type>
//void* pointer_cast(src_type src)
//{
//    return *static_cast<void*>(static_cast<void*>(&src));
//}

template<typename T1, typename T2, typename ...Targs>
class CreateObj {
    typedef struct Info {
        Info() {};
        Info(T1* sender, void (T1::* signal)(Targs...args) /*std::function<void(Targs...args)> signal*/,
            T2* receiver, void (T2::* slots)(Targs...args) /*std::function<void(Targs...args)> slots*/) {
            Sender = sender; Signal = signal;
            Receiver = receiver; Slots = slots;
        }
        T1* Sender; void (T1::* Signal)(Targs...args);//std::function<void(Targs...args)> Signal;
        T2* Receiver; void (T2::* Slots)(Targs...args); //std::function<void(Targs...args)> Slots;
    }Info;
public:
    //~CreateObj() {};
    void Connect(T1* sender, void (T1::* signal)(Targs...args),
        T2* receiver, void (T2::* slots)(Targs...args)) {
        //std::function<void(Targs...args)> signalFun = std::bind(signal, sender);
        //std::function<void(Targs...args)> slotsFun = std::bind(slots, receiver);
        //void* p = pointer_cast<void*>(&signal);
        ////void* p2 = pointer_cast<void*>(&Class1::SignalFun2);
        //std::cout << p << std::endl;
        std::cout << "Connect (&signal): " << (&signal) << std::endl;
        InfoList.push_back(Info(sender, signal, receiver, slots));
    };

    void Call(T1* t, void (T1::* signal)(Targs...args)) {
        std::list<Info>::template iterator it = InfoList.begin();
        for (; it != InfoList.end(); it++) {
            if ((*it).Sender == t/* && (&(*it).Signal) == (&signal)*/) {
                std::cout << "EXist" << std::endl;
            }
            std::cout << "(void*)(*it).Signal): " << (void*)(&(*it).Signal) <<std::endl;
            std::cout << "(&signal): " << (&signal) <<std::endl;
        }
    }
    //单例模式
    static CreateObj* getIns() {
        static CreateObj Ins;
        return &Ins;
    }
private:
    CreateObj() { InfoList.clear(); };  //私有
    std::list<Info> InfoList;
};

class Class1 {
public:
    Class1() {};
    ~Class1() {};
public:
    void SignalFun1() {};
    void SignalFun2(int a) {};
public:
    void SlotsFun1() {};
    void SlotsFun2(int a) {};
};

class Class2 {
public:
    Class2() {};
    ~Class2() {};
public:
    void SignalFun1() {};
    void SignalFun2(int a) {};
public:
    void SlotsFun1() {};
    void SlotsFun2(int a) {};
};

//class B
//{
//public:
//    B();
//    ~B();
//
//    void functionB(int value)
//    {
//        //cout << "functionB:" << value << endl;
//    }
//};


int main()
{
    Class1* pClass1 = new Class1();
    Class2* pClass2 = new Class2();

    //std::cout << static_cast<void (Class1::*)()>(&Class1::SignalFun1) << std::endl;
    //std::cout << &Class1::SignalFun2 << std::endl;

    //void* p1 = pointer_cast(&Class1::SignalFun1);
    //void* p2 = pointer_cast(&Class1::SignalFun2);
    //std::cout << p1 << std::endl << p2 << std::endl << std::endl;
    //static_cast<void*>(&src)

    //std::cout << "(&pClass1): " << (&pClass1) << std::endl;
    //std::cout << "(void*)pClass1: " << (void*)pClass1 << std::endl;
    //B b;
    //std::function<void(int)> fun = std::bind(&B::functionB, &b, std::placeholders::_1);
    //static_cast<void (QTcpSocket::*)()>(&QTcpSocket::connected)
    //static_cast<void* (*)()>(&funint)

    CreateObj<Class1, Class2>::getIns()->Connect(pClass1, &Class1::SignalFun1,
        pClass2, &Class2::SlotsFun1);
    CreateObj<Class1, Class2, int>::getIns()->Connect(pClass1, &Class1::SignalFun2,
        pClass2, &Class2::SlotsFun2);

    CreateObj<Class1, Class2>::getIns()->Call(pClass1, &Class1::SignalFun1);

    return 0;
}