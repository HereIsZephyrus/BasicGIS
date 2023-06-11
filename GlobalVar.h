#pragma once
#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include<fstream>
#include<iostream>
#include<vector>
#include<cstring>
#include<string>

using std::vector;
using std::string;

namespace streaming {
    using std::fstream;
    using std::ofstream;
    using std::ifstream;
    using std::endl;
    using std::cin;
    using std::cout;
}
using namespace streaming;

namespace Vecors {
    class Response;
    class Display;
    class Button;
    vector<Response*> objList;
    vector<Display*> elmList;
    vector<Button*> butList;
}
namespace WINDOW_CONST {
    constexpr unsigned int _Height = 720;
    constexpr unsigned int _Width = 1080;
    constexpr unsigned int _WTool = 200;
    constexpr unsigned int _WButton = 120;
    constexpr unsigned int _LButton = 40;//按钮的左侧开始坐标
    constexpr unsigned int _HButton = 30;
    constexpr unsigned int _UButton = 35;//按钮的上方开始坐标
    constexpr unsigned int _GButton = 15;//按钮上下排列间隔
    constexpr unsigned int _WMSG = 50;
}
namespace Window_Object {
    using namespace WINDOW_CONST;
    class Button;
    class Squareness;
    Squareness map();
    Squareness toolbar();
    Button exitButton();  // 退出按钮
    Button drawButton1(); // 画点按钮
    Button drawButton2(); // 画线按钮
    Button drawButton3(); // 画面按钮
    Button openButton();  // 打开vec按钮
    Button newButton();   // 新建vec按钮
    Button saveButton();  // 保存vec按钮
    Button loadButton();  // 加载底图按钮
    Button switchButton();  // 转换模式按钮
    constexpr int ButtonNum = 9;//一共有9个按钮
}
#endif // !_GLOBAL_H_
