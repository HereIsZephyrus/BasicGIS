/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-08 13:50:54
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-09 10:59:31
 * @FilePath: \GIS\Solution.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
#include "Solution.h"
#include "Commander.h"
#include "GlobalVar.h"
using namespace Vecors;
using namespace Window_Object;

void Initialize(const Commander& cmder) {
	initgraph(_Width, _Height);
    //绘制Toolbar
    //...
    elmList.push_back(&toolbar);
    {
        //绘制按钮
        butList.push_back(&exitButton);
        exitButton._Draw();
        butList.push_back(&drawButton1);
        drawButton1._Draw();
        butList.push_back(&drawButton2);
        drawButton2._Draw();
        butList.push_back(&drawButton3);
        drawButton3._Draw();
        butList.push_back(&openButton);
        openButton._Draw();
        butList.push_back(&newButton);
        newButton._Draw();
        butList.push_back(&saveButton);
        saveButton._Draw();
        butList.push_back(&loadButton);
        loadButton._Draw();
    }
    //绘制地图底框
   
    elmList.push_back(&map);

    loadButton.Press(cmder.stage,cmder.mouse,cmder.obj);//加载一张先
    
	return;
}
void ShutDown(const Commander& cmder) {
    objList.clear();
    elmList.clear();
    butList.clear();
	closegraph();
	return;
}