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
#include <fstream>
#include <iostream>

vector<Point*> objList;
vector<Squareness*> elmList;
vector<Button*> butList;
unsigned int Response::count = 0;
unsigned int Display::count = 0;
void Response::Move(const int& dx, const int& dy)
{
    X = X + dx;
    Y = Y + dy;
	//对对象进行移动，并用EdgeError类处理非法移动
	return;
}
void Text::Print(COLORREF backColor)
{
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

int Point::getSize() const              {return size;}

int Borden::getTermX() const             {return termX;}
int Borden::getTermY() const             {return termY;}
int Borden::getBold() const             {return bold;}
void Borden::DisplayInfo() const
{
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
	//@@@@@@@@@@@@@@@HereIsZephyrus绘制边框
	return 0;
}
int Squareness::_Delete()
{
	drawed = false;
	return 0;
	//@@@@@@@@@@@@@@@HereIsZephyrus擦掉图像
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
		Fa->_DeletePoint(id);
		break;
		}
	case PointType::POLYGEN:
		{
		Polygen* Fa=FindPolygen(father);
		Fa->_DeletePoint(id);
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
PointType Point::getType() const
{
	return PointType::MAX_OBJECT;
}
int Point::Suspend()
{
	focused = true;
	size=size*2;
    _Draw();
	return 0;
}
int Point::UnSuspend()
{
	focused = false;
    size = size /2;
    _Draw();
    return 0;
}
int Point::_Draw()
{
	if (drawed)//failed
		return 1;
	drawed = true;
	color = 1;  size = _SIZE_;
	fillcircle(X,Y,size);
	//@@@@@@@@@@@HereIsZephyrus画图
	return 0;
}
int Point::_Delete()
{
	if (!drawed)//failed
		return 1;
	drawed = false;
	color = 0;  size = _SIZE_;
	//@@@@@@@@@@@@@HereIsZephyrus擦掉图像
	return 0;
}
void Point::DisplayInfo() const
{
	const int n = 2;
	Squareness Msg(X+_dx,Y+_dy,_WMSG,_SIZE_*n);
	{//n
		Msg.AddText(Text());
		Msg.AddText(Text());
	}
	if (!drawed) Msg._Draw();
	else            Msg._Delete();
	return;
}

int Borden::_Draw()
{
	if (drawed)//failed
		return 1;
	drawed = true;
	//@@@@@@@@@@@@@HereIsZephyrus画图
	return 0;
}
int Borden::_Delete()
{
	if (!drawed)//failed
		return 1;
	drawed = false;
	//@@@@@@@@@@@@@HereIsZephyrus擦掉图像
	return 0;
}
double Borden::CalcX(const int &y)
{
	double k=(termY-Y)/(termX-X);
	return k*(y-Y)+X;
}
void Button::DisplayInfo() const{
	//右键显示个guide信息
	return;
}

int Polygen::ClickLeft(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED)
	{
		DisplayInfo();
		return 0;
	}
	_AddPoint(mouse);
	return 0;
}
int Polygen::ClickRight(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED && CastWarning("您确定要删除该多边形吗?") == true) //显示文本框确认（利用短路特性）
		return 0;
	_Delete();
	//在绘制过程中是不可以一下子删除一整个面对象的，所以这里不用管，在Drawing时点到点上会自己删除，边是Display对象不用管
	return 0;
}
int Polygen::Suspend()
{
	focused = true;
	//填充一个多边形出来，透明度低一些
	const size_t num = points.size();
    if (num < 3) return 1;//failed
    vector<POINT> pts;
	for (vector<Point>::iterator it = points.begin(); it != points.end(); ++it)
    {
		POINT pt;	
		pt.x = (*it).getX();	pt.y = (*it).getY();
        pts.push_back(pt);
    }
    setbkcolor(fColor); // 设置背景色为红色
    //setalpha(128);                  // 设置透明度为 50%
    fillpolygon(&pts[0], static_cast<int>(num));
    return 0;
}
int Polygen::UnSuspend()
{
	focused = false;
    //擦掉填充的多边形
    cleardevice();
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
	vector<Point>::iterator p = points.begin();
	const int x1 = mouse.x, y1 = mouse.y, x2 = (*p).getX(), y2 = (*p).getY();
	if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))<_SIZE_)
		_Bind(points.begin(), points.end() - 1);//手动封闭了
	else
	{
		points.push_back(Point(x1,x2,color,alpha, PointType::POLYGEN));
		_Bind(points.end() - 1, points.end() - 2);
	}
	return 0;
}
int Polygen::_DeletePoint(const unsigned int& id)
{
	//从队列中删除一个点，然后重新绑定边
	vector<Point>::iterator p = points.begin();
	Borden* b;
	if ((*p).getID() == id)
	{
		b=&*borders.begin();
		_Erase(b);
		b=&*(borders.end()-1);
		_Erase(b);
		points.erase(p);
		_Bind(points.begin(), points.end() - 1);
		return 0;
	}
	p = points.end()-1;
	if ((*p).getID() == id)
	{
		b = &*(borders.end() - 1);
		_Erase(b);
		b = &*(borders.end()-2);//一个矩形一定起码有三条边的！不会越界
		_Erase(b);
		points.erase(p);
		_Bind(points.begin(), points.end() - 1);
		return 0;
	}
	for (p = points.begin()+1,b=&*(borders.begin()+1); p != points.end(); ++p,++b)
	{
		if (b==&*borders.end())//failed
			return 1;
		if ((*p).getID() == id)
		{
			_Erase(b);_Erase(b-1);
			_Bind(p-1, p+1);//这里不会越界，因为上面的if已经判断过了（挺妙的）
			points.erase(p);
			break;
		}
	}
	return 0;
}
int Polygen::_Bind(vector<Point>::iterator p1, vector<Point>::iterator p2)
{
	const int x1 = ( * p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
	borders.push_back(Borden(x1, y1, x2, y2));
	return 0;
}
int Polygen::_Erase(Borden* b)
{
	//这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
	//！！！记得擦掉图像
	for (vector<Borden>::iterator it = borders.begin(); it != borders.end(); ++it)
		if (&*it == b)
		{
			borders.erase(it);
			return 0;
		}
	return 0;
}
int Polygen::_Draw()
{
	drawed = true;
	//@@@@@@@@@@HereIsZephyrus画出图像
	//如果最后一个点不是收尾相连，那么连上
	return 0;
}
int Polygen::_Delete()
{
	drawed = false;
	return 0;
}
int Polygen::CalcLine(const int& x, const int& y, Polygen* obj)
{
	int calc = 0;
	Polygen* poly = dynamic_cast<Polygen*>(obj);
	for (vector<Borden>::iterator it = (poly->borders).begin(); it != (poly->borders).end(); ++it)
		if ((*it).CalcX(y) <= x)   ++calc;
	return calc;
}
void Polygen::DisplayInfo() const
{
	const int n = 2;
	Squareness Msg(X + _dx, Y + _dy, _WMSG, _SIZE_ * n);
	{//n
		Msg.AddText(Text());
		Msg.AddText(Text());
	}
	Msg._Draw();
	return;
}

int Line::ClickLeft(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED)
	{
		DisplayInfo();
		return 0;
	}
	return 0;
}
int Line::ClickRight(bool Status, const MOUSEMSG &mouse)
{
	if (Status == EXISTED && CastWarning("您确定要删除该线吗?") == true) //显示文本框确认（利用短路特性）
		return 0;
	_Delete();
	// 在绘制过程中是不可以一下子删除一整个线对象的，所以这里不用管，在Drawing时点到点和边上会自己删除
	return 0;
}
int Line::Suspend()
{
	focused = true;
	//把线变个色
	return 0;
}
int Line::UnSuspend()
{
	focused = false;
    //把线变回来
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
		if (getX >= x - _dx && getX <= x + _dx)
			return true;
	}
	return false;
}
void Line::DisplayInfo() const
{
	const int n = 2;
	Squareness Msg(X + _dx, Y + _dy, _WMSG, _SIZE_ * n);
	{//n
		Msg.AddText(Text());
		Msg.AddText(Text());
	}
	Msg._Draw();
	return;
}
int Line::_AddPoint(const MOUSEMSG& mouse)
{
	vector<Point>::iterator p = points.begin();
	const int x1 = mouse.x, y1 = mouse.y, x2 = (*p).getX(), y2 = (*p).getY();
	if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < _SIZE_)
		_Bind(points.begin(), points.end() - 1);
	else
	{
		points.push_back(Point(x1, x2, color, alpha, PointType::POLYGEN));
		_Bind(points.end() - 1, points.end() - 2);
	}
	return 0;
}
int Line::_DeletePoint(const unsigned int& id )
{
	// 从队列中删除一个点，然后重新绑定边
	vector<Point>::iterator p = points.begin();
	Borden* b= &*borders.begin();
	if ((*p).getID() == id)
	{
		_Erase(b);
		points.erase(p);
		return 0;
	}
	p = points.end() - 1;
	if ((*p).getID() == id)
	{
		b = &*(borders.end() - 1);
		_Erase(b);
		points.erase(p);
		return 0;
	}
	for (p = (points.begin() + 1), b = &*(borders.begin() + 1); p != (points.end()); ++p, ++b)
	{
		if (b == &*borders.end()) // failed
			return 1;
		if ((*p).getID() == id)
		{
			_Erase(b);
			_Bind(p - 1, p + 1); // 这里不会越界，因为上面的if已经判断过了（挺妙的）
			points.erase(p);
			break;
		}
	}
	return 0;
}
int Line::_Draw()
{
	drawed = true;
	//@@@@@@@@@@HereIsZephyrus画出图像
	return 0;
}
int Line::_Bind(vector<Point>::iterator p1, vector<Point>::iterator p2)
{
	const int x1 = (*p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
	borders.push_back(Borden(x1, y1, x2, y2));
	return 0;
}
int Line::_Erase(Borden* b)
{
	//这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
	//！！！记得擦掉图像
	for (vector<Borden>::iterator it=borders.begin(); it!=borders.end(); ++it)
		if (&*it == b)
		{
			borders.erase(it);
			return 0;
		}
	return 0;
}
int Line::_Delete()
{
	drawed = false;
	//@@@@@@@@@@HereIsZephyrus擦掉图像
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
    color=bColor;//白色
    _Draw();
    return 0;
}
int Button::Press(Status stage, const MOUSEMSG& mouse, Point* obj,bool Force) {
	if (mouse.uMsg != WM_LBUTTONUP && Force==false) // 除了左键放开，在工具栏都是点着玩的
		return 0;
	if (btype == Exit)
		return 1;
	using std::fstream;
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
		while (LoadPhoto(photoName)) {
			//fail to load
			memset(buffer, 0, sizeof(buffer));
			CastError("请输入正确的文件路径和文件名！");
			InputBox(buffer, 100, L"请输入图片路径与名称");
			photoName = buffer;
		}
		break;
	}
	case New:
	{
		InputBox(filename, 50, L"请输入文件名称（默认保存在程序所在文件夹内");
		fstream fp;
		fp.open(filename, ios_base::out);
		SaveToFile(fp);
		fp.close();
		break;
	}
	case Open:
	{
		InputBox(filename, 50, L"请输入文件绝对路径与完整名称");
		fstream fp;
		fp.open(filename, std::ios_base::in);
		LoadFromFile(fp);
		fp.close();
		break;
	}
	case Save:
	{
		fstream fp;
		fp.open(filename, ios_base::out);
		SaveToFile(fp);
		fp.close();
		break;
	}
	case Switch:
	{
		if (stage == Drag)    stage = Hold;
		if (stage == Hold)    stage = Drag;
		break;
	}
	case DrawPoint:
	{
		stage = Drawing;
		const int x = mouse.x;
		objList.push_back(new Point());
		obj = *(objList.end() - 1);
		break;
	}
	case DrawLine:
	{
		stage = Drawing;
		const int x = mouse.x;
		objList.push_back(dynamic_cast<Point*>(new Line()));
		obj = *(objList.end() - 1);
		break;
	}
	case DrawPolygen:
	{
		stage = Drawing;
		const int x = mouse.x;
		objList.push_back(dynamic_cast<Point*>(new Polygen()));
		obj = *(objList.end() - 1);
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
	return 0;
}
int Button::LoadPhoto(std::wstring& name)
{
	IMAGE	img;
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
void Button::SaveToFile(fstream& fp)
{
	if (!fp.is_open())
	{
		MessageBox(NULL, L"Open File Failed.", L"ERROR...", MB_ICONERROR);
		return;
	}
   // fp << butList << elmList << objList;
	return;
}
void Button::LoadFromFile(fstream& fp)
{
	if (!fp.is_open())
	{
		MessageBox(NULL, L"Open File Failed.", L"ERROR...", MB_ICONERROR);
		return;
	}
	objList.clear();
	elmList.clear();
	butList.clear();
	//fp >> objList>>elmList>> butList;
	for (vector<Point*>::iterator it = objList.begin(); it != objList.end(); ++it)
		(*it)->_Draw();
	for (vector<Squareness*>::iterator it = elmList.begin(); it != elmList.end(); ++it)
		(*it)->_Draw();
	for (vector<Button*>::iterator it = butList.begin(); it != butList.end(); ++it)
		(*it)->_Draw();
	return;
}
void Button::Addinfo(std::string msg)
{
	info = Text(X + (width - _FONT * 4) / 2, Y + (height - _FONT) / 2, msg, _FONT, BLACK);
	return;
}

Line* FindLine(int ID)
{
	for (vector<Point*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		if (typeid(*it) != typeid(Line))  continue;
		if ((*it)->getID() == ID)
			return dynamic_cast<Line*>(* it);
	}
	//failed
	return nullptr;
}
Polygen* FindPolygen(int ID)
{
	for (vector<Point*>::iterator it = objList.begin(); it != objList.end(); ++it)
	{
		if (typeid(*it) != typeid(Polygen))  continue;
		if ((*it)->getID() == ID)
			return dynamic_cast<Polygen*>(*it);
	}
	//failed
	return nullptr;
}