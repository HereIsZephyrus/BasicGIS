/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-07 20:53:55
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-09 10:18:38
 * @FilePath: \GIS\Commander.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-07 20:53:55
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-08 13:59:51
 * @FilePath: \GIS\Commander.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
#include "Commander.h"
#include "Objects.h"
#include "GlobalVar.h"
#include<cmath>

int Commander::getCommand()
{
	if (MouseHit()) {
		mouse = GetMouseMsg();
        //if (illegalClick(mouse))
        //    return 0;
		switch (static_cast<int>(DictateArea(mouse)))
		{
		case static_cast<int>(Toolbar) : {//理论上在工具栏中可操作对象是按钮为充分必要的
            int CMD=onMenuMsg(mouse);
            if (CMD == 1)   {return 0;}
			break;
		}
		case static_cast<int>(Photo): {
            onDrawMsg(mouse);
            break;
        }
		default://out_of_range
			break;
		}
        UpdateStage(mouse);//更新鼠标对象状态
    }
	else {
		//failed
	}
	return 0;
}

Areas Commander::DictateArea(const MOUSEMSG& mouse)
{
	const int x = mouse.x, y = mouse.y;
    if (y<0 || y>_Height)
        return OUT_OF_RANGE;
	if (x>=0 && x<=_WTool) {
		return Toolbar;
	}
	if (x>_WTool && x< _Width) {
		return Photo;
	}
	return OUT_OF_RANGE;
}
vector<Point*>::iterator Commander::DictateButton(const MOUSEMSG &mouse)
{
    int x = mouse.x,y = mouse.y;
    if (x<_LButton || x>_LButton+_WButton)//所有的鼠标都是对齐的，
        return objList.end();
    if (y >= _UButton && y <= _UButton + _HButton)
        //加载底图
        return objList.begin();
    if (y >= _UButton + (_HButton + _GButton) && y <= _UButton + _HButton + (_HButton + _GButton))
        //新建矢量
        return objList.begin() + 1;
    if (y >= _UButton + (_HButton + _GButton) * 2 && y <= _UButton + _HButton + (_HButton + _GButton))
        //加载矢量
        return objList.begin() + 2;
    if (y >= _UButton + (_HButton + _GButton) * 3 && y <= _UButton + _HButton + (_HButton + _GButton) * 3)
        //保存矢量
        return objList.begin() + 3;
    if (y >= _UButton + (_HButton + _GButton) * 4 && y <= _UButton + _HButton + (_HButton + _GButton) * 4)
        //画点
        return objList.begin() + 4;
    if (y >= _UButton + (_HButton + _GButton) * 5 && y <= _UButton + _HButton + (_HButton + _GButton) * 5)
        //画线
        return objList.begin() + 5;
    if (y >= _UButton + (_HButton + _GButton) * 6 && y <= _UButton + _HButton + (_HButton + _GButton) * 6)
        //画多边形
        return objList.begin() + 6;
    if (y >= _UButton + (_HButton + _GButton) * 7 && y <= _UButton + _HButton + (_HButton + _GButton) * 7)
        //转换模式
        return objList.begin() + 7;
    if (y >= _UButton + (_HButton + _GButton) * 8 && y <= _UButton + _HButton + (_HButton + _GButton) * 8)
        //退出
        return objList.begin() + 8;
    return objList.end();
}

void Commander::UpdateStage(const MOUSEMSG &mouse)
{
    switch (mouse.uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        stage = Clicking;
        break;
    }
    case WM_RBUTTONDOWN:
    {
        stage = Clicking;
        break;
    }
    case WM_LBUTTONUP:
    {
        stage = Hold;
        break;
    }
    case WM_RBUTTONUP:
    {
        stage = Hold;
        break;
    }
    case WM_MOUSEMOVE: // 鼠标在移动时可能为绘制、移动和空载三种情况，但是没有额外动作
    {
        break;
    }
    default:
        // failed
        break;
    }
    return;
}

void Commander::TOclick(vector<Point*>::iterator obj,const MOUSEMSG& mouse,bool STATUS)
{
    switch (mouse.uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        (*obj)->ClickLeft(STATUS, mouse);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        (*obj)->ClickRight(STATUS, mouse);
        break;
    }
    default:
        break;
    }
    return;
}

