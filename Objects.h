#pragma once
#ifndef _OBJECTS_H_
#define _OBJECTS_H_
#include "Enums.h"
#include<graphics.h>
#include<conio.h>
#include<cstring>
#include<string>
#include<vector>
using std::string;
using std::wstring;
using std::vector;
using std::fstream;
using std::ofstream;
using std::ifstream;
namespace ColorConst {
	constexpr char _dx = 3;
	constexpr char _dy = 3;
	constexpr char _BOLD_ = 3;
	constexpr char _BOLDER_ = 1;
	constexpr double ALPHA = 1.0;
}
using namespace ColorConst;
constexpr bool DRAWING = false;
constexpr bool EXISTED = true;
constexpr char _SIZE_ = 5;
static LPTSTR filename;


class Commander ;
class Display;
class Response
{
	friend class Display;
protected:
	int X, Y;
	COLORREF color;
	double alpha;
    bool drawed, focused, shownedInfo;
	unsigned int id;
	static unsigned int count;
public:
	Response() :X(0), Y(0), color(WHITE), alpha(1) {
		id = ++count;
		drawed = false;
		focused = false;
		shownedInfo = false;
	}
	Response(int x, int y, COLORREF Color, double Alpha) :X(x), Y(y), color(Color), alpha(Alpha) {
		id=++count;
        drawed=false;
		focused = false;
	}
	~Response() {
		--count;
	}
	virtual int _Draw() { return 0; };
	virtual int _Delete() { return 0; };
	virtual int ClickLeft(bool,const MOUSEMSG&) { return 0; };
    virtual int ClickRight(bool, const MOUSEMSG &) { return 0; };
    virtual int Suspend() { return 0; };
    virtual int UnSuspend() { return 0; };
	virtual int Move(const int&, const int&) { return 0; };
	int getX() const { return X; }
	int getY() const { return Y; }
	int getID() const { return id; }
	bool getDraw() const { return drawed; }
	bool getFocus() const { return focused; }
	double getColor() const { return color; }
	double getAlpha() const { return alpha; }
	unsigned int getCount() const { return count; };
};
class Display
{
	friend class Response;
protected:
	int X, Y;
	unsigned int id;
	bool drawed;
	COLORREF color;
	static unsigned int count;
public:
	Display(int x, int y, COLORREF Color=WHITE) :X(x), Y(y),color(Color) {
		id = ++count;
		drawed = false;
	}
	~Display() {
		--count;
	}
	int getX() const { return X; }
	int getY() const { return Y; }
	int getID() const { return id; }
	virtual int _Draw() =0;
	virtual int _Delete()=0;
	virtual int Move(const int&, const int&) { return 0; };
	unsigned int getCount() const { return count; };
};

class Text {
protected:
	int X, Y,  size;
	COLORREF color;
	string contain;
public:
	Text(): X(1),Y(1),color(BLACK),size(10){}
	Text(int x, int y, string Contain,int font=10, COLORREF colour=BLACK) :X(x), Y(y),contain(Contain), size(font),color(colour) {}
	int getX() const { return X; }
	int getY() const { return Y; }
	COLORREF  getColor() const { return color; }
	int getSize() const { return size; }
	void Print(COLORREF);
};
class Point :public Response
{
private:
	int size;
	Text info;
    PointType type;
    unsigned int father;
public:
    Point() : Response(), type(PointType::POINT), size(_SIZE_) { father = id; }
    Point(int X, int Y, COLORREF Color, double Alpha, PointType Type,int Size=_SIZE_) :Response(X, Y, Color, Alpha), type(Type),size(Size) {
		father=id;
		drawed = false;
		focused = false;
	}
    //~Point() { _Delete(); }
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
    virtual int UnSuspend();
	virtual int _Draw();
	virtual int _Delete();
	virtual int Move(const int&, const int&);
	void SetFather(unsigned int Fa) { father = Fa; }
    PointType getType() const {return PointType::MAX_OBJECT;}
    int getSize() const { return size; }
	virtual void DisplayInfo() const;
};
class Borden :public Display
{
private:
	int termX, termY, bold;
public:
	Borden(int sX,int sY,int tX,int tY,int Bold=_BOLD_):Display(sX,sY,BLACK),termX(tX),termY(tY),bold(Bold){}
    //~Borden() { _Delete(); }
    int getTermX() const { return termX; }
    int getTermY() const { return termY; }
    int getBold() const { return bold; }
    inline double CalcX(const int&);
	virtual void DisplayInfo() const;
	virtual int _Draw();
	virtual int _Delete();
	virtual int Move(const int&, const int&);
};

