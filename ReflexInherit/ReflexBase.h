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
    MsgType Msg_Type = MsgType::Type0;//消息类型
    std::string Msg_Content = "";//消息内容
    friend std::ostream& operator << (std::ostream&, const MsgInfo& P) {
        std::cout << "(" << (int)P.Msg_Type << "," << P.Msg_Content << ")";
        return std::cout;
    }
}MsgInfo;

class CBase {//所有类的基类
public:
    CBase() {};
    virtual ~CBase() {};
    void setObjName(const std::string& objName) { ObjName = objName; };
    std::string getObjName() { return ObjName; };
public:
    /*用于替换掉以前需要在构造函数里面需要执行的内容
    （也就是说现在构造函数里面基本上什么代码都不需要写）（非得写到构造函数里面的代码除外  或许某些机制需要这样的）
    InitClass用于减少模块间初始化相互依赖的问题
    如需要各种不同的构造函数走不同的构造函数逻辑，可以在类的构造函数里面及类里面增加一个成员变量加以区分
    当走InitClass时候也可用这个标志位加以区分，从而把类完整的初始化完毕*/
    virtual void InitClass() {};
    //用于统一通知
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
//总结：想咋玩就咋玩---遵守原则大概率能避免屎山

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

