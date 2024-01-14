#pragma once
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <type_traits>
#include <functional>
#include <sstream>

#define signal(sig)\
 #sig
#define slots(slot)\
 #slot

typedef enum class MsgType
{
    Type0 = 0, Type1, Type2
};

typedef struct MsgInfo {
    MsgInfo() {};
    MsgInfo(MsgType msg_Type, std::string msg_Content) {
        Msg_Type = msg_Type; Msg_Content = msg_Content;
    };
    MsgType Msg_Type = MsgType::Type0;//消息类型
    std::string Msg_Content = "";//消息内容
    friend std::ostream& operator << (std::ostream&, const MsgInfo& P) {
        std::cout << "(" << (int)P.Msg_Type << "," << P.Msg_Content << ")";
        return std::cout;
    }
}MsgInfo;

typedef enum class SignalSlotType
{
    Signal = 0, Slot
};

typedef struct SignalSlotInfo {
    SignalSlotInfo() {};
    SignalSlotInfo(int id, std::string funName, SignalSlotType signalSlotType) { Id = id; FunName = funName; SignalSlotType = signalSlotType; };
    int Id=-1;
    std::string FunName="";
    SignalSlotType SignalSlotType= SignalSlotType::Signal;
}SignalSlotInfo;

class CBase {//所有类的基类
public:
    CBase() { SignalSlotInfoList.clear(); };
    virtual ~CBase() {};
    void setObjName(const std::string& objName) { ObjName = objName; };
    std::string getObjName() { return ObjName; };
public:
    /*用于替换掉以前需要在构造函数里面需要执行的内容 【成员变量初始化】要写在构造函数里面
    （也就是说现在构造函数里面基本上什么代码都不需要写(【除了成员变量初始化外】)）（非得写到构造函数里面的代码除外  或许某些机制需要这样的）
    InitClass用于减少模块间初始化相互依赖的问题
    如需要各种不同的构造函数走不同的构造函数逻辑，可以在类的构造函数里面及类里面增加一个成员变量加以区分
    当走InitClass时候也可用这个标志位加以区分，从而把类完整的初始化完毕*/
    virtual void InitClass() {};
    //用于统一通知
    virtual void UpDateClass(const MsgInfo& msgInfo) {};
    //用于信号槽预先探索
    //idd 用于选择槽函数的
    //arg 用于传递参数的
    virtual void NotifyClass(int idd, void** arg) {};
    virtual void AddSignalSlotInfoList(std::string funName, SignalSlotType signalSlotType) {
        int id = SignalSlotInfoList.size()+1;
        std::list<SignalSlotInfo>::iterator it;
        for (it = SignalSlotInfoList.begin(); it != SignalSlotInfoList.end(); it++) {
            if (it->FunName == funName) {
                std::cout << "信号或槽不可重复添加！！！" << std::endl;
                return;
            }
        }
        SignalSlotInfoList.push_back(SignalSlotInfo(id, funName, signalSlotType));
    };
    virtual bool FindSignalIsExist(std::string signalName) {
        std::list<SignalSlotInfo>::iterator it;
        for (it = SignalSlotInfoList.begin(); it != SignalSlotInfoList.end(); it++) {
            if (it->FunName == signalName) {
                return true;
            }
        }
        std::cout << "对象信号不存在！！！" << std::endl;
        return false;
    };
    virtual int FindSlotIsExist(std::string slotName) {
        int id = -1;
        std::list<SignalSlotInfo>::iterator it;
        for (it = SignalSlotInfoList.begin(); it != SignalSlotInfoList.end(); it++) {
            if (it->FunName == slotName) {
                id = it->Id;
                return id;
            }
        }
        std::cout << "对象槽函数不存在！！！" << std::endl;
        return id;
    }
private:
    std::string ObjName;
    std::list<SignalSlotInfo> SignalSlotInfoList;
};

typedef struct SenderInfo {
    SenderInfo() {};
    SenderInfo(CBase* senderPtr, std::string signalName) {SenderPtr = senderPtr; SignalName = signalName;};
    CBase* SenderPtr =nullptr;
    std::string SignalName="";
}SenderInfo;

