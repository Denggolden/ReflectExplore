#include"ReflexBase.h"
#include "WorkClass.h"

//初始化所有所需对象
void InitAllObj();

int main()
{
	InitAllObj();
	//访问指定对象 根据类及类名
	WorkClass *pWorkClass1= ObjFactory<WorkClass>::getIns()->getClass("WorkClass1");
	pWorkClass1->seta(100);
	std::cout << "pWorkClass1->geta(): "<< pWorkClass1->geta() <<std::endl;
	pWorkClass1->setsTa(ST(3,"1233"));
	ST st = pWorkClass1->getsTa();
	return 0;
}

void InitAllObj()
{
	ReflexObjInit reflexObjInit;
	reflexObjInit.InitObj();
}
