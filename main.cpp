// @主程序，只负责控制流程
#include "Solution.h"
#include "Objects.h"
#include "Errors.h"
#include "Commander.h"
Commander cmder;
int main(int argc,char *argv[]) {
	Initialize(cmder);
    bool bExit = false;
	while (!cmder.getCommand()) {}
	ShutDown(cmder);
	return 0;
}