typedef struct ReceiverInfo {
    ReceiverInfo() {};
    ReceiverInfo(CBase* receiverPtr, int slotsFunId) { ReceiverPtr = receiverPtr; SlotsFunId = slotsFunId; };
    CBase* ReceiverPtr=nullptr;
    int SlotsFunId=-1;
}ReceiverInfo;

typedef struct ConnectInfo {
    ConnectInfo() {};
    ConnectInfo(SenderInfo senderInfo, ReceiverInfo receiverInfo) { SenderInfo = senderInfo; ReceiverInfo = receiverInfo; };
    SenderInfo SenderInfo;
    ReceiverInfo ReceiverInfo;
}ConnectInfo;

class FunPtrBase {
    //typedef void (*ImplFn)(int which, FunPtrBase* this_, CBase* receiver, void** args, bool* ret);
    //const ImplFn m_impl;
protected:
    enum Operation {
        Destroy=0,
        Call,
        Compare,/*暂时未知*/
        NumOperations/*暂时未知*/
    };
public:
    FunPtrBase() {};
    ~FunPtrBase() {};
    //方法1  采用虚函数多态
public:
    virtual void CallSlots(int which, FunPtrBase* this_, void** arg) {};
//    //方法2  采用函数指针回调(似乎失败)
//public:
//    inline void call(CBase* r, void** args) { m_impl(Call, this, r, args, nullptr); }
//    inline void destroy() {m_impl(Destroy, this, nullptr, nullptr, nullptr);}
};

template<typename T, typename Ret, typename... Args>
class FunPtrSlot :public FunPtrBase
{
    typedef void (T::* slotFunPtr)(Args...);
public:
    FunPtrSlot(T* t, slotFunPtr funPtr) :Tptr(t), FunPtr(funPtr){ /*FunPtr = funPtr;*/ };
    void CallSlots(int which, FunPtrBase* this_, void** arg) override {
        switch (which) {
        case Destroy:
            delete static_cast<FunPtrSlot*>(this_); break;
        case Call: 
            call<Ret,Args...>(arg);break;//Ret辅助构造的
                 //case Compare:
                 //    *ret = *reinterpret_cast<Func*>(a) == static_cast<QSlotObject*>(this_)->function;
                 //    break;
        case NumOperations:;
        }
    };
    template<typename Ret>//无参
    Ret call(void** arg) {
        (Tptr->*FunPtr)();
    }
    template<typename Ret,typename Arg1>//任意类型 1参
    Ret call(void** arg) {
        (Tptr->*FunPtr)(*reinterpret_cast<Arg1*>(arg[0]));
    }
    template<typename Ret, typename Arg1, typename Arg2>//任意类型 2参
    Ret call(void** arg) {
        (Tptr->*FunPtr)(*reinterpret_cast<Arg1*>(arg[0]), *reinterpret_cast<Arg2*>(arg[1]));
    }
    //后续追加参数的时候只要往后面这样追加就行了
private:
    T* Tptr;
    slotFunPtr FunPtr;
};

template<typename T,typename Ret,typename F, typename... Args>
class FunPtrSlotLam :public FunPtrBase
{
public:
    FunPtrSlotLam(T* t, F const& funPtr) :Tptr(t), FunPtr(funPtr) { /*FunPtr = funPtr;*/ };
    void CallSlots(int which, FunPtrBase* this_, void** arg) override {
        switch (which) {
        case Destroy:
            delete static_cast<FunPtrSlotLam*>(this_); break;
        case Call:
            call<Ret, Args...>(arg); break;//Ret辅助构造的
            //case Compare:
            //    *ret = *reinterpret_cast<Func*>(a) == static_cast<QSlotObject*>(this_)->function;
            //    break;
        case NumOperations:;
        }
    };
    template<typename Ret>//无参
    Ret call(void** arg) {
        FunPtr();
    }
    template<typename Ret, typename Arg1>//任意类型 1参
    Ret call(void** arg) {
        FunPtr(*reinterpret_cast<Arg1*>(arg[0]));
    }
    template<typename Ret, typename Arg1, typename Arg2>//任意类型 2参
    Ret call(void** arg) {
        FunPtr(*reinterpret_cast<Arg1*>(arg[0]), *reinterpret_cast<Arg2*>(arg[1]));
    }
private:
    T* Tptr;
    F FunPtr;
};

