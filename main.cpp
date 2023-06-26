/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 22:10:24
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-17 15:16:55
 * @FilePath: \BasicGIS\main.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
// @主程序，只负责控制流程
#include "Solution.h"
#include "Objects.h"
#include "Errors.h"
#include "Commander.h"
Commander cmder;
int main(int argc,char *argv[]) {
	Initialize(cmder);
	while (!cmder.getCommand()) {}
	ShutDown(cmder);
	return 0;
}