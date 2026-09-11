#include "display_server_haiku.h"

#include <Application.h>
#include <Clipboard.h>
#include <Screen.h>

#include "gl_manager_haiku.h"

Vector<String> DisplayServerHaiku::get_rendering_drivers_func() {
	Vector<String> drivers;
#ifdef GLES3_ENABLED
	drivers.push_back("opengl3");
#endif
	return drivers;
}

DisplayServer *DisplayServerHaiku::create_func(const String &p_rendering_driver, DisplayServerEnums::WindowMode p_mode, DisplayServerEnums::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, DisplayServerEnums::Context p_context, int64_t p_parent_window, Error &r_error) {
	DisplayServerHaiku *server = memnew(DisplayServerHaiku(p_rendering_driver, p_mode, p_vsync_mode, p_flags, p_position, p_resolution, p_screen, p_context, p_parent_window, r_error));
	if (r_error != OK) {
		memdelete(server);
		return nullptr;
	}
	return server;
}

void DisplayServerHaiku::register_haiku_driver() {
	register_create_function("haiku", create_func, get_rendering_drivers_func);
}

void DisplayServerHaiku::register_haiku_driver_static() {
	register_haiku_driver();
}

DisplayServerHaiku::WindowData *DisplayServerHaiku::_get_window(DisplayServerEnums::WindowID p_id) {
	return windows.getptr(p_id);
}

const DisplayServerHaiku::WindowData *DisplayServerHaiku::_get_window(DisplayServerEnums::WindowID p_id) const {
	return windows.getptr(p_id);
}

DisplayServerHaiku::DisplayServerHaiku(const String &p_rendering_driver, DisplayServerEnums::WindowMode p_mode, DisplayServerEnums::VSyncMode p_vsync_mode, uint32_t p_flags, const Vector2i *p_position, const Vector2i &p_resolution, int p_screen, DisplayServerEnums::Context p_context, int64_t p_parent_window, Error &r_error) {
	r_error = OK;
	if (be_app == nullptr) {
		r_error = ERR_UNCONFIGURED;
		return;
	}

	BScreen screen;
	BRect frame = screen.Frame();
	Vector2i size = p_resolution;
	if (size.x <= 0 || size.y <= 0) {
		size = Vector2i(1280, 720);
	}
	Vector2i pos = p_position ? *p_position : Vector2i((int)(frame.Width() - size.x) / 2, (int)(frame.Height() - size.y) / 2);
	BRect rect(pos.x, pos.y, pos.x + size.x - 1, pos.y + size.y - 1);

	HaikuWindow *window = memnew(HaikuWindow(rect, "Godot", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE, B_CURRENT_WORKSPACE, this));
	HaikuGLView *view = memnew(HaikuGLView(window->Bounds(), "GodotGL", B_FOLLOW_ALL, B_WILL_DRAW));
	window->AddChild(view);
	WindowData data;
	data.window = window;
	data.view = view;
	data.position = pos;
	data.size = size;
	data.mode = p_mode;
	data.vsync = p_vsync_mode;
	data.focused = false;
	windows.insert(DisplayServerEnums::MAIN_WINDOW_ID, data);
	current_window = DisplayServerEnums::MAIN_WINDOW_ID;
	window->Show();
	view->MakeFocus(true);
}

DisplayServerHaiku::~DisplayServerHaiku() {
	for (KeyValue<DisplayServerEnums::WindowID, WindowData> &E : windows) {
		if (E.value.window && E.value.window->Lock()) {
			E.value.window->Quit();
		}
	}
	windows.clear();
}

bool DisplayServerHaiku::has_feature(DisplayServerEnums::Feature p_feature) const {
	switch (p_feature) {
		case DisplayServerEnums::FEATURE_CLIPBOARD:
		case DisplayServerEnums::FEATURE_MOUSE:
		case DisplayServerEnums::FEATURE_WINDOW_TRANSPARENCY:
			return true;
		default:
			return false;
	}
}

int DisplayServerHaiku::get_screen_count() const { return 1; }

Point2i DisplayServerHaiku::screen_get_position(int p_screen) const {
	if (p_screen != 0) return Point2i();
	BScreen screen;
	BRect r = screen.Frame();
	return Point2i((int)r.left, (int)r.top);
}

Size2i DisplayServerHaiku::screen_get_size(int p_screen) const {
	if (p_screen != 0) return Size2i();
	BScreen screen;
	BRect r = screen.Frame();
	return Size2i((int)r.Width() + 1, (int)r.Height() + 1);
}

Rect2i DisplayServerHaiku::screen_get_usable_rect(int p_screen) const {
	return Rect2i(screen_get_position(p_screen), screen_get_size(p_screen));
}

