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
    if (MouseHit())
    {
        mouse = GetMouseMsg();
        //if (illegalClick(mouse))    return 0;!!会影响cmder状态的回位！
       //MessageBox(NULL, L"Area", L"Area", NULL);
		switch (static_cast<int>(DictateArea(mouse)))
		{
		case static_cast<int>(Toolbar) : {//理论上在工具栏中可操作对象是按钮为充分必要的
            //MessageBox(NULL, L"ToolBar", L"ToolBar", NULL);
            int CMD=onMenuMsg(mouse);
            if (CMD == 1)   return 1;//退出按钮
			break;
		}
		case static_cast<int>(Photo): {
            //MessageBox(NULL, L"Photo", L"Photo", NULL);
            onDrawMsg(mouse);
            break;
        }
		default://out_of_range
			break;
		}
        //UpdateStage(mouse);//更新鼠标对象状态
        //不对需要修改，应当绑定在行为上与行为同步改变符合自觉
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
Button* Commander::DictateButton(const MOUSEMSG &mouse)
{
    int x = mouse.x,y = mouse.y;
    if (x<_LButton || x>_LButton+_WButton)//所有的鼠标都是对齐的，
        return nullptr;
    if (y >= _UButton && y <= _UButton + _HButton)
        //加载底图
        return *butList.begin();
    if (y >= _UButton + (_HButton + _GButton) && y <= _UButton + _HButton + (_HButton + _GButton))
        //新建矢量
        return *(butList.begin() + 1);
    if (y >= _UButton + (_HButton + _GButton) * 2 && y <= _UButton + _HButton + (_HButton + _GButton)*2 )
        //加载矢量
        return *(butList.begin() + 2);
    if (y >= _UButton + (_HButton + _GButton) * 3 && y <= _UButton + _HButton + (_HButton + _GButton) * 3)
        //保存矢量
        return *(butList.begin() + 3);
    if (y >= _UButton + (_HButton + _GButton) * 4 && y <= _UButton + _HButton + (_HButton + _GButton) * 4)
        //画点
        return *(butList.begin() + 4);
    if (y >= _UButton + (_HButton + _GButton) * 5 && y <= _UButton + _HButton + (_HButton + _GButton) * 5)
        //画线
        return *(butList.begin() + 5);
    if (y >= _UButton + (_HButton + _GButton) * 6 && y <= _UButton + _HButton + (_HButton + _GButton) * 6)
        //画多边形
        return *(butList.begin() + 6);
    if (y >= _UButton + (_HButton + _GButton) * 7 && y <= _UButton + _HButton + (_HButton + _GButton) * 7)
        //转换模式
        return *(butList.begin() + 7);
    if (y >= _UButton + (_HButton + _GButton) * 8 && y <= _UButton + _HButton + (_HButton + _GButton) * 8)
        //退出
        return *(butList.begin() + 8);
    return nullptr;
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

void Commander::TOclick(Point* obj,const MOUSEMSG& mouse,bool STATUS)
{
    switch (mouse.uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        obj->ClickLeft(STATUS, mouse);
        break;
    }
    case WM_RBUTTONDOWN:
    {
        obj->ClickRight(STATUS, mouse);
        break;
    }
    default:
        break;
    }
    return;
}

Point* Commander::FocusObjID(const int x,const int y)
{
    for (vector<Point*>::iterator it = objList.begin(); it != objList.end(); it++)
    {
        if (typeid(*it) == typeid(Point*)) // 指到点的范围内就可以，注意Point的X和Y是中心点
            if (Distance(x, y, (*it)->getX(), (*it)->getY())<(dynamic_cast<Point *>(*it))->getSize())
                return *it;
        if (typeid(*it) == typeid(Line*))
            if (Line::CheckEdges(x, y, dynamic_cast<Line *>(*it)))
                return *it;
        if (typeid(*it) == typeid(Polygen*))// 通过鼠标x轴坐标经过的绘制边界的奇偶性，判断鼠标是否在对象中
            if ((Polygen::CalcLine(x, y, dynamic_cast<Polygen *>(*it)) & 1) != 0)
                return *it;
        // 理论上Button对象不可能出现在Photo中，后期考虑写一个异常
    }
    return nullptr;
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
    Button* focusedObj = DictateButton(mouse);
    
    if (focusedObj == nullptr) //没有点击在有效对象上
    {
        for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it)
            if ((*it)->getFocus())  (*it)->UnSuspend();
        return 0;
    }
    if (focusedObj->getID() > ButtonNum)//这是一个没有被定义的行为，要报个错
        return -1;
    if (!focusedObj->getFocus())//意味着有可能有其他对象没有被unfocus
        for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it)
            if ((*it)->getFocus())  (*it)->UnSuspend();
    focusedObj->Suspend();
    //if (mouse.uMsg == WM_MOUSEMOVE && stage == Hold)    return 0;
    return focusedObj->Press(stage, mouse, obj, false);//Press返回1为Exit，不返回1则不退出
}
int Commander::onDrawMsg(const MOUSEMSG& mouse)
{
    Point* focusedObj = FocusObjID(mouse.x, mouse.y);
    if (obj != nullptr && obj != focusedObj)//如果鼠标不在之前focus的对象上
        obj->UnSuspend();//取消之前的focus
    if (focusedObj != nullptr)//如果鼠标在新的对象上
        focusedObj->Suspend();//focus新的对象
    obj = focusedObj;//更新focus对象
    if (mouse.uMsg == WM_RBUTTONUP)//这个动作是不被定义的，右键只会被用来删除，而删除是瞬间的
        return 0;
    if (mouse.uMsg == WM_LBUTTONUP)
    {
        if (stage != Drag)  return 0;
        else
        {
            stage = Hold;//结束拖动
            if (typeid(*obj) == typeid(Line))
                if (!CheckExceed(obj, 0))
                {
                    // 越界了，非法拖动
                    return -1;
                }
            if (typeid(*obj) == typeid(Polygen))
                if (!CheckExceed(obj, 1))
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
        Point* obj = FocusObjID(mouse.x,mouse.y);
        if (obj == nullptr)
            break;
        TOclick(obj, mouse, EXISTED);
        break;
    }
    case Drag:
    {
        Point* obj = FocusObjID(mouse.x,mouse.y);
        if (obj == nullptr)
            break;
        switch (mouse.uMsg)//查看鼠标信息看看是被拖动的哪一个状态
        {
        case WM_LBUTTONUP:
        {
            //开始拖动
            break;
        }
        case WM_LBUTTONDOWN://结束拖动
        {
            //obj->Move(mouse.x - obj->getX(), mouse.y - obj->getY());
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
