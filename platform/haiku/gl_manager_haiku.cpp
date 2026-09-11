#include "gl_manager_haiku.h"

#include "haiku_gl_view.h"

status_t GLManagerHaiku::initialize()
{
	return B_OK;
}

status_t GLManagerHaiku::make_current(BGLView *view)
{
	if (view == nullptr)
		return B_BAD_VALUE;
	view->LockGL();
	return B_OK;
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
