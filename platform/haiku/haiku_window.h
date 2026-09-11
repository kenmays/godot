#pragma once

#include <Window.h>

class DisplayServerHaiku;

class HaikuWindow : public BWindow {
	DisplayServerHaiku *owner = nullptr;
public:
	HaikuWindow(BRect frame, const char *title, window_type type, uint32 flags, uint32 workspace, DisplayServerHaiku *p_owner);
	virtual bool QuitRequested() override;
	virtual void FrameMoved(BPoint origin) override;
	virtual void FrameResized(float width, float height) override;
	virtual void WindowActivated(bool active) override;
	virtual void MessageReceived(BMessage *message) override;
};
