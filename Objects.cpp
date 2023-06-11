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
#include <fstream>
#include <iostream>

int Response::getX() const              {return X;}
int Response::getY() const              {return Y;}
int Response::getID() const             {return id;}
double Response::getColor() const     {return color;}
double Response::getAlpha() const { return alpha; }
void Response::Move(const int& dx, const int& dy)
{
    //对对象进行移动，并用EdgeError类处理非法移动
    return;
}

int Display::getX() const               {return X;}
int Display::getY() const               {return Y;}
int Display::getID() const              {return id;}

int Text::getX() const                  {return X;}
int Text::getY() const                  {return Y;}
int Text::getColor() const              {return color;}
int getSize() const                     {return size}

int Point::getSize() const              {return size;}

int Borden::getTermX() const             {return termX;}
int Borden::getTermY() const             {return termY;}
int Borden::getBold() const             {return bold;}

void Print(const int& x, const int& y)
{
    return;
}
void Squareness::AddText(Text texts)
{
    msg.push_back(texts);
    return;
}
void Squareness::_Draw()
{
    int x = X,y=Y;
    for (vector<Text>::iterator it = msg.begin(); it != msg.end(); ++it)
    {
        (*it)->Print(x,y);
        y = y + (*it)->getSize()+_dy;
    }
    //@@@@@@@@@@@@@@@HereIsZephyrus绘制边框
    return;
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
    if (Status == EXISTED) //显示文本框确认
    {

        HWND hnd = GetHWnd();
        SetWindowText(hnd, "警告！");
        int isDelete = MessageBox("您确定要删除该点吗");
        if (isDelete == IDCANCEL)
            return 0;
    }
    switch (type)
    {
        case POINT:
        {
        _Delete();
        break;
        }
        case LINE:
        {
        Line* Fa=FindLine(father);
        *Fa._DeletePoint(id);
        break;
        }
        case POLYGEN:
        {
        Polygen* Fa=FindPolygen(father);
        *Fa._DeletePoint(id);
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
    color = 1; alpha = 0.8;
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
    using namespace WINDOW_CONST;
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
    if (Status == EXISTED) // 显示文本框确认
    {

        HWND hnd = GetHWnd();
        SetWindowText(hnd, "警告！");
        int isDelete = MessageBox("您确定要删除该面吗");
        if (isDelete == IDCANCEL)
            return 0;
        _Delete();
    }
    //在绘制过程中是不可以一下子删除一整个面对象的，所以这里不用管，在Drawing时点到点上会自己删除，边是Display对象不用管
    return 0;
}
int Polygen::Suspend()
{
    //填充一个多边形出来，透明度低一些
    const unsigned int num = points.size();
    POINT pts[num] = {};
    for (int i=0,vector<Point>::iterator it = points.begin(); it != points.end(); ++i,++it)
        pts[i] = { (*it).getX(),(*it).getY() };
    fillpolygon(pts, num);//！！！再把透明度做一下
    return 0;
}
double Polygen::CalcArea()
{
    //有时间再开发，不是特别难写，想用树状数组，但是不好调试太久没写了
    return 0;
}
int Polygen::_AddPoint(const MOUSEMSG& mouse)
{
    vector<Point>::iterator p = points.begin();
    const int x1 = mouse.x, y1 = mouse.y, x2 = (*p).getX(), y2 = (*p).getY();
    if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))<_SIZE_)
        _Bind(points.begin(), points.end() - 1);
    else
    {
        points.push_back(Point(x1,x2,_BOLD_,color,alpha));
        _Bind(points.end() - 1, points.end() - 2);
    }
    return 0;
}
int Polygen::_DeletePoint(const unsigned int& id)
{
    //从队列中删除一个点，然后重新绑定边
    vector<Point>::iterator p = points.begin();
    vector<Borden>::iterator b;
    if ((*p).getID() == id)
    {
        b=borders.begin();
        _Erase(b);
        b=borders.end()-1;
        _Erase(b);
        points.erase(p);
        _Bind(points.begin(), points.end() - 1);
        return 0;
    }
    p = points.end()-1;
    if ((*p).getID() == id)
    {
        b = borders.end() - 1;
        _Erase(b);
        b = borders.end()-2;//一个矩形一定起码有三条边的！不会越界
        _Erase(b);
        points.erase(p);
        _Bind(points.begin(), points.end() - 1);
        return 0;
    }
    for (p = points.begin()+1,b=borders.begin()+1; p != points.end(); ++p,++b)
    {
        if (b==borders.end())//failed
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
    const int x1 = (*p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
    borders.push_back(Borden(x1, y1, x2, y2));
    return 0;
}
int Polygen::_Erase(vector<Borden>::iterator b)
{
    //这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
    //！！！记得擦掉图像
    borders.erase(b);
    return 0;
}
int Polygen::_Draw()
{
    //@@@@@@@@@@HereIsZephyrus画出图像
    return 0;
}
void Polygen::DisplayInfo() const
{
    using namespace WINDOW_CONST;
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
    if (Status == EXISTED) // 显示文本框确认
    {

        HWND hnd = GetHWnd();
        SetWindowText(hnd, "警告！");
        int isDelete = MessageBox("您确定要删除该线吗");
        if (isDelete == IDCANCEL)
            return 0;
        _Delete();
    }
    // 在绘制过程中是不可以一下子删除一整个线对象的，所以这里不用管，在Drawing时点到点和边上会自己删除
    return 0;
}
int Line::Suspend()
{
    //把线变个色
    return 0;
}
void Line::DisplayInfo() const
{
    using namespace WINDOW_CONST;
    const int n = 2;
    Squareness Msg(X + _dx, Y + _dy, _WMSG, _SIZE_ * n);
    {//n
        Msg.AddText(Text());
        Msg.AddText(Text());
    }
    Msg._Draw();
    return;
}
void Line::_AddPoint(const MOUSEMSG& mouse)
{
    vector<Point>::iterator p = points.begin();
    const int x1 = mouse.x, y1 = mouse.y, x2 = (*p).getX(), y2 = (*p).getY();
    if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < _SIZE_)
        _Bind(points.begin(), points.end() - 1);
    else
    {
        points.push_back(Point(x1, x2, _BOLD_, color, alpha));
        _Bind(points.end() - 1, points.end() - 2);
    }
    return;
}
int Line::_DeletePoint(const unsigned int& id )
{
    // 从队列中删除一个点，然后重新绑定边
    vector<Point>::iterator p = points.begin();
    vector<Borden>::iterator b;
    if ((*p).getID() == id)
    {
        b = borders.begin();
        _Erase(b);
        points.erase(p);
        return 0;
    }
    p = points.end() - 1;
    if ((*p).getID() == id)
    {
        b = borders.end() - 1;
        _Erase(b);
        points.erase(p);
        return 0;
    }
    for (p = points.begin() + 1, b = borders.begin() + 1; p != points.end(); ++p, ++b)
    {
        if (b == borders.end()) // failed
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
    //@@@@@@@@@@HereIsZephyrus画出图像
    return 0;
}
int Line::_Bind(vector<Point>::iterator p1, vector<Point>::iterator p2)
{
    const int x1 = (*p1).getX(), y1 = (*p1).getY(), x2 = (*p2).getX(), y2 = (*p2).getY();
    borders.push_back(Borden(x1, y1, x2, y2));
    return 0;
}
int Line::_Erase(vector<Borden>::iterator b)
{
    //这里有个bug，就是删除的时候，如果是最后一个，那么就会越界，但是这个函数不会被调用到，所以不用管
    //！！！记得擦掉图像
    borders.erase(b);
    return 0;
}
void Line::_Delete()
{
    //@@@@@@@@@@HereIsZephyrus擦掉图像
    return;
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
    //变个色
    return 0;
}
int Button::Press(Status stage, const MOUSEMSG& mouse, vector<Response*>::iterator& obj) {
    if (type == Exit)
        return 0;
    using std::fstream;
    using std::ios_base;
    switch (type)
    {
    case Load:
    {
        string photoName;
        InputBox(photoName, 50, L"请输入图片绝对路径与完整名称");
        if (LoadPhoto(photoName))
            //fail to load
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
        //objList.push_back(new Point());
        obj = objList.end() - 1;
        break;
    }
    case DrawLine:
    {
        stage = Drawing;
        const int x = mouse.x;
        //objList.push_back(new Line());
        obj = objList.end() - 1;
        break;
    }
    case DrawPolygen:
    {
        stage = Drawing;
        const int x = mouse.x;
        //objList.push_back(new Polygen());
        obj = objList.end() - 1;
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
    //fillroundrect();
    return 0;
}
int Button::LoadPhoto(string name)
{
    using namespace WINDOW_CONST;
    IMAGE	img;
    loadimage(&img,name);
    putimage(_WTool, 0, &img);
    return 0;
}
void Button::SaveToFile(fstream* fp)
{
    using Window_Object;
    if (!fp.is_open())
    {
        MessageBox(NULL, "Open File Failed.", "ERROR...", MB_ICONERROR);
        return;
    }
    return;
}
void Button::LoadFromFile(fstream* fp)
{
    using namespace Window_Object;
    using namespace streaming;
    if (!fp.is_open())
    {
        MessageBox(NULL, "Open File Failed.", "ERROR...", MB_ICONERROR);
        return;
    }
    objList.clear();
    elmList.clear();
    butList.clear();
    fp << objList<<elmList<< butList;
    for (auto& pshp : objList)
    {
        if (pshp)	(*pshp)->_Draw();
    }
    for (auto& pshp : elmList)
    {
        if (pshp)	(*pshp)->_Draw();
    }
    for (auto& pshp : butList)
    {
        if (pshp)	(*pshp)->_Draw();
    }
    return;
}