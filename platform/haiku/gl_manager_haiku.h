#ifndef HAIKU_GL_MANAGER_H
#define HAIKU_GL_MANAGER_H

#include <SupportDefs.h>

class BWindow;
class BGLView;

class GLManagerHaiku {
public:
	GLManagerHaiku() = default;
	~GLManagerHaiku() = default;

	status_t initialize();
	status_t make_current(BGLView *view);
	void release_current(BGLView *view);
	void swap_buffers(BGLView *view);

	const char *version() const { return fVersion; }
	const char *vendor() const { return fVendor; }
	const char *renderer() const { return fRenderer; }

private:
	const char *fVersion = "";
	const char *fVendor = "";
	const char *fRenderer = "";
};

#endif
