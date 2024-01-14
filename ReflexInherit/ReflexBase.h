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
    MsgType Msg_Type = MsgType::Type0;//��Ϣ����
    std::string Msg_Content = "";//��Ϣ����
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

class CBase {//������Ļ���
public:
    CBase() { SignalSlotInfoList.clear(); };
    virtual ~CBase() {};
    void setObjName(const std::string& objName) { ObjName = objName; };
    std::string getObjName() { return ObjName; };
public:
    /*�����滻����ǰ��Ҫ�ڹ��캯��������Ҫִ�е����� ����Ա������ʼ����Ҫд�ڹ��캯������
    ��Ҳ����˵���ڹ��캯�����������ʲô���붼����Ҫд(�����˳�Ա������ʼ���⡿)�����ǵ�д�����캯������Ĵ������  ����ĳЩ������Ҫ�����ģ�
    InitClass���ڼ���ģ����ʼ���໥����������
    ����Ҫ���ֲ�ͬ�Ĺ��캯���߲�ͬ�Ĺ��캯���߼�����������Ĺ��캯�����漰����������һ����Ա������������
    ����InitClassʱ��Ҳ���������־λ�������֣��Ӷ����������ĳ�ʼ�����*/
    virtual void InitClass() {};
    //����ͳһ֪ͨ
    virtual void UpDateClass(const MsgInfo& msgInfo) {};
    //�����źŲ�Ԥ��̽��
    //idd ����ѡ��ۺ�����
    //arg ���ڴ��ݲ�����
    virtual void NotifyClass(int idd, void** arg) {};
    virtual void AddSignalSlotInfoList(std::string funName, SignalSlotType signalSlotType) {
        int id = SignalSlotInfoList.size()+1;
        std::list<SignalSlotInfo>::iterator it;
        for (it = SignalSlotInfoList.begin(); it != SignalSlotInfoList.end(); it++) {
            if (it->FunName == funName) {
                std::cout << "�źŻ�۲����ظ���ӣ�����" << std::endl;
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
        std::cout << "�����źŲ����ڣ�����" << std::endl;
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
        std::cout << "����ۺ��������ڣ�����" << std::endl;
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
        Compare,/*��ʱδ֪*/
        NumOperations/*��ʱδ֪*/
    };
public:
    FunPtrBase() {};
    ~FunPtrBase() {};
    //����1  �����麯����̬
public:
    virtual void CallSlots(int which, FunPtrBase* this_, void** arg) {};
//    //����2  ���ú���ָ��ص�(�ƺ�ʧ��)
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
            call<Ret,Args...>(arg);break;//Ret���������
                 //case Compare:
                 //    *ret = *reinterpret_cast<Func*>(a) == static_cast<QSlotObject*>(this_)->function;
                 //    break;
        case NumOperations:;
        }
    };
    template<typename Ret>//�޲�
    Ret call(void** arg) {
        (Tptr->*FunPtr)();
    }
    template<typename Ret,typename Arg1>//�������� 1��
    Ret call(void** arg) {
        (Tptr->*FunPtr)(*reinterpret_cast<Arg1*>(arg[0]));
    }
    template<typename Ret, typename Arg1, typename Arg2>//�������� 2��
    Ret call(void** arg) {
        (Tptr->*FunPtr)(*reinterpret_cast<Arg1*>(arg[0]), *reinterpret_cast<Arg2*>(arg[1]));
    }
    //����׷�Ӳ�����ʱ��ֻҪ����������׷�Ӿ�����
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
            call<Ret, Args...>(arg); break;//Ret���������
            //case Compare:
            //    *ret = *reinterpret_cast<Func*>(a) == static_cast<QSlotObject*>(this_)->function;
            //    break;
        case NumOperations:;
        }
    };
    template<typename Ret>//�޲�
    Ret call(void** arg) {
        FunPtr();
    }
    template<typename Ret, typename Arg1>//�������� 1��
    Ret call(void** arg) {
        FunPtr(*reinterpret_cast<Arg1*>(arg[0]));
    }
    template<typename Ret, typename Arg1, typename Arg2>//�������� 2��
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
    //ִ���źŲ����Ӻ���
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
    //���в������͵� ��ͨ��Ա���� ��Ȼ����ֵvoid 
    template<typename T1, typename T2, typename... Args>
    void Connect(T1* send, void (T1::* signalFunPtr)(Args...), T2* rec, void (T2::* slotFunPtr)(Args...)) {
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr;std::string signalAddr = saddr.str();
        std::cout << "Connect signalAddr: " << signalAddr << std::endl;
        ConnectInfoImplList.push_back(ConnectInfoImpl(SenderInfoImpl(send, signalAddr), new FunPtrSlot<T2,void, Args...>(rec, slotFunPtr)));
    }
    //���в������͵� lambda ���ʽ ��Ȼ����ֵvoid 
    template<typename T1, typename F, typename... Args>
    void Connect(T1* send, void (T1::* signalFunPtr)(Args...), T1* sendR, F const& f) {//������˵�Ļ��������������Բ�Ҫ
        void* addr = *reinterpret_cast<void**>(&signalFunPtr);
        std::stringstream saddr; saddr << addr; std::string signalAddr = saddr.str();
        std::cout << "Connect signalAddr: " << signalAddr << std::endl;
        ConnectInfoImplList.push_back(ConnectInfoImpl(SenderInfoImpl(send, signalAddr), new FunPtrSlotLam<T1,void, F, Args...>(sendR, f)));
    }
    //�Ƴ�����
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
                //ConnectInfoImplList.erase(it++);//����
                //break;
            }
            else
                ++it;
        }
    }

    //ִ���ź�֪ͨ�ۺ��������ĺ���  ֮ǰ��
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
    //ִ���ź�֪ͨ�ۺ��������ĺ���  ���ڵ�  ���ӷ���ǿ����
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
    //����ģʽ
    static MetaObj* getIns() {
        static MetaObj Ins;
        return &Ins;
    }
