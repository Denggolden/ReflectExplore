#pragma once
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <type_traits>

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

class CBase {//������Ļ���
public:
    CBase() {};
    virtual ~CBase() {};
    void setObjName(const std::string& objName) { ObjName = objName; };
    std::string getObjName() { return ObjName; };
public:
    /*�����滻����ǰ��Ҫ�ڹ��캯��������Ҫִ�е�����
    ��Ҳ����˵���ڹ��캯�����������ʲô���붼����Ҫд�����ǵ�д�����캯������Ĵ������  ����ĳЩ������Ҫ�����ģ�
    InitClass���ڼ���ģ����ʼ���໥����������
    ����Ҫ���ֲ�ͬ�Ĺ��캯���߲�ͬ�Ĺ��캯���߼�����������Ĺ��캯�����漰����������һ����Ա������������
    ����InitClassʱ��Ҳ���������־λ�������֣��Ӷ����������ĳ�ʼ�����*/
    virtual void InitClass() {};
    //����ͳһ֪ͨ
    virtual void UpDateClass(const MsgInfo& msgInfo) {};
private:
    std::string ObjName;
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
//�ܽ᣺��զ���զ��---����ԭ�������ܱ���ʺɽ

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


class ReflexObjInit
{
public:
    ReflexObjInit() {};
    ~ReflexObjInit() {};
public:
    void InitObjFactory();
    void InitObjNotify();
};

