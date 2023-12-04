#pragma once
#include <iostream>
#include <map>

template<typename T>
class ObjFactory {
public:
    //通过类名获取类
    T* getClass(std::string objName) {
        T* pT= nullptr;
        std::map<std::string, T*>::template iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { pT= nullptr; }
        pT = it->second;
        std::cout << "get: " << objName << "   adress: " << (void*)pT << std::endl;
        return pT;
    }
    //注册类名称与类指针映射关系
    void registClass(std::string objName, T* pT) {
        ObjMap[objName] = pT;
        std::cout << "regist: " << objName << "   adress: " << (void*)pT << std::endl;
    }
    //判断类名是否存在
    bool objNameIsExist(std::string objName) { 
        std::map<std::string, T*>::template iterator it = ObjMap.find(objName);
        if (it == ObjMap.end()) { return false; }
        return true;
    };
    //单例模式
    static ObjFactory* getIns() {
        static ObjFactory Ins;
        return &Ins;
    }
private:
    ObjFactory() { ObjMap.clear(); };  //私有
    std::map<std::string, T*> ObjMap;
};

template<typename T, typename ...Targs>
class CreateObj {
public:
    CreateObj(std::string objName, Targs...args) {
        if (ObjFactory<T>::getIns()->objNameIsExist(objName)) {
            std::cout << "Create: " << objName<<" Existing" << std::endl;
            return;
        }
        T* pT = new T(std::forward<Targs>(args)...);
        std::cout << "Create: " << objName << "   adress: " << (void*)pT << std::endl;
        ObjFactory<T>::getIns()->registClass(objName, pT);
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


class ReflexObjInit
{
public:
    ReflexObjInit() {};
    ~ReflexObjInit() {};
public:
    void InitObj();
};

