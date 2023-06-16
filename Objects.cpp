/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-11 13:47:16
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-11 14:44:55
 * @FilePath: \GIS\Objects.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
/***
 * @Author: ChanningTong Channing_TongCN@outlook.com
 * @Date: 2023-06-07 20:53:55
 * @LastEditors: ChanningTong Channing_TongCN@outlook.com
 * @LastEditTime: 2023-06-07 21:25:06
 * @FilePath: \GIS\Objects.cpp
 * @Description:
 * @
 * @Copyright (c) 2023 by ChanningTong, All Rights Reserved.
 */
#include "Objects.h"
#include "GlobalVar.h"
#include "Errors.h"
#include "Commander.h"
#include "Enums.h"
#include <windows.h>
#include <fstream>
#include <iostream>

vector<Response*> objList;
vector<Squareness*> elmList;
vector<Button*> butList;
unsigned int Response::count = 0;
unsigned int Display::count = 0;
static wstring filename{};
using std::istream;
using std::ostream;
using std::endl;
IMAGE img;

void Text::Print(COLORREF backColor)
{
	//删除原有文字
	setbkcolor(BLACK); // 将背景色设置为黑色
	outtextxy(X, Y, L" "); // 在指定位置输出一个空格
	int len = MultiByteToWideChar(CP_ACP, 0, contain.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, contain.c_str(), -1, wstr, len);
	//settextstyle(size);
	setbkcolor(backColor);
	settextcolor(BLACK);
	outtextxy(X, Y, wstr);
	delete[] wstr;
	return;
}
void Squareness::AddText(Text texts)
{
	msg.push_back(texts);
	return;
}
int Squareness::_Draw()
{
	drawed = true;
	int x = X,y=Y;
	setfillcolor(color);
	fillrectangle(X,Y,X+width,Y+height);
	for (vector<Text>::iterator it = msg.begin(); it != msg.end(); ++it)
	{
		(*it).Print(color);
		y = y + (*it).getSize()+_dy;
	}
	return 0;
}
int Squareness::_Delete()
{
	drawed = false;
    Flush();
	//clearrectangle(X,Y,X+width,Y+height);
    //ReDraw(X,X+width,Y,Y);
    //ReDraw(X, X + width, Y+height, Y+height);
    //ReDraw(X, X, Y, Y + height);
    //ReDraw(X + width, X + width, Y, Y + height);
    return 0;
}

int Point::ClickLeft(bool Status, const MOUSEMSG& mouse)
{
	if (Status == EXISTED)
	{
		DisplayInfo();
		return 0;
	}
	X = mouse.x; Y = mouse.y;
	_Draw();
	return 0;
}
int Point::ClickRight(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED && CastWarning("您确定要删除该点吗?") == true) //显示文本框确认（利用短路特性）
		return 0;
	switch (type)
	{
	case PointType::POINT:
		{
		_Delete();
		break;
		}
	case PointType::LINE:
		{
		Line* Fa=FindLine(father);
		vector<Point> temp = Fa->points;
		int loc = 0;
		for (vector<Point>::iterator p = temp.begin(); p != temp.end(); ++p, ++loc)
			if (sqrt((mouse.x - (*p).getX()) * (mouse.x - (*p).getX()) + (mouse.y - (*p).getY()) * (mouse.y - (*p).getY())) <= (*p).getSize())
			{
				Fa->_DeletePoint((Fa->points).begin() + loc,loc);
				break;
			}
		break;
		}
	case PointType::POLYGEN:
		{
		Polygen* Fa=FindPolygen(father);
		vector<Point> temp;
		for (auto it = (Fa->points).begin(); it != (Fa->points).end(); ++it)
				temp.push_back(*it);
		int loc = 0;
		for (vector<Point>::iterator p = temp.begin(); p != temp.end(); ++p, ++loc)
			if (sqrt((mouse.x - (*p).getX()) * (mouse.x - (*p).getX()) + (mouse.y - (*p).getY()) * (mouse.y - (*p).getY())) <= (*p).getSize())
			{
				Fa->_DeletePoint((Fa->points).begin() + loc,loc);
				break;
			}
		break;
		}
		default:
		{
		//failed
		break;
		}
	}
	return 0;
}
int Point::Suspend()
{
	if (drawed == false)	return 0;
	focused = true;
	size= _SIZE_ +2;
	setfillcolor(sColor);
	fillcircle(X, Y, size);
	return 0;
}
int Point::UnSuspend()
{
	focused = false;
    Flush();
    return 0;
}
int Point::_Draw()
{
	drawed = true;
	size = _SIZE_;
	setlinecolor(fColor);
	setlinestyle(PS_SOLID, _BOLDER_);
	circle(X, Y, size);
    setlinestyle(PS_NULL, _BOLDER_);
    return 0;
}
int Point::_Delete()
{
	if (!drawed)//failed
		return 1;
	drawed = false;
	color = 0;  size = _SIZE_;
    Flush();
    //ReDraw(X - size/2, X - size/2, Y - size/2, Y + size/2);
    //ReDraw(X - size/2, X + size/2, Y - size/2, Y - size/2);
    //ReDraw(X + size/2, X + size/2, Y - size/2, Y + size/2);
    //ReDraw(X - size/2, X + size/2, Y + size/2, Y + size/2);
	return 0;
}

