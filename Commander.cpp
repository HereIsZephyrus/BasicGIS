 // @Description:做控制器的实现，负责图形交互的操作
#include "Commander.h"
#include "Objects.h"
#include "GlobalVar.h"
#include "Errors.h"
#include<cmath>

int Commander::getCommand()
{
	if (MouseHit())
	{
		mouse = GetMouseMsg();
		switch (static_cast<int>(DictateArea(mouse)))
		{
		case static_cast<int>(Toolbar) : {//理论上在工具栏中可操作对象是按钮为充分必要的
			int CMD=onMenuMsg(mouse);
			if (CMD == 1)   return 1;//退出按钮
			break;
		}
		case static_cast<int>(Photo): {
			onDrawMsg(mouse);
			break;
		}
		default://out_of_range
			break;
		}
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
	for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it) {
		if ((x >= (*it)->getX() && x <= (*it)->getX() + (*it)->getWidth()) &&
			(y >= (*it)->getY() && y <= (*it)->getY() + (*it)->getHeight()))
			if ((*it)->getDraw())
				return *it;
	}
	return nullptr;
}

void Commander::TOclick(Response* obj,const MOUSEMSG& mouse,bool STATUS)
{
	switch (mouse.uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		obj->ClickLeft(STATUS, mouse);
		for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it)
			if ((*it)->getType() == Switch)
			{
				Button* res = *it;
				res->Setinfo("选择模式"); res->_Draw();
				stage = Hold;
				break;
			}
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

Response* Commander::FocusObjID(const int x,const int y)
{
	for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		if (!(*it)->getDraw())	continue;
		if (dynamic_cast<Point*>(*it) != nullptr)
			if (Distance(x, y, (*it)->getX(), (*it)->getY()) < (dynamic_cast<Point*>(*it))->getSize())
				return *it;
		if (dynamic_cast<Polygen*>(*it) != nullptr)
		{
			Polygen* polygen = dynamic_cast<Polygen*>(*it);
			if ((Polygen::CalcLine(x, y, polygen) & 1) != 0)
				return polygen;
			for (vector<Point>::iterator p = polygen->points.begin(); p != polygen->points.end(); ++p)
				if (Distance(x, y, p->getX(), p->getY()) < p->getSize())
					return &(*p);
		}
		if (dynamic_cast<Line*>(*it) != nullptr)
		{
            Line *line = dynamic_cast<Line*>(*it);
			if (Line::CheckEdges(x, y, line))
				return line;
			for (vector<Point>::iterator p = line->points.begin(); p != line->points.end(); ++p)
                if (Distance(x, y, p->getX(), p->getY()) < p->getSize())
                    return &(*p);
		}
	}
	return nullptr;
}

double Distance(const int& x1, const int& y1, const int& x2, const int& y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int Commander::onMenuMsg(const MOUSEMSG& mouse)
{
	Button* focusedObj = DictateButton(mouse);
	if (focusedObj == nullptr) //没有点击在有效对象上
	{
		for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it) {
			if ((*it)->getType() == Finish)	continue;
			if ((*it)->getFocus())  (*it)->UnSuspend();
		}
		return 0;
	}
	if (focusedObj->getID() > ButtonNum)//这是一个没有被定义的行为，要报个错
		return -1;
	if (focusedObj->getFocus()==false)//意味着有可能有其他对象没有被unfocus
		for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it)
			if ((*it)->getDraw() && (*it)->getFocus())  (*it)->UnSuspend();
	focusedObj->Suspend();
	return focusedObj->Press(stage, mouse, obj, false);//Press返回1为Exit，不返回1则不退出
}
int Commander::onDrawMsg(const MOUSEMSG& mouse)
{
	if (stage == Drawing)
	{
		TOclick(obj, mouse, DRAWING);
		return 0;
	}
	if (Moving == true)
	{
		obj->Move(mouse.x - movingX, mouse.y - movingY);
		Flush();
		movingX = mouse.x; movingY = mouse.y;
		if (mouse.uMsg == WM_LBUTTONUP)//结束拖动
		{
			Moving = false;
			if (dynamic_cast<Point*>(obj) != nullptr)
			{
				Point* point = dynamic_cast<Point*>(obj);
				if (point->getFather() != point->getID())//是边中或者线中的一点
					for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
						if ((*it)->getID() == point->getFather())
						{
							if (dynamic_cast<Line*>(*it) != nullptr)
							{
								Polygen* line = dynamic_cast<Polygen*>(*it);
								for (vector<Borden>::iterator b = (line->borders).begin(); b != (line->borders).end(); ++b)
								{
									if ((*b).getX() == originX&& (*b).getY() == originY)
                                        (*b).setStart(point->getX(), point->getY());
                                    if ((*b).getTermX() == originX&& (*b).getTermY() == originY)
                                        (*b).setTerm(point->getX(), point->getY());
                                }
							}
                            if (dynamic_cast<Polygen*>(*it) != nullptr)
                            {
                                Polygen *poly = dynamic_cast<Polygen *>(*it);
                                for (vector<Borden>::iterator b = (poly->borders).begin(); b != (poly->borders).end(); ++b)
                                {
                                    if ((*b).getX() == originX && (*b).getY() == originY)
                                        (*b).setStart(point->getX(), point->getY());
                                    if ((*b).getTermX() == originX && (*b).getTermY() == originY)
                                        (*b).setTerm(point->getX(), point->getY());
                                }
                            }
                        }
			}
		}
		return 0;
	}
	Response* focusedObj = FocusObjID(mouse.x, mouse.y);
	if (focusedObj == nullptr)//|| obj != focusedObj
		for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
			if ((*it)->getFocus())  (*it)->UnSuspend();//如果鼠标不在之前focus的对象上
	if (focusedObj != nullptr)//如果鼠标在新的对象上
		focusedObj->Suspend();//focus新的对象
	obj = focusedObj;//更新focus对象
	if (obj == nullptr)	return 0;
	if (stage == Drag && mouse.uMsg == WM_LBUTTONDOWN) //Moving=false
	{
		Moving = true;
		movingX = mouse.x; movingY = mouse.y;
        originX=obj->getX();originY=obj->getY();
		return 0;
	}
	if (stage == Hold)
		TOclick(obj, mouse, EXISTED);
	return 1;//failed
}