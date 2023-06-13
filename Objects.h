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
namespace ColorConst {
	constexpr char _dx = 3;
	constexpr char _dy = 3;
	constexpr char _BOLD_ = 2;
	constexpr char _SIZE_ = 10;
	constexpr double ALPHA = 1.0;
}
using namespace ColorConst;
constexpr bool DRAWING = false;
constexpr bool EXISTED = true;
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
    bool drawed;
	unsigned int id;
	static unsigned int count;
public:
	Response() :X(0), Y(0), color(WHITE), alpha(1) {
		id = ++count;
		drawed = false;
	}
	Response(int x, int y, COLORREF Color, double Alpha) :X(x), Y(y), color(Color), alpha(Alpha) {
		id=++count;
        drawed=false;
	}
	~Response() {
		--count;
	}
	virtual int _Draw() =0;
	virtual int _Delete()=0;
	virtual int ClickLeft(bool,const MOUSEMSG&) = 0;
    virtual int ClickRight(bool, const MOUSEMSG &) = 0;
    virtual int Suspend()=0;
    void Move(const int& , const int& );
	int getX() const;
	int getY() const;
	int getID() const;
	unsigned int getCount() const { return count; };
	double getColor() const;
	double getAlpha() const;
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
	int getX() const;
	int getY() const;
	int getID() const;
	virtual int _Draw() =0;
	virtual int _Delete()=0;
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
    int getX() const;
    int getY() const;
	COLORREF  getColor() const;
	int getSize() const;
	void Print(COLORREF);
};
class Point :public Response
{
private:
	int size;
	Text info;
    PointType type;
protected:
	virtual int _Delete();
	virtual void DisplayInfo() const;
    unsigned int father;//派生类可以修改father！这符合father的含义！
public:
    Point() : Response(), type(PointType::POINT), size(_SIZE_) { father = id; }
    Point(int X, int Y, COLORREF Color, double Alpha, PointType Type,int Size=_SIZE_) :Response(X, Y, Color, Alpha), type(Type),size(Size) {father=id;}
    ~Point() { _Delete(); }
    int getSize() const;
	PointType getType() const;
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	virtual int _Draw();
};
class Borden :public Display
{
private:
	int termX, termY, bold;
protected:
	virtual int _Draw();
    virtual int _Delete();
public:
	Borden(int sX,int sY,int tX,int tY,int Bold=_BOLD_):Display(sX,sY,BLACK),termX(tX),termY(tY),bold(Bold){}
    ~Borden() { _Delete(); }
	int getTermX() const;
	int getTermY() const;
	int getBold() const;
	inline double CalcX(const int&);
	virtual void DisplayInfo() const;
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
private:
	vector<Point> points;
	vector<Borden> borders;
	double area;
	bool shownedInfo;
protected:
	double CalcArea();
    int _AddPoint(const MOUSEMSG &);
    int _DeletePoint(const unsigned int &);
    int _Erase(vector<Borden>::iterator);
    int _Bind(vector<Point>::iterator, vector<Point>::iterator);
    virtual int _Draw();
    virtual int _Delete();
    virtual void DisplayInfo() const;
public:
    Polygen() : Response(), points{}, borders{}, area(0), shownedInfo(false) {}
	Polygen(int X, int Y, COLORREF Color) :Response(X, Y, Color, ALPHA), points{}, borders{}, area(0), shownedInfo(false) {}
	~Polygen() {
		points.clear();
		borders.clear();
        _Delete();
    }
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	static int CalcLine(const int&, const int&, Polygen*);
};
class Line :public Response
{
friend class Point;
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
    int _Erase(vector<Borden>::iterator);
    int _Bind(vector<Point>::iterator, vector<Point>::iterator);
	virtual void DisplayInfo() const;
public:
    Line() : Response(), points{}, borders{}, length(0), shownedInfo(false) {}
	Line(int X, int Y, COLORREF Color) :Response(X, Y, Color, ALPHA), points{}, borders{}, length(0), shownedInfo(false) {}
	~Line() {
		points.clear();
		borders.clear();
        _Delete();
	}
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	static bool CheckEdges(const int&, const int&, Line*);
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
	void SaveToFile(fstream&);
	void LoadFromFile(fstream&);
public:
	Button(int X, int Y, COLORREF Color, int w, int h, ButtonType b) :Response(X, Y, Color, ALPHA), width(w), height(h), btype{ b } {};
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	int Press(Status,const MOUSEMSG&, vector<Point*>::iterator&);
	virtual int _Draw();
    virtual int _Delete();
    virtual void DisplayInfo() const;
	void Addinfo(string);
};

Line* FindLine(int ID);
Polygen* FindPolygen(int ID);
bool CastWarning(const char*);
#endif // !_OBJECTS_H_