int Borden::_Draw()
{
	drawed = true;
	setlinecolor(fColor);
	setlinestyle(PS_SOLID, _BOLD_);
	line(X, Y, termX, termY);
    setlinestyle(PS_NULL, _BOLDER_);
    return 0;
}
int Borden::_Delete()
{
	drawed = false;
    Flush();
    //setrop2(R2_NOP);//用背景色填充画笔，因为是静态图像所以像素覆盖可以实现擦除
    //line(X, Y, termX, termY);
    //setrop2(R2_COPYPEN);
	return 0;
}
double Borden::CalcX(const int &y)
{
    double k = (double)(termX - X)/(termY - Y);
    return k*(y-Y)+X;
}

int Polygen::ClickLeft(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED)
	{
		DisplayInfo();
		return 0;
	}
	MOUSEMSG tmpMsg = mouse;
	while (!_AddPoint(tmpMsg)) {
		_Draw();
		MOUSEMSG Catch=GetMouseMsg();
		while (Catch.uMsg!=WM_LBUTTONDOWN){
            // 如果鼠标在点上，把这个点删除
			if (Catch.uMsg == WM_RBUTTONDOWN && points.size() >= 2)
			{
				vector<Point> temp = points;
				int loc = 0;
				for (vector<Point>::iterator p = temp.begin(); p != temp.end(); ++p,++loc)
					if (sqrt((mouse.x - (*p).getX()) * (mouse.x - (*p).getX()) + (mouse.y - (*p).getY()) * (mouse.y - (*p).getY())) <= (*p).getSize())
					{
						_DeletePoint(points.begin()+loc, loc);
						break;
					}
			}
			Catch = GetMouseMsg();
        }
		tmpMsg = Catch;
	}
	_Draw();
	return 0;
}
int Polygen::ClickRight(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED && CastWarning("您确定要删除该多边形吗?") == true) //显示文本框确认（利用短路特性）
		return 0;
	_Delete();
	return 0;
}
int Polygen::Suspend()
{
	if (drawed == false)	return 0;
	focused = true;
	//填充一个多边形出来，透明度低一些
	const size_t num = points.size();
    if (num < 3)    return 1;//failed
    vector<POINT> pts;
	for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it)
    {
		POINT pt;
		pt.x = (*it).getX();	pt.y = (*it).getY();
        pts.push_back(pt);
    }
    setfillcolor(fColor);
    //setalpha(128);                  // 设置透明度为 50%
    fillpolygon(&pts[0], static_cast<int>(num));
    return 0;
}
int Polygen::UnSuspend()
{
	focused = false;
    Flush();
    return 0;
}
double Polygen::CalcArea()
{
    //利用三角形划分求多边形面积(有待调试，不保真)
    double area=0;
    for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it)
    {
        area+=(*it).getX()*(*(it+1)).getY()-(*it).getY()*(*(it+1)).getX();
    }
    return area/2;
}
int Polygen::_AddPoint(const MOUSEMSG& mouse)
{
	if (mouse.x<_WTool || mouse.x>_Width || mouse.y<0 || mouse.y>_Height)
	{
		CastError("请在图框范围内绘制");
		return 0;
	}
	if (points.empty()) {
		points.push_back(Point(mouse.x, mouse.y, color,PointType::POLYGEN));
		points.back().SetFather(id);
		return 0;
	}
	vector<Point>::iterator p = points.begin();
	const int x1 = mouse.x, y1 = mouse.y, x2 = (*p).getX(), y2 = (*p).getY();
	if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= _SIZE_+2)
	{
		_Bind(points.begin(), points.end() - 1);//手动封闭了
		return 1;
	}
	else
	{
		points.push_back(Point(x1,y1,color,PointType::POLYGEN));
		points.back().SetFather(id);
		_Bind(points.end() - 1, points.end() - 2);
	}
	return 0;
}
int Polygen::_DeletePoint(vector<Point>::iterator p,int loc)
{
	//从队列中删除一个点，然后重新绑定边
	if (points.size() == 3 &&
		sqrt((points.begin()->getX() - points.back().getX()) * (points.begin()->getX() - points.back().getX()) + (points.begin()->getY() - points.back().getY()) * (points.begin()->getY() - points.back().getY())) <= (*p).getSize())
	{
		_Delete();
		return 0;
	}
	vector<Borden>::iterator b = borders.begin() + loc;
	if (p==points.begin())
	{
		_Erase(&*(borders.end() - 1));//先删后面
		if (sqrt((points.begin()->getX() - points.back().getX()) * (points.begin()->getX() - points.back().getX()) + (points.begin()->getY() - points.back().getY()) * (points.begin()->getY() - points.back().getY())) <= (*p).getSize())
			_Erase(&*borders.begin());
		(*p)._Delete();
		points.erase(p);
		_Bind(points.begin(), points.end() - 1);
		return 0;
	}
	if (p == points.end() - 1)
	{
		_Erase(&*(borders.end() - 1));
		_Erase(&*(borders.end() - 1));//上面那个b已经删掉了,下边顺序移动一位！
		(*p)._Delete();
		points.erase(p);
		Flush();
		_Bind(points.begin(), points.end() - 1);
		return 0;
	}
	_Erase(&*b);
	b = borders.begin() + loc-1;
	_Erase(&*b);//上面那个b已经删掉了,下边顺序移动一位！
	_Bind(p-1, p+1);//这里不会越界，因为上面的if已经判断过了（挺妙的）
	(*p)._Delete();
	points.erase(p);
	Flush();
	return 0;
}
int Polygen::_Bind(vector<Point>::iterator p1, vector<Point>::iterator p2)
{
	const int x1 = ( * p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
	borders.push_back(Borden(x1, y1, x2, y2));
	borders.back().SetDrawed(true);
	return 0;
}
int Polygen::_Erase(Borden* b)
{
	//这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
	for (vector<Borden>::iterator it = borders.begin(); it != borders.end(); ++it)
		if (&*it == b)
		{
			(*it).SetDrawed(false);
			borders.erase(it);
            //setrop2(R2_NOP); // 用背景色填充画笔，因为是静态图像所以像素覆盖可以实现擦除
            //line(b->getX(), b->getY(), b->getTermX(), b->getTermY());
            //setrop2(R2_COPYPEN);
            return 0;
		}
	return 0;
}
int Polygen::_Draw()
{
	drawed = true;
	for (vector<Point>::iterator p = points.begin(); p != points.end(); ++p)			(*p)._Draw();
	for (vector<Borden>::iterator b = borders.begin(); b != borders.end(); ++b)	(*b)._Draw();
	return 0;
}
int Polygen::_Delete()
{
	drawed = false;
    for (vector<Point>::iterator p = points.begin(); p != points.end(); ++p)				(*p)._Delete();
    for (vector<Borden>::iterator b = borders.begin(); b != borders.end(); ++b)	    (*b)._Delete();
    Flush();
	return 0;
}
int Polygen::CalcLine(const int& x, const int& y, Polygen* poly)
{
	int calc = 0;
	for (vector<Borden>::iterator it = (poly->borders).begin(); it != (poly->borders).end(); ++it)
		if (	(y - (*it).getY()) * (y - (*it).getTermY()) < 0 && ((*it).CalcX(y)<=x))
			++calc;
	return calc;
}

int Line::ClickLeft(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED)
	{
		DisplayInfo();
		return 0;
	}
	MOUSEMSG tmpMsg = mouse;
	extern Button finishButton;
    extern Commander cmder;
	while (!_AddPoint(tmpMsg)) {
		_Draw();
		if (points.size() >= 2) 	finishButton._Draw();
		MOUSEMSG Catch = GetMouseMsg();
		while (Catch.uMsg != WM_LBUTTONDOWN)
		{
			const int x=Catch.x,y=Catch.y;
			if ((x >= finishButton.getX() && x <= finishButton.getX() + finishButton.getWidth()) &&
				(y >= finishButton.getY() && y <= finishButton.getY() + finishButton.getHeight()))
				if (finishButton.getFocus()==false && points.size() >= 2)
					finishButton.Suspend();
			else
				if (finishButton.getFocus() == true && points.size() >= 2)
					finishButton.UnSuspend();
            if (Catch.uMsg == WM_RBUTTONDOWN && points.size() >= 2)
            {
                // 如果鼠标在点上，把这个点删除
                bool Inpoint=false;
				vector<Point> temp = points;
                int loc = 0;
				for (vector<Point>::iterator p = temp.begin(); p != temp.end(); ++p, ++loc)
					if (sqrt((mouse.x - (*p).getX()) * (mouse.x - (*p).getX()) + (mouse.y - (*p).getY()) * (mouse.y - (*p).getY())) <= (*p).getSize())
					{
						_DeletePoint(points.begin() + loc, loc);
						break;
					}
                if (!Inpoint)
                {
					MOUSEMSG tempmsg = Catch;
					tempmsg.uMsg = WM_LBUTTONDOWN;
					tempmsg.x = finishButton.getX() + 1;	tempmsg.y = finishButton.getY() + 1;
                    finishButton.Press(cmder.stage,tempmsg,cmder.getObj(), true);
					Flush();
                    break;
                }
            }
			Catch = GetMouseMsg();
		}
		tmpMsg = Catch;
	}
	_Draw();
	finishButton._Delete();
	return 0;
}
int Line::ClickRight(bool Status, const MOUSEMSG &mouse)
{
    if (Status == EXISTED && CastWarning("您确定要删除该线吗?") == true)
		    return 0;
    _Delete();
	Flush();
	return 0;
}
int Line::Suspend()
{
	if (drawed == false)	return 0;
	focused = true;
    for (vector<Borden>::iterator b = borders.begin(); b != borders.end(); ++b)
    {
        setlinecolor(fColor);
        setlinestyle(PS_SOLID, _BOLD_+2);
        line(b->getX(), b->getY(), b->getTermX(), b->getTermY());
        setlinestyle(PS_NULL, _BOLDER_);
    }
	return 0;
}
int Line::UnSuspend()
{
	focused = false;
    Flush();
    return 0;
}
double Line::CalcLength()
{
    double res=0;
    for (vector<Point>::iterator it = points.begin()+1; it != points.end(); ++it)
        res += sqrt(((*it).getX() - (*(it - 1)).getX()) * ((*it).getX() - (*(it - 1)).getX()) + ((*it).getY() - (*(it - 1)).getY()) * ((*it).getY() - (*(it - 1)).getY()));
	return 0;
}
bool Line::CheckEdges(const int& x, const int& y, Line* obj)
{
	Line* line = dynamic_cast<Line*>(obj);
	for (vector<Borden>::iterator it = (line->borders).begin(); it != (line->borders).end(); ++it)
	{
		double getX = (*it).CalcX(y);
		if (getX >= x - _dx && getX <= x + _dx && (y - (*it).getY()) * (y - (*it).getTermY()) < 0)
			return true;
	}
	return false;
}
int Line::_AddPoint(const MOUSEMSG& mouse)
{
	extern Button finishButton;
	const int x = mouse.x, y = mouse.y;
	if (x<_WTool || x>_Width || y<0 || y>_Height)
	{
		if ((x >= finishButton.getX() && x <= finishButton.getX() + finishButton.getWidth()) &&
			(y >= finishButton.getY() && y <= finishButton.getY() + finishButton.getHeight()))
			return 1;
		CastError("请在图框范围内绘制");
		return 0;
	}
	if (points.empty()) {
		points.push_back(Point(mouse.x, mouse.y, color,  PointType::POLYGEN));
		points.back().SetFather(id);
		return 0;
	}
	points.push_back(Point(x, y, color, PointType::POLYGEN));
	points.back().SetFather(id);
	_Bind(points.end() - 1, points.end() - 2);
	return 0;
}
int Line::_DeletePoint(vector<Point>::iterator p,int loc)
{
	// 从队列中删除一个点，然后重新绑定边
	if (p == points.begin())
	{
		_Erase(&*borders.begin());
		(*p)._Delete();
		points.erase(p);
		Flush();
		return 0;
	}
	if (p == points.end() - 1)
	{
		_Erase(&*(borders.end() - 1));
		(*p)._Delete();
		points.erase(p);
		Flush();
		return 0;
	}
	vector<Borden>::iterator b = borders.begin() + loc;
	_Erase(&*b);
	b = borders.begin() + loc-1;
	_Erase(&*b);//顺序移动
	_Bind(p - 1, p + 1);//这里不会越界，因为上面的if已经判断过了（挺妙的）
	(*p)._Delete();
	points.erase(p);
	Flush();
	return 0;
}
int Line::_Draw()
{
	drawed = true;
	for (vector<Point>::iterator p = points.begin(); p != points.end(); ++p)	(*p)._Draw();
	for (vector<Borden>::iterator b = borders.begin(); b != borders.end(); ++b)	(*b)._Draw();
	return 0;
}
int Line::_Bind(vector<Point>::iterator p1, vector<Point>::iterator p2)
{
	const int x1 = (*p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
	borders.push_back(Borden(x1, y1, x2, y2));
	borders.back().SetDrawed(true);
	return 0;
}
int Line::_Erase(Borden* b)
{
	//这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
    for (vector<Borden>::iterator it = borders.begin(); it != borders.end(); ++it)
        if (&*it == b)
        {
			(*it).SetDrawed(false);
            borders.erase(it);
            //setrop2(R2_NOP); // 用背景色填充画笔，因为是静态图像所以像素覆盖可以实现擦除
            //line(b->getX(), b->getY(), b->getTermX(), b->getTermY());
            //setrop2(R2_COPYPEN);
            return 0;
        }
    return 0;
}
int Line::_Delete()
{
	drawed = false;
    for (vector<Point>::iterator p = points.begin(); p != points.end(); ++p)    (*p)._Delete();
    for (vector<Borden>::iterator b = borders.begin(); b != borders.end(); ++b)    (*b)._Delete();
    Flush();
	return 0;
}

int Button::ClickLeft(bool Status, const MOUSEMSG &mouse)
{
	//定义到Press中了，其实Button不该由Response派生过来，或者Response基类承载的接口溢出了，吸取教训。
	return 0;
}
int Button::ClickRight(bool Status, const MOUSEMSG &mouse)
{
	//右键响应功能没有被定义！可以弹出个对话框给一个提示
	//MessageBox()//user guide
	return 0;
}
int Button::Suspend()
{
	focused = true;
	color=RGB(138,43,226);//紫色
    _Draw();
	return 0;
}
int Button::UnSuspend()
{
	focused = false;
    color=bColor;//黄色
    _Draw();
    return 0;
}
int Button::Press(Status& stage, const MOUSEMSG& mouse, Response*& obj,bool Force) {
	if (mouse.uMsg != WM_LBUTTONUP && Force==false) // 除了左键放开，在工具栏都是点着玩的
		return 0;
	if (stage == Drawing) {
		CastError("无效绘制！请先完成绘制");
		return 0;
	}
	if (btype == Exit)
		return 1;
	using std::ofstream;
	using std::ifstream;
	using std::ios_base;
	switch (btype)
	{
		case Load:
		{
			extern Squareness map;
			map._Draw();
			wchar_t buffer[100] = { 0 };
			InputBox(buffer, 100, L"请输入图片路径与名称");
			std::wstring photoName = buffer;
			if (LoadPhoto(photoName))
			{
				//fail to load
				//memset(buffer, 0, sizeof(buffer));
				CastError("请输入正确的文件路径和文件名！");
				//InputBox(buffer, 100, L"请输入图片路径与名称");
				//photoName = buffer;
			}
			break;
		}
		case New:
		{
			TCHAR name[MAX_PATH] = { 0 };
			InputBox(name, 50, L"请输入文件名称（不包含后缀名）");
			filename=wstring(name);
			filename +=  L".vec";
			ofstream fp(filename);
			SaveToFile(fp);
			fp.close();
			break;
		}
		case Open:
		{
			TCHAR name[MAX_PATH] = { 0 };
			InputBox(name, 50, L"请输入文件名称（不包含后缀名）");
			filename = wstring(name);
			filename += L".vec";
			ifstream fp;
			fp.open(filename);
			while (fp.is_open() == false) {
				CastError("请输入正确文件名称（不包含后缀名）");
				return 0;
			}
			LoadFromFile(fp);
			fp.close();
			break;
		}
		case Save:
		{
			ofstream fp;
			if (filename.size() == 0)
			{
				TCHAR name[MAX_PATH] = { 0 };
				InputBox(name, 50, L"请输入文件名称（不包含后缀名）");
				filename = wstring(name);
				filename += L".vec";
			}
			fp.open(filename);
			if (fp.is_open() == false)//不被允许的逻辑错误
			{
				CastError("保存失败！请联系管理员ChanningTong@gmail.com");
				fp.close();
				break;
			}
			SaveToFile(fp);
			fp.close();
			break;
		}
		case Switch:
		{
			if (stage == Drag)
			{
				stage = Hold;
				Setinfo("选择模式"); _Draw();
				break;
			}
			if (stage == Hold)
			{
				stage = Drag;
				Setinfo("拖动模式"); _Draw();
				break;
			}
		}
		case DrawPoint:
		{
			stage = Drawing;
			const int x = mouse.x;
			objList.push_back(dynamic_cast<Response*>(new Point()));
			obj = objList.back();
			break;
		}
		case DrawLine:
		{
			stage = Drawing;
			const int x = mouse.x;
			objList.push_back(dynamic_cast<Response*>(new Line()));
			obj = objList.back();
			break;
		}
		case DrawPolygen:
		{
			stage = Drawing;
			const int x = mouse.x;
			objList.push_back(dynamic_cast<Response*>(new Polygen()));
			obj = objList.back();
			break;
		}
		default: {
			break;
		}
	}
	return 0;
}
int Button::_Draw()
{
	drawed = true;
	setfillcolor(color);
	fillroundrect(X, Y, X + width, Y + height, _Ellipse, _Ellipse);
	info.Print(color);
	return 0;
}
int Button::_Delete()
{
	drawed = false;
	setfillcolor(tColor);
	fillroundrect(X - _dx, Y - _dy, X + _dx + width, Y + _dy + height, _Ellipse, _Ellipse);
	return 0;
}
int Button::LoadPhoto(std::wstring& name)
{
	bool failed = false;
	int len = WideCharToMultiByte(CP_ACP, 0, name.c_str(), -1, NULL, 0, NULL, NULL);
	char* str = new char[len];
	WideCharToMultiByte(CP_ACP, 0, name.c_str(), -1, str, len, NULL, NULL);
	int len2 = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len2];
	MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, len2);
	LPCTSTR lname = wstr;
	loadimage(&img, lname);
	if (img.getwidth() == 0 && img.getheight() == 0)
		failed = true;
	else
		putimage(_WTool, 0, &img);
	delete[] str;
	delete[] wstr;
	return failed;
}
void Button::SaveToFile(ofstream& fp)
{
	if (!fp.is_open())
	{
		MessageBox(NULL, L"Open File Failed.", L"ERROR...", MB_ICONERROR);
		return;
	}
    for (vector<Response*>::iterator it=objList.begin(); it!=objList.end(); ++it)
        fp << *(*it);
	return;
}
void Button::LoadFromFile(ifstream& fp)
{
	for (auto it = objList.begin(); it != objList.end(); ++it)         delete* it; // 手动释放对象的内存
	for (auto it = elmList.begin(); it != elmList.end(); ++it)        delete* it; // 手动释放对象的内存
	objList.clear();
	elmList.clear();
    while (!fp.eof())
    {
        Response *pshp = Response::FindClone(fp); // findClone的实现见下
        if (pshp)
            objList.push_back(pshp);
    }
	for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
		(*it)->_Draw();
	for (vector<Squareness*>::iterator it = elmList.begin(); it != elmList.end(); ++it)
		(*it)->_Draw();
	return;
}
void Button::Setinfo(std::string msg)
{
	info = Text(X + (width - _FONT * 4) / 2, Y + (height - _FONT) / 2, msg, _FONT, BLACK);
	return;
}