class Squareness :public Display
{
private:
	int width, height;
	vector<Text> msg;
public:
	Squareness(int x, int y, int w, int h, COLORREF color=WHITE) :Display(x, y,color), width(w), height(h),msg{} {};
	~Squareness() {
		msg.clear();
        _Delete();
    }
	virtual int _Draw();
    virtual int _Delete();
	inline void AddText(Text);
};

class Polygen :public Response
{
friend class Point;
friend class Commander;
private:
	vector<Point> points;
	vector<Borden> borders;
	double area;
protected:
	double CalcArea();
    int _AddPoint(const MOUSEMSG &);
    int _DeletePoint(const unsigned int &);
    int _Erase(Borden*);
    int _Bind(vector<Point>::iterator, vector<Point>::iterator);
    virtual int _Draw();
    virtual int _Delete();
    virtual void DisplayInfo() const;
	virtual int Move(const int&, const int&);
public:
    Polygen() : Response(), points{}, borders{}, area(0) {}
	Polygen(int X, int Y, COLORREF Color) :Response(X, Y, Color, ALPHA), points{}, borders{}, area(0) {
		drawed = false;
		focused = false;
	}
	~Polygen() {
    for (auto i = points.begin(); i != points.end(); ++i)
        i->~Point();
    for (auto i = borders.begin(); i != borders.end(); ++i)
        i->~Borden();
		points.clear();
		borders.clear();
        _Delete();
    }
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
    virtual int UnSuspend();
	static int CalcLine(const int&, const int&, Polygen*);
};
class Line :public Response
{
friend class Point;
friend class Commander;
private:
	vector<Point> points;
	vector<Borden> borders;
	double length;
	bool shownedInfo;
protected:
	double CalcLength();
	int _AddPoint(const MOUSEMSG&);
	int _DeletePoint(const unsigned int&);
    virtual int _Draw();
    virtual int _Delete();
    int _Erase(Borden*);
    int _Bind(vector<Point>::iterator, vector<Point>::iterator);
	virtual void DisplayInfo() const;
public:
    Line() : Response(), points{}, borders{}, length(0), shownedInfo(false) {}
	Line(int X, int Y, COLORREF Color) :Response(X, Y, Color, ALPHA), points{}, borders{}, length(0), shownedInfo(false) {
		drawed = false;
		focused = false;
	}
	~Line() {
    for (auto i = points.begin(); i != points.end(); ++i)
        i->~Point();
    for (auto i = borders.begin(); i != borders.end(); ++i)
        i->~Borden();
		points.clear();
		borders.clear();
        _Delete();
	}
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
    virtual int UnSuspend();
	static bool CheckEdges(const int&, const int&, Line*);
	virtual int Move(const int&, const int&);
};

class Button :public Response
{
friend class Commander;
private:
	int width, height;
	Text info;
	ButtonType btype;
protected:
	int LoadPhoto(wstring&);
	void SaveToFile(ofstream&);
	void LoadFromFile(ifstream&);
public:
	Button(int X, int Y, COLORREF Color, int w, int h, ButtonType b) :Response(X, Y, Color, ALPHA), width(w), height(h), btype{ b } {
		drawed = false;
		focused = false;
	};
	const int getWidth() {		return width; }
	const int getHeight() {	return height;}
	ButtonType getType() { return btype; }
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	virtual int UnSuspend();
	int Press(Status&,const MOUSEMSG&, Response*&,const bool);
	virtual int _Draw();
    virtual int _Delete();
    virtual void DisplayInfo() const;
	void Setinfo(string);
};

Line* FindLine(int ID);
Polygen* FindPolygen(int ID);
LPTSTR CharToLPTSTR(const char*);
char* LPTSTRToChar(LPTSTR str);
void ReDraw(const int &, const int &, const int &, const int &);
void Flush();
#endif // !_OBJECTS_H_