int DisplayServerHaiku::screen_get_dpi(int p_screen) const { return 96; }
float DisplayServerHaiku::screen_get_scale(int p_screen) const { return 1.0f; }
float DisplayServerHaiku::screen_get_max_scale() const { return 1.0f; }
float DisplayServerHaiku::screen_get_refresh_rate(int p_screen) const { return SCREEN_REFRESH_RATE_FALLBACK; }

Vector<DisplayServerEnums::WindowID> DisplayServerHaiku::get_window_list() const {
	Vector<DisplayServerEnums::WindowID> list;
	for (const KeyValue<DisplayServerEnums::WindowID, WindowData> &E : windows) list.push_back(E.key);
	return list;
}

DisplayServerEnums::WindowID DisplayServerHaiku::create_sub_window(DisplayServerEnums::WindowMode p_mode, DisplayServerEnums::VSyncMode p_vsync_mode, uint32_t p_flags, const Rect2i &p_rect, bool p_exclusive, DisplayServerEnums::WindowID p_transient_parent) {
	DisplayServerEnums::WindowID id = next_window_id++;
	BRect rect(p_rect.position.x, p_rect.position.y, p_rect.end.x - 1, p_rect.end.y - 1);
	HaikuWindow *window = memnew(HaikuWindow(rect, "Godot", B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS, B_CURRENT_WORKSPACE, this));
	HaikuGLView *view = memnew(HaikuGLView(window->Bounds(), "GodotGL", B_FOLLOW_ALL, B_WILL_DRAW));
	window->AddChild(view);
	WindowData data;
	data.window = window; data.view = view; data.position = p_rect.position; data.size = p_rect.size; data.mode = p_mode; data.vsync = p_vsync_mode;
	windows.insert(id, data);
	window->Show();
	return id;
}

void DisplayServerHaiku::show_window(DisplayServerEnums::WindowID p_id) { if (WindowData *w = _get_window(p_id)) w->window->Show(); }
void DisplayServerHaiku::delete_sub_window(DisplayServerEnums::WindowID p_id) { if (p_id == DisplayServerEnums::MAIN_WINDOW_ID) return; if (WindowData *w = _get_window(p_id)) { if (w->window->Lock()) w->window->Quit(); windows.erase(p_id); } }
DisplayServerEnums::WindowID DisplayServerHaiku::get_window_at_screen_position(const Point2i &p_position) const { for (const KeyValue<DisplayServerEnums::WindowID, WindowData> &E : windows) { if (Rect2i(E.value.position, E.value.size).has_point(p_position)) return E.key; } return DisplayServerEnums::INVALID_WINDOW_ID; }
void DisplayServerHaiku::window_attach_instance_id(ObjectID p_id, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) w->instance_id = p_id; }
ObjectID DisplayServerHaiku::window_get_attached_instance_id(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w ? w->instance_id : ObjectID(); }
void DisplayServerHaiku::window_set_rect_changed_callback(const Callable &p_callable, DisplayServerEnums::WindowID p_window) { rect_callback = p_callable; }
void DisplayServerHaiku::window_set_window_event_callback(const Callable &p_callable, DisplayServerEnums::WindowID p_window) { window_callback = p_callable; }
void DisplayServerHaiku::window_set_input_event_callback(const Callable &p_callable, DisplayServerEnums::WindowID p_window) { input_callback = p_callable; }
void DisplayServerHaiku::window_set_input_text_callback(const Callable &p_callable, DisplayServerEnums::WindowID p_window) { }
void DisplayServerHaiku::window_set_title(const String &p_title, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) { if (w->window->Lock()) { w->window->SetTitle(p_title.utf8().get_data()); w->window->Unlock(); } } }
Point2i DisplayServerHaiku::window_get_position(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w ? w->position : Point2i(); }
Point2i DisplayServerHaiku::window_get_position_with_decorations(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); if (!w) return Point2i(); BRect r = w->window->Frame(); return Point2i((int)r.left, (int)r.top); }
void DisplayServerHaiku::window_set_position(const Point2i &p_position, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) { w->position = p_position; if (w->window->Lock()) { w->window->MoveTo(p_position.x, p_position.y); w->window->Unlock(); } _emit_rect_changed(p_window); } }
void DisplayServerHaiku::window_set_size(const Size2i p_size, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) { w->size = p_size; if (w->window->Lock()) { w->window->ResizeTo(p_size.x - 1, p_size.y - 1); w->window->Unlock(); } _emit_rect_changed(p_window); } }
Size2i DisplayServerHaiku::window_get_size(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w ? w->size : Size2i(); }
Size2i DisplayServerHaiku::window_get_size_with_decorations(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); if (!w) return Size2i(); BRect r = w->window->Frame(); return Size2i((int)r.Width() + 1, (int)r.Height() + 1); }
void DisplayServerHaiku::window_set_mode(DisplayServerEnums::WindowMode p_mode, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) { w->mode = p_mode; if (p_mode == DisplayServerEnums::WINDOW_MODE_FULLSCREEN) { BScreen s; BRect r = s.Frame(); if (w->window->Lock()) { w->window->MoveTo(r.left, r.top); w->window->ResizeTo(r.Width(), r.Height()); w->window->Unlock(); } } } }
DisplayServerEnums::WindowMode DisplayServerHaiku::window_get_mode(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w ? w->mode : DisplayServerEnums::WINDOW_MODE_WINDOWED; }
void DisplayServerHaiku::window_set_vsync_mode(DisplayServerEnums::VSyncMode p_mode, DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) w->vsync = p_mode; }
DisplayServerEnums::VSyncMode DisplayServerHaiku::window_get_vsync_mode(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w ? w->vsync : DisplayServerEnums::VSYNC_ENABLED; }
void DisplayServerHaiku::window_set_flag(DisplayServerEnums::WindowFlags p_flag, bool p_enabled, DisplayServerEnums::WindowID p_window) { }
void DisplayServerHaiku::window_move_to_foreground(DisplayServerEnums::WindowID p_window) { if (WindowData *w = _get_window(p_window)) { if (w->window->Lock()) { w->window->Activate(true); w->window->Unlock(); } } }
bool DisplayServerHaiku::window_is_focused(DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); return w && w->focused; }
bool DisplayServerHaiku::window_can_draw(DisplayServerEnums::WindowID p_window) const { return _get_window(p_window) != nullptr; }
bool DisplayServerHaiku::can_any_window_draw() const { return !windows.is_empty(); }
int64_t DisplayServerHaiku::window_get_native_handle(DisplayServerEnums::HandleType p_type, DisplayServerEnums::WindowID p_window) const { const WindowData *w = _get_window(p_window); if (!w) return 0; switch (p_type) { case DisplayServerEnums::WINDOW_HANDLE: return (int64_t)w->window; case DisplayServerEnums::WINDOW_VIEW: return (int64_t)w->view; case DisplayServerEnums::OPENGL_CONTEXT: return (int64_t)w->view; default: return 0; } }
void DisplayServerHaiku::process_events() { if (be_app) be_app->Pulse(); }
void DisplayServerHaiku::swap_buffers() { WindowData *w = _get_window(current_window); if (w && w->view) w->view->SwapBuffers(); }

