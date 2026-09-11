#include "haiku_application.h"

#include "os_haiku.h"

HaikuApplication::HaikuApplication(OS_Haiku *p_owner) : BApplication("application/x-vnd.godot-haiku"), owner(p_owner) {
}

void HaikuApplication::Pulse() {
	if (owner) owner->process_application_pulse();
	if (quit_requested) PostMessage(B_QUIT_REQUESTED);
}

bool HaikuApplication::QuitRequested() {
	return true;
}

void HaikuApplication::request_quit() {
	quit_requested = true;
}
