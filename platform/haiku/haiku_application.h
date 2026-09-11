#pragma once

#include <Application.h>

class OS_Haiku;

class HaikuApplication : public BApplication {
	OS_Haiku *owner = nullptr;
	bool quit_requested = false;
public:
	HaikuApplication(OS_Haiku *p_owner);
	void Pulse() override;
	bool QuitRequested() override;
	void request_quit();
};