private:
    MetaObj() { ConnectInfoList.clear(); ConnectInfoImplList.clear(); };  //˽��
    std::list<ConnectInfo> ConnectInfoList;//֮ǰ�ú궨���ַ�����
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

//ԭ��
//void registClass(std::string objName, CBase* pT) {
//    //ObjMap[objName] = pT;
//    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
//}

//��ס��ģ��ɱ������---���п���
//��������Ƿ��� InitClass() ��Ա����  ������������ ���ж�ʧ�������Ƿ��Ա������Ȩ���ǹ��У�public���� 
template <typename T>
typename std::enable_if<has_member_InitClass<T>::value>::type registClass111(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------"<< std::endl;
}

//��ס��ģ��ɱ������---���п���
//��������Ƿ��� UpDateClass(const MsgInfo& msgInfo) ��Ա����  ������������ ���ж�ʧ�������Ƿ��Ա������Ȩ���ǹ��У�public���� 
template <typename T, typename... Args>
typename std::enable_if<has_member_UpDateClass<T, Args...>::value>::type registClass1(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------" << std::endl;
}

//��ס��ģ��ɱ������---���п���
//������Ͼ�������  ������������ ���ж�ʧ�������Ƿ��Ա������Ȩ���ǹ��У�public���� 
template <typename T, typename... Args>
typename std::enable_if<has_member_InitClass<T>::value&& has_member_UpDateClass<T, Args...>::value>::type
registClass11(std::string objName, CBase* pT) {
    std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    std::cout << "--------------------------------regist:-----------------------------------------" << std::endl;
}


//Ҳ����˵Ŀǰ��֮��ͨ�ŵķ�ʽ������
//1.ʹ�ö�������� ��Ҫͨ����֮���ṩ���еĽӿڽ���
//2.�������֪ͨ ��Ҫͨ���౾����дUpDateClass�ӿڽ���
//�ܽ᣺��զ���զ��---����ԭ�������ܱ���ʺɽ���䰮����Զ��ʺɽ

//���������---����ģʽ
class ObjFactory {
public:
    //ͨ��������ȡ��
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
    //ע������������ָ��ӳ���ϵ
    void registClass(std::string objName, CBase* pT) {
        ObjMap[objName] = pT;
        std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    }
    //�ж������Ƿ����
    bool objNameIsExist(std::string objName) {
        std::map<std::string, CBase*>::iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { return false; }
        return true;
    };
    //��ʼ�����Ҫ����Դ
    void initObjClass() {
        std::map<std::string, CBase*>::iterator it;
        for (it = ObjMap.begin(); it != ObjMap.end(); it++) {
            it->second->InitClass();
        }
    };
    //����ģʽ
    static ObjFactory* getIns() {
        static ObjFactory Ins;
        return &Ins;
    }
private:
    ObjFactory() { ObjMap.clear(); };  //˽��
    std::map<std::string, CBase*> ObjMap;
};

//�������֪ͨ---�������ģ��۲���ģʽ��
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
    //������Ϣ�����ǵ��߲������������Ҫ�����̶߳��ģ�
    //������Ϣ �ŵ���������ȥ���δ����ǾͿ�����Ҫ�����ˣ�
    void subscribeInfo(CBase* pCBase, const MsgType& msgType) {
        if (existSubscribe(pCBase, msgType) == true)//��ֹͬһ�����ظ�����ͬһ���ͣ����⣩��Ϣ
            return;
        SubscribeInfoList.push_back(SubscribeInfo(pCBase, msgType));
        std::cout << "---already Subscribe---" << std::endl;
    };
    //�Ƿ��ظ�����
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
    //������Ϣ��ָ�������ߣ����ǵ��߲������������Ҫ�����̷߳�����
    //������Ϣ �ŵ���������ȥ���δ����ǾͿ�����Ҫ�����ˣ�
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
    //����ģʽ
    static ObjNotify* getIns() {
        static ObjNotify Ins;
        return &Ins;
    }
private:
    ObjNotify() { SubscribeInfoList.clear(); };  //˽��
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
    /*����һ��ָ��  void* ��ָ�롣 ����˵pBufferArr��һ�����飬������void*Ԫ��*/ \
     void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&PARA1)) };\
     MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (CLASSTYPE::*)(int)>(&CLASSTYPE::FUNNAME), pBufferArr);\
}


//void signalFun(int a) {
//    std::cout << "void signalFun(int a): " << a << std::endl;
//    void* pBufferArr[] = { const_cast<void*>(reinterpret_cast<const void*>(&a)) };//	����һ��ָ��  void* ��ָ�롣 ����˵pBufferArr��һ�����飬������void*Ԫ�� 
//    MetaObj::getIns()->ActivateSlotImpl(this, static_cast<void (WorkThrid::*)(int)>(&WorkThrid::signalFun), pBufferArr);
//};


////ֻ�����������ַ�ʽ�������к�����
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
//    //���ͺ�ֵһ��ҪŪ���
//    //IDD ����ѡ�� Ret ��������ֵ Args�����������ɱ������
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