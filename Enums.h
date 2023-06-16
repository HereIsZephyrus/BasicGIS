//枚举类管理，因为枚举类型不能有外部链接，一些图方便没写成class后面也出了问题
#pragma once
#ifndef _ENUMS_H_
#define _ENUMS_H_
enum ButtonType {
	Load,
	Save,
	New,
	Open,
	Exit,
	Switch,
	DrawPoint,
	DrawLine,
	DrawPolygen,
	Finish,
	NOEXIST_BUTTON
};
enum Status
{
	Hold,
	Drawing,
	Drag,
	NOEXIST_STATUS
};
enum Areas
{
	Toolbar,
	Photo,
	OUT_OF_RANGE
};
enum class PointType {
	POINT,
	LINE,
	POLYGEN,
	MAX_OBJECT
};
#endif // !_ENUMS_H_