vector<Point*>::iterator Commander::FocusObjID(const MOUSEMSG &mouse)
{
    const int x = mouse.x, y = mouse.y;
    for (vector<Point*>::iterator it = objList.begin(); it != objList.end(); it++)
    {
        if (typeid(*it) == typeid(Point)) // 指到点的范围内就可以，注意Point的X和Y是中心点
            if (Distance(x, y, (*it)->getX(), (*it)->getY())<(dynamic_cast<Point *>(*it))->getSize())
                return it;
        if (typeid(*it) == typeid(Line))
            if (Line::CheckEdges(x, y, dynamic_cast<Line *>(*it)))
                return it;
        if (typeid(it) == typeid(Polygen))// 通过鼠标x轴坐标经过的绘制边界的奇偶性，判断鼠标是否在对象中
            if ((Polygen::CalcLine(x, y, dynamic_cast<Polygen *>(*it)) & 1) != 0)
                return it;
        // 理论上Button对象不可能出现在Photo中，后期考虑写一个异常
    }
    return objList.end();
}

double Distance(const int& x1, const int& y1, const int& x2, const int& y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool CheckExceed(const Response* obj, bool style)
{
    if (style == 0)//Line
    {
        return false;
    }
    //Polygen
    return false;
}
bool illegalClick(const MOUSEMSG& mouse)
{
    if (mouse.uMsg == WM_MBUTTONDOWN ||
        mouse.uMsg == WM_MBUTTONUP ||
        mouse.uMsg == WM_MBUTTONDBLCLK ||
        mouse.uMsg == WM_LBUTTONDBLCLK ||
        mouse.uMsg == WM_RBUTTONDBLCLK ||
        mouse.uMsg == WM_MOUSEWHEEL)
        return true;//未定义的输入，不做处理，空转
    return false;
}
int Commander::onMenuMsg(const MOUSEMSG& mouse)
{
    if (mouse.uMsg != WM_LBUTTONUP) // 除了左键放开，在工具栏都是点着玩的
        return 0;
    obj = DictateButton(mouse);
    if (obj == objList.end())
        return 0;//没有点击在有效对象上
    //if ((*obj)->getID() > ButtonNum)//这是一个没有被定义的行为，要报个错
    //    return 0;
    if (!dynamic_cast<Button*>((*obj))->Press(stage, mouse, obj))
        return 1;
    return 0;
}
int Commander::onDrawMsg(const MOUSEMSG& mouse)
{
    if (mouse.uMsg == WM_RBUTTONUP)//这个动作是不被定义的，右键只会被用来删除，而删除是瞬间的
        return 0;
    if (mouse.uMsg == WM_LBUTTONUP)
    {
        if (stage != Drag)  return 0;
        else
        {
            stage = Hold;//结束拖动
            if (typeid(*obj) == typeid(Line))
                if (!CheckExceed(*obj, 0))
                {
                    // 越界了，非法拖动
                    return -1;
                }
            if (typeid(*obj) == typeid(Polygen))
                if (!CheckExceed(*obj, 1))
                {
                    // 越界了，非法拖动
                    return -1;
                }
        }
    }
    //剩下的鼠标输入状态只剩下左键按下，右键按下，鼠标移动
    switch (stage)
    {
    case Drawing: // 在绘制状态下，鼠标左键按下时，响应对象的ClickLeft()函数用以新建对象
    {
        TOclick(obj, mouse, DRAWING);
        break;
    }
    case Hold: // 空载状态，可能将更新已有矢量状态
    {
        vector<Point*>::iterator obj = FocusObjID(mouse);
        if (obj == objList.end())
            break;
        (*obj)->Suspend();//被focus的高亮显示
        TOclick(obj, mouse, EXISTED);
        break;
    }
    case Drag:
    {
        vector<Point*>::iterator obj = FocusObjID(mouse);
        if (obj == objList.end())
            break;
        (*obj)->Suspend();//被focus的高亮显示
        switch (mouse.uMsg)//查看鼠标信息看看是被拖动的哪一个状态
        {
        case WM_LBUTTONUP:
        {
            //开始拖动
            break;
        }
        case WM_LBUTTONDOWN://结束拖动
        {
            (*obj)->Move(mouse.x - (*obj)->getX(), mouse.y - (*obj)->getY());
            break;
        }
        case WM_MOUSEMOVE:
        {
            //拖动过程中，有精力写个异或线，先空着
            break;
        }
        }
        break;
    }
    case Clicking:
    {
        //感觉没啥好写， 现在也没想到解决鼠标响应冲突的办法
        break;
    }
    }
    return 0;
}