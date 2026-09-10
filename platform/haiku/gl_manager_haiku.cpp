#include "gl_manager_haiku.h"

#include "haiku_gl_view.h"

#include <GL/gl.h>

status_t GLManagerHaiku::initialize()
{
	return B_OK;
}

status_t GLManagerHaiku::make_current(BGLView *view)
{
	if (view == nullptr)
		return B_BAD_VALUE;
	return view->LockGL() ? B_OK : B_ERROR;
}

void GLManagerHaiku::release_current(BGLView *view)
{
	if (view != nullptr)
		view->UnlockGL();
}

void GLManagerHaiku::swap_buffers(BGLView *view)
{
	if (view != nullptr)
		view->SwapBuffers();
}
