#pragma once

#include "core/os/crash_handler.h"

class CrashHandlerHaiku : public CrashHandler {
public:
	void initialize() override {}
	void disable() override {}
};