typedef struct SenderInfoImpl {
    SenderInfoImpl() {};
    SenderInfoImpl(CBase* senderPtr, std::string signalAddr) { SenderPtr = senderPtr; SignalAddr = signalAddr; };
    CBase* SenderPtr = nullptr;
    std::string SignalAddr = "";
}SenderInfoImpl;

typedef struct ConnectInfoImpl {
    ConnectInfoImpl() {};
    ConnectInfoImpl(SenderInfoImpl sender, FunPtrBase* recSlotInfo) { Sender = sender; RecSlotInfo = recSlotInfo; };
    SenderInfoImpl Sender;
    FunPtrBase* RecSlotInfo=nullptr;
}ConnectInfoImpl;

class MetaObj {
public:
    //执行信号槽连接函数
    void Connect(CBase* sender, std::string signal, CBase* receiver, std::string slots) {
        SenderInfo senderInfo; ReceiverInfo receiverInfo;

        if (sender->FindSignalIsExist(signal) == false)
            return;
        senderInfo = SenderInfo(sender, signal);

        int slotId = receiver->FindSlotIsExist(slots);
        if (slotId==-1)
            return;
        receiverInfo = ReceiverInfo(receiver, slotId);

        ConnectInfoList.push_back(ConnectInfo(senderInfo, receiverInfo));
    };
    //所有参数类型的 普通成员函数 当然返回值void 
    template<typename T1, typename T2, typename... Args>
    void Connect(T1* send, void (T1::* signalFunPtr)(Args...), T2* rec, void (T2::* slotFunPtr)(Args...)) {
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr;std::string signalAddr = saddr.str();
        std::cout << "Connect signalAddr: " << signalAddr << std::endl;
        ConnectInfoImplList.push_back(ConnectInfoImpl(SenderInfoImpl(send, signalAddr), new FunPtrSlot<T2,void, Args...>(rec, slotFunPtr)));
    }
    //所有参数类型的 lambda 表达式 当然返回值void 
    template<typename T1, typename F, typename... Args>
    void Connect(T1* send, void (T1::* signalFunPtr)(Args...), T1* sendR, F const& f) {//按理来说的话第三个参数可以不要
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr; std::string signalAddr = saddr.str();
        std::cout << "Connect signalAddr: " << signalAddr << std::endl;
        ConnectInfoImplList.push_back(ConnectInfoImpl(SenderInfoImpl(send, signalAddr), new FunPtrSlotLam<T1,void, F, Args...>(sendR, f)));
    }
    //移除连接
    template<typename T, typename ... Args>
    void DisConnectImpl(T* sender, void (T::* signalFunPtr)(Args...), void** arg) {
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr; std::string signalAddr = saddr.str();
        std::cout << "Call signalAddr: " << signalAddr << std::endl;
        std::list<ConnectInfoImpl>::iterator it;
        for (it = ConnectInfoImplList.begin(); it != ConnectInfoImplList.end();) {
            if (it->Sender.SenderPtr == sender && it->Sender.SignalAddr == signalAddr) {
                FunPtrBase* recSlotInfo = it->RecSlotInfo;
                recSlotInfo->CallSlots(0, recSlotInfo, arg);
                it = ConnectInfoImplList.erase(it);
                //ConnectInfoImplList.erase(it++);//或者
                //break;
            }
            else
                ++it;
        }
    }