Line* FindLine(int ID)
{
	for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
		if ((*it)->getID() == ID)
			return dynamic_cast<Line*>(* it);
	//failed
	return nullptr;
}
Polygen* FindPolygen(int ID)
{
	for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
		if ((*it)->getID() == ID)
			return dynamic_cast<Polygen*>(*it);
	//failed
	return nullptr;
}

int Point::Move(const int& dx, const int& dy)
{
	if ((X + dx > _Width) || (X + dx < _WTool) || (Y + dy > _Height) || (Y + dy < 0))
		return 1;
	X = X + dx;
	Y = Y + dy;
	//对对象进行移动，并用EdgeError类处理非法移动
	return 0;
}
int Borden::Move(const int& dx, const int& dy)
{
	if ((X + dx > _Width) || (X + dx < _WTool) || (Y + dy > _Height) || (Y + dy < 0))
		return 1;
	if ((termX + dx > _Width) || (termX + dx < _WTool) || (termY + dy > _Height) || (termY + dy < 0))
		return 1;
	X = X + dx;	termX = termX + dx;
	Y = Y + dy;	termY = termY + dy;
	//对对象进行移动，并用EdgeError类处理非法移动
	return 0;
}
int Polygen::Move(const int& dx, const int& dy)
{
	for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it)
		(*it).Move(dx, dy);
	for (vector<Borden>::iterator it = borders.begin(); it != borders.end(); ++it)
		(*it).Move(dx, dy);
	//对对象进行移动，并用EdgeError类处理非法移动
	return 0;
}
int Line::Move(const int& dx, const int& dy)
{
	for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it)
		(*it).Move(dx, dy);
	for (vector<Borden>::iterator it = borders.begin(); it != borders.end(); ++it)
		(*it).Move(dx, dy);
	//对对象进行移动，并用EdgeError类处理非法移动
	return 0;
}

