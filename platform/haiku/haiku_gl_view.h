#ifndef HAIKU_GL_VIEW_H
#define HAIKU_GL_VIEW_H

#include <GLView.h>

class HaikuGLView : public BGLView {
public:
	HaikuGLView(BRect frame, const char *name, uint32 resizingMode,
		uint32 options);
	~HaikuGLView() override = default;

	void FrameResized(float width, float height) override;
	void AttachedToWindow() override;
	void DetachedFromWindow() override;
	void KeyDown(const char *bytes, int32 numBytes) override;
	void KeyUp(const char *bytes, int32 numBytes) override;
	void MouseDown(BPoint where) override;
	void MouseUp(BPoint where) override;
	void MouseMoved(BPoint where, uint32 transit, const BMessage *dragMessage) override;
	void MouseWheelChanged(float x, float y) override;
};

#endif