    //执行信号通知槽函数触发的函数  之前的
    void ActivateSlot(CBase* sender, std::string signalName, void** arg) {
        std::list<ConnectInfo>::iterator it;
        for (it = ConnectInfoList.begin(); it != ConnectInfoList.end(); it++) {
            if (it->SenderInfo.SenderPtr== sender&& it->SenderInfo.SignalName== signalName) {
                int idd = it->ReceiverInfo.SlotsFunId;
                it->ReceiverInfo.ReceiverPtr->NotifyClass(idd, arg);
                //break;
            }
        }
    };
    //执行信号通知槽函数触发的函数  现在的  更加方便强大了
    template<typename T, typename ... Args>
    void ActivateSlotImpl(T* sender, void (T::* signalFunPtr)(Args...), void** arg) {
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr; std::string signalAddr = saddr.str();
        std::cout << "Call signalAddr: " << signalAddr << std::endl;
        std::list<ConnectInfoImpl>::iterator it;
        for (it = ConnectInfoImplList.begin(); it != ConnectInfoImplList.end(); it++) {
            if (it->Sender.SenderPtr== sender && it->Sender.SignalAddr== signalAddr) {
                FunPtrBase* recSlotInfo = (*it).RecSlotInfo;
                recSlotInfo->CallSlots(1, recSlotInfo,arg);
                //break;
            }
        }
    }
    //单例模式
    static MetaObj* getIns() {
        static MetaObj Ins;
        return &Ins;
    }
private:
    MetaObj() { ConnectInfoList.clear(); ConnectInfoImplList.clear(); };  //私有
    std::list<ConnectInfo> ConnectInfoList;//之前用宏定义字符串的
    std::list<ConnectInfoImpl> ConnectInfoImplList;
};

#define HAS_MEMBER(member)\
template<typename T, typename... Args> struct has_member_##member{\
private:\
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    template<typename U> static std::false_type Check(...);\
public:\
    enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(InitClass);
HAS_MEMBER(UpDateClass);

//原型
//void registClass(std::string objName, CBase* pT) {
//    //ObjMap[objName] = pT;
//    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
//}

//记住【模板可变参数】---可有可无
//检查类中是否含有 InitClass() 成员函数  如果类里面存在 但判断失败请检查是否成员函数的权限是公有（public）的 
template <typename T>
typename std::enable_if<has_member_InitClass<T>::value>::type registClass111(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------"<< std::endl;
}

//记住【模板可变参数】---可有可无
//检查类中是否含有 UpDateClass(const MsgInfo& msgInfo) 成员函数  如果类里面存在 但判断失败请检查是否成员函数的权限是公有（public）的 
template <typename T, typename... Args>
typename std::enable_if<has_member_UpDateClass<T, Args...>::value>::type registClass1(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------" << std::endl;
}

//记住【模板可变参数】---可有可无
//两者组合就完整了  如果类里面存在 但判断失败请检查是否成员函数的权限是公有（public）的 
template <typename T, typename... Args>
typename std::enable_if<has_member_InitClass<T>::value&& has_member_UpDateClass<T, Args...>::value>::type
registClass11(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------" << std::endl;
}


//也就是说目前类之间通信的方式有两种
//1.使用对象管理工厂 需要通信类之间提供公有的接口进行
//2.对象管理通知 需要通信类本身重写UpDateClass接口进行
//总结：想咋玩就咋玩---遵守原则大概率能避免屎山，珍爱生命远离屎山

//对象管理工厂---工厂模式
class ObjFactory {
public:
    //通过类名获取类
    CBase* getClassBase(std::string objName) {
        CBase* pT = nullptr;
        std::map<std::string, CBase*>::iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { pT = nullptr; }
        pT = it->second;
        std::cout << "get: " << objName << "   adress: " << (void*)pT << std::endl;
        return pT;
    }
    template<typename T>
    T* getClass(std::string objName) {
        CBase* pBT = nullptr;
        std::map<std::string, CBase*>::iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { pBT = nullptr; }
        else pBT = it->second;
        if (pBT == nullptr) { return nullptr; }
        std::cout << "get: " << objName << "   adress: " << (void*)pBT << std::endl;
        T* pT = dynamic_cast<T*>(pBT);
        return pT;
    }
    //注册类名称与类指针映射关系
    void registClass(std::string objName, CBase* pT) {
        ObjMap[objName] = pT;
        std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    }
    //判断类名是否存在
    bool objNameIsExist(std::string objName) {
        std::map<std::string, CBase*>::iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { return false; }
        return true;
    };
    //初始化类必要的资源
    void initObjClass() {
        std::map<std::string, CBase*>::iterator it;
        for (it = ObjMap.begin(); it != ObjMap.end(); it++) {
            it->second->InitClass();
        }
    };
    //单例模式
    static ObjFactory* getIns() {
        static ObjFactory Ins;
        return &Ins;
    }
private:
    ObjFactory() { ObjMap.clear(); };  //私有
    std::map<std::string, CBase*> ObjMap;
};

