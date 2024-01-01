#include "ReflexBase.h"
#include "WorkClass.h"
#include <functional>

//ֻ�����������ַ�ʽ�������к�����
void Fun1() { std::cout << "void Fun1()" << std::endl; return; };
void Fun2(int) { std::cout << "void Fun2(int)" << std::endl; return;};
int Fun3() { std::cout << "int Fun3()" << std::endl; return 1;};
int Fun4(int) { std::cout << "int Fun4(int)" << std::endl; return 1;};

namespace SM {
	struct SFun1 {};
	struct SFun2 {};
	struct SFun3 {};
	struct SFun4 {};
}

struct SInvokk {
	//���ͺ�ֵһ��ҪŪ���
	//IDD ����ѡ�� Ret ��������ֵ Args�����������ɱ������
	template <typename IDD, typename Ret, typename... Args>
	typename std::enable_if<std::is_same<IDD, SM::SFun1>::value, Ret>::type Invoke(Args&&... args) {
		return Fun1(std::forward<Args>(args)...);
	}

	template <typename IDD, typename Ret, typename... Args>
	typename std::enable_if<std::is_same<IDD, SM::SFun2>::value, Ret>::type Invoke(Args&&... args) {
		return Fun2(std::forward<Args>(args)...);
	}

	template <typename IDD, typename Ret, typename... Args>
	typename std::enable_if<std::is_same<IDD, SM::SFun3>::value, Ret>::type Invoke(Args&&... args) {
		return Fun3(std::forward<Args>(args)...);
	}

	template <typename IDD, typename Ret, typename... Args>
	typename std::enable_if<std::is_same<IDD, SM::SFun4>::value, Ret>::type Invoke(Args&&... args) {
		return Fun4(std::forward<Args>(args)...);
	}
};

//std::function<void()> f
//struct CallFun111 {
//};

//��ʼ�������������
void InitAllObj();

int main()
{
	//CallFun111<void> callFun(1);
	//callFun.Invoke();

	SInvokk sInvokk;
	sInvokk.Invoke<SM::SFun1, void>();
	sInvokk.Invoke<SM::SFun2, void, int>(1);
	sInvokk.Invoke<SM::SFun3, int>();
	std::cout<< sInvokk.Invoke<SM::SFun4, int, int>(1)<<std::endl;
	//SInvokk sInvokk1; SInvokk sInvokk2;
	//sInvokk1.Invoke<int,void>();
	//sInvokk2.Invoke<float, void, int>(1);

	InitAllObj();
	//����ָ������ �����༰����
	WorkClass1* pWorkClass1_1 = ObjFactory::getIns()->getClass<WorkClass1>("WorkClass1_1");
	//pWorkClass1->seta(100);
	//std::cout << "pWorkClass1->geta(): "<< pWorkClass1->geta() <<std::endl;
	//pWorkClass1->setsTa(ST(3,"1233"));
	//ST st = pWorkClass1->getsTa();
	ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type0, "msg---info"));
	ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type1, "msg---info"));
	//ObjNotify::getIns()->PublishInfo(MsgInfo(MsgType::Type2, "msg---info"));

	return 0;
}

void InitAllObj()
{
	ReflexObjInit reflexObjInit;
	reflexObjInit.InitObjFactory();
	//reflexObjInit.InitObjNotify();
	reflexObjInit.Test();
}
