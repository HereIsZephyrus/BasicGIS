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
constexpr unsigned int _Height = 720;
constexpr unsigned int _Width = 1080;
constexpr unsigned int _WTool = 200;
constexpr unsigned int _WButton = 120;
constexpr unsigned int _LButton = 40; // 按钮的左侧开始坐标
constexpr unsigned int _HButton = 30;
constexpr unsigned int _UButton = 35; // 按钮的上方开始坐标
constexpr unsigned int _GButton = 15; // 按钮上下排列间隔
constexpr unsigned int _WMSG = 50;
class Point;
class Button;
class Squareness;
extern vector<Point *> objList;
extern vector<Squareness *> elmList;
extern vector<Button *> butList;

#endif // !_GLOBAL_H_