//对象管理通知---发布订阅（观察者模式）
class ObjNotify {
    typedef struct SubscribeInfo {
        SubscribeInfo() {};
        SubscribeInfo(CBase* pCBase, MsgType msgType) {
            PCBase = pCBase; MsgType = msgType;
        };
        CBase* PCBase = nullptr;
        MsgType MsgType = MsgType::Type0;
    }SubscribeInfo;
public:
    //订阅消息（考虑到高并发情况或许需要开启线程订阅）
    //订阅消息 放到队列里面去依次处理（那就考虑需要加锁了）
    void subscribeInfo(CBase* pCBase, const MsgType& msgType) {
        if (existSubscribe(pCBase, msgType) == true)//防止同一对象重复订阅同一类型（主题）消息
            return;
        SubscribeInfoList.push_back(SubscribeInfo(pCBase, msgType));
        std::cout << "---already Subscribe---" << std::endl;
    };
    //是否重复订阅
    bool existSubscribe(CBase* pCBase, const MsgType& msgType) {
        std::list<SubscribeInfo>::iterator it;
        for (it = SubscribeInfoList.begin(); it != SubscribeInfoList.end(); it++) {
            if (it->PCBase == pCBase && it->MsgType == msgType) {
                std::cout << "---exist Subscribe---" << std::endl;
                return true;
            }
        }
        std::cout << "---NO Subscribe---" << std::endl;
        return false;
    }
    //发布消息给指定订阅者（考虑到高并发情况或许需要开启线程发布）
    //发布消息 放到队列里面去依次处理（那就考虑需要加锁了）
    void PublishInfo(const MsgInfo& msgInfo) {
        std::list<SubscribeInfo>::iterator it;
        for (it = SubscribeInfoList.begin(); it != SubscribeInfoList.end(); it++) {
            if (it->MsgType == msgInfo.Msg_Type) {
                it->PCBase->UpDateClass(msgInfo);
                std::cout << "already Publish To---" << msgInfo << "  " << (void*)it->PCBase << std::endl;
            }
        }
        std::cout << "---All already Publish---" << msgInfo << std::endl;
    };
    //单例模式
    static ObjNotify* getIns() {
        static ObjNotify Ins;
        return &Ins;
    }
private:
    ObjNotify() { SubscribeInfoList.clear(); };  //私有
    std::list<SubscribeInfo> SubscribeInfoList;
};

template<typename T, typename ...Targs>
class CreateObj {
public:
    CreateObj(std::string objName, Targs&&...args) {
        if (ObjFactory::getIns()->objNameIsExist(objName)) {
            std::cout << "Create: " << objName << " Existing" << std::endl;
            return;
        }
        T* pT = new T(std::forward<Targs>(args)...);
        std::cout << "Create: " << objName << "   adress: " << (void*)pT << std::endl;
        ObjFactory::getIns()->registClass(objName, pT);
    };
    ~CreateObj() {};
};

#define CallFun(FunName)\
pT1->##FunName(std::forward<Targs>(args)...);

template<typename T1,typename T2, typename ...Targs>
T2 Invok(std::string target, std::string fun,Targs&&...args) {
    T1* pT1 = ObjFactory::getIns()->getClass<T1>(target);
    CallFun(fun.c_str());
}