void Button::DisplayInfo() const
{
    // 右键显示个guide信息
    return;
}
void Borden::DisplayInfo() const
{
    return;
}
void Point::DisplayInfo() const
{
    const int n = 2;
    Squareness Msg(X + _dx, Y + _dy, _WMSG, _FONT * n);
    { // n
        Msg.AddText(Text());
        Msg.AddText(Text());
    }
    if (!drawed)
        Msg._Draw();
    else
        Msg._Delete();
    return;
}
void Polygen::DisplayInfo() const
{
    const int n = 2;
    Squareness Msg(X + _dx, Y + _dy, _WMSG, _SIZE_ * n);
    { // n
        Msg.AddText(Text());
        Msg.AddText(Text());
    }
    Msg._Draw();
    return;
}
void Line::DisplayInfo() const
{
    const int n = 2;
    Squareness Msg(X + _dx, Y + _dy, _WMSG, _SIZE_ * n);
    { // n
        Msg.AddText(Text());
        Msg.AddText(Text());
    }
    Msg._Draw();
    return;
}

void Flush()
{
    putimage(_WTool, 0, &img);
    for (vector<Response*>::iterator it = objList.begin(); it != objList.end(); ++it)
        if ((*it)->getDraw()==true)
        {
            (*it)->_Draw();
            if ((*it)->getFocus()==true)
                (*it)->Suspend();
        }
    return;
}
void ReDraw(const int& x1, const int& x2, const int& y1, const int& y2)
{
	extern Commander cmder;
    double k=(double)(y2-y1)/(x2-x1);
    for (int i = x1; i <= x2; ++i){
        int j=(int)(k*(i-x1)+y1);
		Response* tmpObj = nullptr;
		tmpObj=cmder.FocusObjID(i, j-1);
        if (tmpObj!=nullptr)    tmpObj->_Draw();
        tmpObj = cmder.FocusObjID(i, j);
        if (tmpObj != nullptr)    tmpObj->_Draw();
        tmpObj = cmder.FocusObjID(i, j+1);
        if (tmpObj != nullptr)  tmpObj->_Draw();
    }
    return;
}

