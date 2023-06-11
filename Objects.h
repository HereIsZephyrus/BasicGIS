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
static string filename;


class Commander ;
class Display;
class Response
{
	friend class Display;
protected:
	int X, Y, borderBold, color;
	double alpha;
	static unsigned int count;
    bool drawed;
	unsigned int id;
	virtual int _Draw() = 0;
    virtual int _Delete() = 0;
public:
	Response() :X(0), Y(0), borderBold(_BOLD_), color(0), alpha(1) {
		id = ++count;
	}
	Response(int x, int y, int Bold, int Color, double Alpha) :X(x), Y(y), borderBold(Bold), color(Color), alpha(Alpha) {
		id=++count;
        drawed=false;
	}
	~Response() {
		--count;
	}
	virtual int ClickLeft(bool,const MOUSEMSG&) = 0;
    virtual int ClickRight(bool, const MOUSEMSG &) = 0;
    virtual int Suspend() = 0;
    void Move(const int& , const int& );
	int getX() const;
	int getY() const;
	int getID() const;
	double getColor() const;
	double getAlpha() const;
};
class Display
{
	friend class Response;
protected:
	int X, Y;
	static unsigned int count;
	unsigned int id;
	virtual int _Draw() = 0;
    virtual int _Delete() = 0;
public:
	Display(int x, int y) :X(x), Y(y) {
		id = ++count;
	}
	~Display() {
		--count;
	}
	int getX() const;
	int getY() const;
	int getID() const;
};

class Text {
protected:
	int X, Y, color, size;
	string contain;
public:
	Text(): X(1),Y(1),color(BLACK),size(10){}
	Text(int x, int y, int colour, int font,string Contain) :X(x), Y(y), color(colour), size(font),contain(Contain) {}
    int getX() const;
    int getY() const;
    int getColor() const;
	int getSize() const;
	void Print(const int&,const int&)
};
class Point :public Response
{
public:
    enum PointType{
        POINT,
        LINE,
        POLYGEN,
        MAX_OBJECT
    };
private:
	int size;
	Text info;
    PointType type;
protected:
	virtual int _Delete();
	virtual void DisplayInfo() const;
	virtual int _Draw();
    virtual int _Delete();
    unsigned int father;//派生类可以修改father！这符合father的含义！
public:
	Point(int X, int Y, int Bold, int Color, double Alpha, PointType Type,int Size=_SIZE_) :Response(X, Y, Bold, Color, Alpha), type(Type),size(Size) {father=id;}
    ~Point() { _Delete(); }
    int getSize() const;
	int getType() const;
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
};
class Borden :public Display
{
private:
	int termX, termY, bold;
protected:
	virtual int _Draw();
    virtual int _Delete();
public:
	Borden(int sX,int sY,int tX,int tY,int Bold=_BOLD_):Display(sX,sY),termX(tX),termY(tY),bold(Bold){}
    ~Borden() { _Delete(); }
	int getTermX() const;
	int getTermY() const;
	int getBold() const;
	virtual void DisplayInfo() const;
};

class Squareness :public Display
{
private:
	int width, height;
	vector<Text> msg;
public:
	Squareness(int x, int y, int w, int h) :Display(x, y), width(w), height(h),msg{} {};
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
	Polygen(int X, int Y, int Bold, int Color) :Response(X, Y, Bold, Color, ALPHA), points{}, borders{}, area(0), shownedInfo(false) {}
	~Polygen() {
		points.clear();
		borders.clear();
        _Delete();
    }
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
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
	Line(int X, int Y, int Bold, int Color) :Response(X, Y, Bold, Color, ALPHA), points{}, borders{}, length(0), shownedInfo(false) {}
	~Line() {
		points.clear();
		borders.clear();
        _Delete();
	}
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
};

class Button :public Response
{
friend class Commander;
private:
	int width, height;
	Text info;
	ButtonType type;
protected:
	int LoadPhoto(string);
	void SaveToFile(fstream*);
	void LoadFromFile(fstream*);
public:
	Button(int X, int Y, int Bold, int Color, int w, int h) :Response(X, Y, Bold, Color, ALPHA), width(w), height(h), type{ NOEXIST_BUTTON } {};
    virtual int ClickLeft(bool, const MOUSEMSG &);
    virtual int ClickRight(bool, const MOUSEMSG &);
    virtual int Suspend();
	int Press(Status,const MOUSEMSG&, vector<Response*>::iterator&);
	virtual int _Draw();
    virtual int _Delete();
    virtual void DisplayInfo() const;
};
#endif // !_OBJECTS_H_
