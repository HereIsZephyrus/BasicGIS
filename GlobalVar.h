/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 22:10:24
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-13 23:15:15
 * @FilePath: \BasicGIS\GlobalVar.h
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
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
constexpr unsigned int _UButton = 50; // 按钮的上方开始坐标
constexpr unsigned int _GButton = 15; // 按钮上下排列间隔
constexpr unsigned int _WMSG = 50;
constexpr unsigned int _Ellipse=2;
constexpr unsigned int _FONT = 14;
constexpr int ButtonNum = 9;                    // 一共有9个按钮
constexpr COLORREF(eColor) = RGB(250, 250, 250);//橡皮擦颜色
constexpr COLORREF(tColor) = RGB(0, 191, 255);//工具栏颜色
constexpr COLORREF(bColor) = YELLOW;//按钮颜色
constexpr COLORREF(fColor) = RGB(210, 105, 30);//填充颜色
class Point;
class Button;
class Squareness;
extern vector<Point *> objList;
extern vector<Squareness *> elmList;
extern vector<Button *> butList;

#endif // !_GLOBAL_H_