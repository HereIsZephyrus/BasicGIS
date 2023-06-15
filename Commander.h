/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 22:10:24
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-15 23:24:53
 * @FilePath: \BasicGIS\Commander.h
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */

#pragma once
#ifndef _COMMANDER_H_
#define _COMMANDER_H_
#include "Enums.h"
#include<typeinfo>
#include<graphics.h>
#include<fstream>
#include<iostream>
#include<conio.h>
#include<vector>
using std::vector;

class Response;
class Display;
class Point;
class Squareness;
class Button;
class Polygen;
class Line;
class Commander {
	friend class Button;
private:
	MOUSEMSG mouse;
	Response* obj;//指向当前响应对象的指针
protected:
	Areas DictateArea(const MOUSEMSG&);
	Button* DictateButton(const MOUSEMSG&);
	void TOclick(Response*,const MOUSEMSG &,bool);
	int onMenuMsg(const MOUSEMSG &);
	int onDrawMsg(const MOUSEMSG &);
public:
	Status stage;
	Commander() :mouse{}, stage{ Hold }{
		FlushMouseMsgBuffer();
	}
	int getCommand();
	MOUSEMSG &getMouse() { return mouse; }
	Response*& getObj(){ return obj; }
	Response* FocusObjID(const int, const int);
};

inline double Distance(const int &, const int &, const int &, const int &);

#endif // !_COMMANDER_H_
