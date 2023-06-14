/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 22:10:24
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-12 10:25:34
 * @FilePath: \BasicGIS\Commander.h
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 13:47:16
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-11 14:25:28
 * @FilePath: \GIS\Commander.h
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-07 20:53:55
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-08 22:58:08
 * @FilePath: \GIS\Commander.h
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
	Response* FocusObjID(const int ,const int );
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
};

inline double Distance(const int &, const int &, const int &, const int &);

#endif // !_COMMANDER_H_
