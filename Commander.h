// @控制器的类定义
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

static bool Moving = false;
static int movingX = 0, movingY = 0;
static int originX = 0, originY = 0;
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