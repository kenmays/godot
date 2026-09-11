#include "haiku_window.h"

#include "display_server_haiku.h"

HaikuWindow::HaikuWindow(BRect frame, const char *title, window_type type, uint32 flags, uint32 workspace, DisplayServerHaiku *p_owner) :
		BWindow(frame, title, type, flags, workspace), owner(p_owner) {
}

bool HaikuWindow::QuitRequested() {
	if (owner) {
		owner->window_close_requested(DisplayServerEnums::MAIN_WINDOW_ID);
	}
	return false;
}

void HaikuWindow::FrameMoved(BPoint origin) {
	BWindow::FrameMoved(origin);
	if (owner) {
		owner->window_frame_changed(this);
	}
}

void HaikuWindow::FrameResized(float width, float height) {
	BWindow::FrameResized(width, height);
	if (owner) {
		owner->window_frame_changed(this);
	}
}

void HaikuWindow::WindowActivated(bool active) {
	BWindow::WindowActivated(active);
	if (owner) {
		owner->window_activation_changed(this, active);
	}
}

void HaikuWindow::MessageReceived(BMessage *message) {
	if (owner && owner->handle_window_message(this, message)) {
		return;
	}
	BWindow::MessageReceived(message);
}