bool Point::write(ostream &os) const
{
    os << static_cast<int>(PointType::POINT) << "\t" << X << "\t" << Y << endl;
    return true;
}

bool Point::read(istream &is)
{
    is >> X >> Y;
    return true;
}
bool Line::write(ostream &os) const
{
    os << static_cast<int>(PointType::LINE) << "\t" << points.size() << "\t";
    for (auto &pt : points)
        os << pt.getX() << "\t" << pt.getY() << "\t";
    os << endl;
    return true;
}
bool Line::read(istream &is)
{
    size_t ncount;
    is >> ncount;
    for (int i = 0; i < ncount; ++i)
    {
        int x,y;
        is >> x >> y;
        if (i)    borders.push_back(Borden(points.back().getX(),points.back().getY(),x,y));
        points.push_back(Point(x,y,fColor, PointType::LINE));
        points.back().SetFather(id);
    }
    return true;
}
bool Polygen::write(ostream &os) const
{
    os << static_cast<int>(PointType::POLYGEN) << "\t" << points.size() << "\t";
    for (auto &pt : points)
        os << pt.getX() << "\t" << pt.getY() << "\t";
    os << endl;
    return true;
}
bool Polygen::read(istream &is)
{
    size_t ncount;
    is >> ncount;
    for (int i = 0; i < ncount; ++i)
    {
        int x, y;
        is >> x >> y;
        if (i)    borders.push_back(Borden(points.back().getX(), points.back().getY(), x, y));
        points.push_back(Point(x, y, fColor, PointType::POLYGEN));
        points.back().SetFather(id);
    }
    return true;
}
Response *Response::FindClone(istream &is)
{
    int type;
    Response *pshp = nullptr;
    is >> type;
    switch (type)
    {
	case static_cast<int>(PointType::POINT):
        pshp = new Point();
        break;
	case static_cast<int>(PointType::LINE):
        pshp = new Line();
        break;
	case static_cast<int>(PointType::POLYGEN):
        pshp = new Polygen();
        break;
    default:
        pshp = nullptr;
        break;
    }
    if (pshp)
        is >> (*pshp);
    return pshp;
}