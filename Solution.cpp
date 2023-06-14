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
#pragma omp single
#include "Solution.h"
#include "Commander.h"
#include "GlobalVar.h"
#include "Objects.h"

Squareness map(_WTool, 0, _Width - _WTool, _Height, WHITE);
Squareness toolbar(0, 0, _WTool, _Height, tColor);
Button loadButton(_LButton, _UButton, bColor,  _WButton, _HButton, Load);         // 加载底图按钮
Button newButton(_LButton, _UButton + (_HButton + _GButton) * 1, bColor,  _WButton,_HButton, New);          // 新建vec按钮
Button openButton(_LButton, _UButton + (_HButton + _GButton) * 2, bColor,  _WButton,_HButton, Open);         // 打开vec按钮
Button saveButton(_LButton, _UButton + (_HButton + _GButton) * 3, bColor,  _WButton,_HButton, Save);			// 保存vec按钮
Button drawButton1(_LButton, _UButton + (_HButton + _GButton) * 4, bColor,  _WButton,_HButton, DrawPoint);        // 画点按钮
Button drawButton2(_LButton, _UButton + (_HButton + _GButton) * 5, bColor,  _WButton,_HButton, DrawLine);        // 画线按钮
Button drawButton3(_LButton, _UButton + (_HButton + _GButton) * 6, bColor,  _WButton,_HButton, DrawPolygen);        // 画面按钮
Button switchButton(_LButton, _UButton + (_HButton + _GButton) * 7, bColor,  _WButton,_HButton, Switch);       // 转换模式按钮
Button exitButton(_LButton, _UButton + (_HButton + _GButton) * 8, bColor,  _WButton,_HButton, Exit);			// 退出按钮

void Initialize(Commander& cmder) {
	initgraph(_Width, _Height);
    {
        toolbar.AddText(Text((_WTool-_FONT*3)/2, 10, "工具栏", _FONT, RGB(75, 0, 130)));
        toolbar._Draw();
        map._Draw();
    }
    {
        using std::string;
        loadButton.Setinfo(R"(加载底图)");
        butList.push_back(&loadButton);        loadButton._Draw();
        newButton.Setinfo(R"(新建矢量)");
        butList.push_back(&newButton);        newButton._Draw();
        openButton.Setinfo(R"(加载矢量)");
        butList.push_back(&openButton);        openButton._Draw();
        saveButton.Setinfo(R"(保存矢量)");
        butList.push_back(&saveButton);        saveButton._Draw();
        drawButton1.Setinfo(R"(画    点)");
        butList.push_back(&drawButton1);        drawButton1._Draw();
        drawButton2.Setinfo(R"(画 折 线)");
        butList.push_back(&drawButton2);        drawButton2._Draw();
        drawButton3.Setinfo(R"(画多边形)");
        butList.push_back(&drawButton3);        drawButton3._Draw();
        switchButton.Setinfo(R"(选择模式)");
        butList.push_back(&switchButton);        switchButton._Draw();
        exitButton.Setinfo(R"(退出程序)");
        butList.push_back(&exitButton);        exitButton._Draw();
    }
    loadButton.Press(cmder.stage, cmder.getMouse(), cmder.getObj(),true);//加载一张图先
    setbkmode(TRANSPARENT);                                              // 设置背景模式为透明
    setbkcolor(eColor);                                    // 设置背景色为白色
    return;
}
void ShutDown(Commander& cmder) {
    for (auto it = objList.begin(); it != objList.end(); ++it)         delete* it; // 手动释放对象的内存
    for (auto it = elmList.begin(); it != elmList.end(); ++it)        delete* it; // 手动释放对象的内存
    objList.clear();
    elmList.clear();
    butList.clear();
	closegraph();
	return;
}