void DisplayServerHaiku::clipboard_set(const String &p_text) { if (!be_clipboard || !be_clipboard->Lock()) return; be_clipboard->Clear(); BMessage *data = be_clipboard->Data(); if (data) data->AddData("text/plain", B_MIME_TYPE, p_text.utf8().get_data(), p_text.utf8().size()); be_clipboard->Commit(); be_clipboard->Unlock(); }
String DisplayServerHaiku::clipboard_get() const { if (!be_clipboard || !be_clipboard->Lock()) return String(); const BMessage *data = be_clipboard->Data(); const char *text = nullptr; ssize_t size = 0; String result; if (data && data->FindData("text/plain", B_MIME_TYPE, (const void **)&text, &size) == B_OK) result = String::utf8(text, size); be_clipboard->Unlock(); return result; }
bool DisplayServerHaiku::clipboard_has() const { return !clipboard_get().is_empty(); }
void DisplayServerHaiku::mouse_set_mode(DisplayServerEnums::MouseMode p_mode) { mouse_mode = p_mode; }
void DisplayServerHaiku::_emit_window_event(DisplayServerEnums::WindowEvent p_event, DisplayServerEnums::WindowID p_id) { if (window_callback.is_valid()) window_callback.call(p_event, p_id); }
void DisplayServerHaiku::_emit_rect_changed(DisplayServerEnums::WindowID p_id) { if (rect_callback.is_valid()) rect_callback.call(p_id); }
void DisplayServerHaiku::window_close_requested(DisplayServerEnums::WindowID p_id) { _emit_window_event(DisplayServerEnums::WINDOW_EVENT_CLOSE_REQUEST, p_id); }
void DisplayServerHaiku::window_frame_changed(HaikuWindow *p_window) { for (KeyValue<DisplayServerEnums::WindowID, WindowData> &E : windows) if (E.value.window == p_window) { BRect r = p_window->Frame(); E.value.position = Point2i((int)r.left, (int)r.top); BRect vr = p_window->Bounds(); E.value.size = Size2i((int)vr.Width() + 1, (int)vr.Height() + 1); _emit_rect_changed(E.key); break; } }
void DisplayServerHaiku::window_activation_changed(HaikuWindow *p_window, bool p_active) { for (KeyValue<DisplayServerEnums::WindowID, WindowData> &E : windows) if (E.value.window == p_window) { E.value.focused = p_active; _emit_window_event(p_active ? DisplayServerEnums::WINDOW_EVENT_FOCUS_IN : DisplayServerEnums::WINDOW_EVENT_FOCUS_OUT, E.key); break; } }
bool DisplayServerHaiku::handle_window_message(HaikuWindow *p_window, BMessage *p_message) { return false; }
