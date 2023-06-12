/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 22:10:24
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-12 14:40:22
 * @FilePath: \BasicGIS\Solution.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
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
#include "Objects.h"

Squareness map(_WTool, 0, _Width - _WTool, _Height, WHITE);
Squareness toolbar(0, 0, _WTool, _Height, BLUE);
Button loadButton(_LButton, _UButton, _BOLD_, YELLOW, _HButton, _WButton, Load);         // 加载底图按钮
Button newButton(_LButton, _UButton + (_HButton + _GButton) * 1, _BOLD_, YELLOW, _HButton, _WButton, New);          // 新建vec按钮
Button openButton(_LButton, _UButton + (_HButton + _GButton) * 2, _BOLD_, YELLOW, _HButton, _WButton, Open);         // 打开vec按钮
Button saveButton(_LButton, _UButton + (_HButton + _GButton) * 3, _BOLD_, YELLOW, _HButton, _WButton, Save);			// 保存vec按钮
Button drawButton1(_LButton, _UButton + (_HButton + _GButton) * 4, _BOLD_, YELLOW, _HButton, _WButton, DrawPoint);        // 画点按钮
Button drawButton2(_LButton, _UButton + (_HButton + _GButton) * 5, _BOLD_, YELLOW, _HButton, _WButton, DrawLine);        // 画线按钮
Button drawButton3(_LButton, _UButton + (_HButton + _GButton) * 6, _BOLD_, YELLOW, _HButton, _WButton, DrawPolygen);        // 画面按钮
Button switchButton(_LButton, _UButton + (_HButton + _GButton) * 7, _BOLD_, YELLOW, _HButton, _WButton, Switch);       // 转换模式按钮
Button exitButton(_LButton, _UButton + (_HButton + _GButton) * 8, _BOLD_, YELLOW, _HButton, _WButton, Exit);			// 退出按钮
constexpr int ButtonNum = 9; // 一共有9个按钮

void Initialize(Commander& cmder) {
	initgraph(_Width, _Height);
    //绘制Toolbar
    //...
    {
    elmList.push_back( &toolbar);     toolbar._Draw();
    elmList.push_back(&map);          map._Draw();
    }
    {
        //绘制按钮
        butList.push_back(&exitButton);        exitButton._Draw();
        butList.push_back(&drawButton1);        drawButton1._Draw();
        butList.push_back(&drawButton2);        drawButton2._Draw();
        butList.push_back(&drawButton3);        drawButton3._Draw();
        butList.push_back(&openButton);        openButton._Draw();
        butList.push_back(&newButton);        newButton._Draw();
        butList.push_back(&saveButton);        saveButton._Draw();
        butList.push_back(&loadButton);        loadButton._Draw();
    }
    //绘制地图底框
    loadButton.Press(cmder.getStage(), cmder.getMouse(), cmder.getObj());//加载一张先

	return;
}
void ShutDown(Commander& cmder) {
    objList.clear();
    elmList.clear();
    butList.clear();
	closegraph();
	return;
}