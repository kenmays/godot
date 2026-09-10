#include "haiku_gl_view.h"

HaikuGLView::HaikuGLView(BRect frame, const char *name, uint32 resizingMode, uint32 options)
	: BGLView(frame, name, resizingMode, options)
{
}

void HaikuGLView::FrameResized(float width, float height)
{
	BGLView::FrameResized(width, height);
}

void HaikuGLView::AttachedToWindow()
{
	BGLView::AttachedToWindow();
	LockGL();
	glViewport(0, 0, Bounds().IntegerWidth() + 1, Bounds().IntegerHeight() + 1);
	UnlockGL();
}

void HaikuGLView::DetachedFromWindow()
{
	BGLView::DetachedFromWindow();
}

void HaikuGLView::KeyDown(const char *bytes, int32 numBytes)
{
	BGLView::KeyDown(bytes, numBytes);
}

void HaikuGLView::KeyUp(const char *bytes, int32 numBytes)
{
	BGLView::KeyUp(bytes, numBytes);
}

void HaikuGLView::MouseDown(BPoint where)
{
	BGLView::MouseDown(where);
}

void HaikuGLView::MouseUp(BPoint where)
{
	BGLView::MouseUp(where);
}

void HaikuGLView::MouseMoved(BPoint where, uint32 transit, const BMessage *dragMessage)
{
	BGLView::MouseMoved(where, transit, dragMessage);
}

void HaikuGLView::MouseWheelChanged(float x, float y)
{
	BGLView::MouseWheelChanged(x, y);
}
