#ifndef _Diaballik_BoardArea_h_
#define _Diaballik_BoardArea_h_

#include <CtrlCore/CtrlCore.h>

class BoardArea : public Picture
{
public:
	Callback2<Point, dword> WhenClicked;
	void LeftUp(Point p, dword keyflags) { WhenClicked.Execute(p, keyflags); }
	void operator=(const Drawing& d) { Picture::operator=(d); }
};

#endif