#define CallFunEn(classType,className,retType,funName,...)\
classType* pT1 = ObjFactory::getIns()->getClass<classType>(#className);\
pT1->##funName(__VA_ARGS__);

#define CallMenFun(MemFun)\
template<typename T1, typename T2, typename ...Targs>\
T2 Invok##MemFun(std::string target, Targs&&...args) {\
    T1* pT1 = ObjFactory::getIns()->getClass<T1>(target);\
    pT1->##MemFun(std::forward<Targs>(args)...);\
}\

CallMenFun(InitClass);

#define CreateMemVar1(VARTYPE,VARNAME,DEFAULTVALUE) \
private: \
     VARTYPE VARNAME=DEFAULTVALUE; \
public: \
      void set##VARNAME(VARTYPE VARNAME##_) { VARNAME = VARNAME##_; }; \
      VARTYPE get##VARNAME() { return VARNAME; };

#define CreateMemVar2(VARTYPE,VARNAME) \
private: \
     VARTYPE VARNAME; \
public: \
      void set##VARNAME(VARTYPE VARNAME##_) { VARNAME = VARNAME##_; }; \
      VARTYPE get##VARNAME() { return VARNAME; };


#define CREATETYPE(CLASSTYPE,...) CreateObj<CLASSTYPE,__VA_ARGS__>

#define CREATEPARA(CLASSNAME,...) CreateObj##CLASSNAME(#CLASSNAME,__VA_ARGS__);

#define OBJINITCLASS ObjFactory::getIns()->initObjClass();

#define CREATESIGNAL(CLASSTYPE,FUNNAME,PARA1TYPE,PARA1)\
void FUNNAME(PARA1TYPE PARA1){\
     std::cout << #FUNNAME<<"("<<#PARA1TYPE<<" "<<#PARA1<<")"<<PARA1<< std::endl;\
    /*这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素*/ \
     void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&PARA1)) };\
     MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (CLASSTYPE::*)(int)>(&CLASSTYPE::FUNNAME), pBufferArr);\
}


//void signalFun(int a) {
//    std::cout << "void signalFun(int a): " << a << std::endl;
//    void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)) };//	这是一个指向  void* 的指针。 或者说pBufferArr是一个数组，里面存放void*元素 
//    MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(int)>(&WorkThrid::signalFun), pBufferArr);
//};


////只能暂且用这种方式包罗所有函数了
//void Fun1() { std::cout << "void Fun1()" << std::endl; return; };
//void Fun2(int) { std::cout << "void Fun2(int)" << std::endl; return; };
//int Fun3() { std::cout << "int Fun3()" << std::endl; return 1; };
//int Fun4(int) { std::cout << "int Fun4(int)" << std::endl; return 1; };
//
//namespace SM {
//    struct SFun1 {};
//    struct SFun2 {};
//    struct SFun3 {};
//    struct SFun4 {};
//}
//
//struct SInvokk {
//    //类型和值一定要弄清楚
//    //IDD 函数选择 Ret 函数返回值 Args函数参数（可变参数）
//    template <typename IDD, typename Ret, typename... Args>
//    typename std::enable_if<std::is_same<IDD, SM::SFun1>::value, Ret>::type Invoke(Args&&... args) {
//        return Fun1(std::forward<Args>(args)...);
//    }
//
//    template <typename IDD, typename Ret, typename... Args>
//    typename std::enable_if<std::is_same<IDD, SM::SFun2>::value, Ret>::type Invoke(Args&&... args) {
//        return Fun2(std::forward<Args>(args)...);
//    }
//
//    template <typename IDD, typename Ret, typename... Args>
//    typename std::enable_if<std::is_same<IDD, SM::SFun3>::value, Ret>::type Invoke(Args&&... args) {
//        return Fun3(std::forward<Args>(args)...);
//    }
//
//    template <typename IDD, typename Ret, typename... Args>
//    typename std::enable_if<std::is_same<IDD, SM::SFun4>::value, Ret>::type Invoke(Args&&... args) {
//        return Fun4(std::forward<Args>(args)...);
//    }
//};

class ReflexObjInit
{
public:
    ReflexObjInit() {};
    ~ReflexObjInit() {};
public:
    void InitObjFactory();
    void InitObjNotify();

    void Test();
};