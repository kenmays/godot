#pragma once

#include "servers/display/display_server_headless.h"

#include "haiku_window.h"
#include "haiku_gl_view.h"

class DisplayServerHaiku : public DisplayServerHeadless {
	GDSOFTCLASS(DisplayServerHaiku, DisplayServerHeadless);

	struct WindowData {
		HaikuWindow *window = nullptr;
		HaikuGLView *view = nullptr;
		Vector2i position;
		Vector2i size;
		DisplayServerEnums::WindowMode mode = DisplayServerEnums::WINDOW_MODE_WINDOWED;
		DisplayServerEnums::VSyncMode vsync = DisplayServerEnums::VSYNC_ENABLED;
		bool focused = false;
		ObjectID instance_id;
	};

	HashMap<DisplayServerEnums::WindowID, WindowData> windows;
	DisplayServerEnums::WindowID next_window_id = DisplayServerEnums::MAIN_WINDOW_ID + 1;
	DisplayServerEnums::WindowID current_window = DisplayServerEnums::MAIN_WINDOW_ID;
	Point2i mouse_position;
	DisplayServerEnums::MouseMode mouse_mode = DisplayServerEnums::MOUSE_MODE_VISIBLE;
	Callable input_callback;
	Callable window_callback;
	Callable rect_callback;
	String clipboard;

	static Vector<String> get_rendering_drivers_func();
	static DisplayServer *create_func(const String &, DisplayServerEnums::WindowMode, DisplayServerEnums::VSyncMode, uint32_t, const Vector2i *, const Vector2i &, int, DisplayServerEnums::Context, int64_t, Error &);
	static void register_haiku_driver();

	WindowData *_get_window(DisplayServerEnums::WindowID p_id);
	const WindowData *_get_window(DisplayServerEnums::WindowID p_id) const;
	void _emit_window_event(DisplayServerEnums::WindowEvent p_event, DisplayServerEnums::WindowID p_id);
	void _emit_rect_changed(DisplayServerEnums::WindowID p_id);

public:
	static void register_haiku_driver_static();

	String get_name() const override { return "haiku"; }
	bool has_feature(DisplayServerEnums::Feature p_feature) const override;
	int get_screen_count() const override;
	int get_primary_screen() const override { return 0; }
	Point2i screen_get_position(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;
	Size2i screen_get_size(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;
	Rect2i screen_get_usable_rect(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;
	int screen_get_dpi(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;
	float screen_get_scale(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;
	float screen_get_max_scale() const override;
	float screen_get_refresh_rate(int p_screen = DisplayServerEnums::SCREEN_OF_MAIN_WINDOW) const override;

	Vector<DisplayServerEnums::WindowID> get_window_list() const override;
	DisplayServerEnums::WindowID create_sub_window(DisplayServerEnums::WindowMode, DisplayServerEnums::VSyncMode, uint32_t, const Rect2i &, bool, DisplayServerEnums::WindowID) override;
	void show_window(DisplayServerEnums::WindowID) override;
	void delete_sub_window(DisplayServerEnums::WindowID) override;
	DisplayServerEnums::WindowID get_window_at_screen_position(const Point2i &) const override;
	void window_attach_instance_id(ObjectID, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	ObjectID window_get_attached_instance_id(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	void window_set_rect_changed_callback(const Callable &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	void window_set_window_event_callback(const Callable &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	void window_set_input_event_callback(const Callable &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	void window_set_input_text_callback(const Callable &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	void window_set_title(const String &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	int window_get_current_screen(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override { return 0; }
	void window_set_current_screen(int, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override {}
	Point2i window_get_position(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	Point2i window_get_position_with_decorations(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	void window_set_position(const Point2i &, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	void window_set_size(const Size2i, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	Size2i window_get_size(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	Size2i window_get_size_with_decorations(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	void window_set_mode(DisplayServerEnums::WindowMode, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	DisplayServerEnums::WindowMode window_get_mode(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	void window_set_vsync_mode(DisplayServerEnums::VSyncMode, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	DisplayServerEnums::VSyncMode window_get_vsync_mode(DisplayServerEnums::WindowID) const override;
	bool window_is_maximize_allowed(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override { return true; }
	void window_set_flag(DisplayServerEnums::WindowFlags, bool, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	bool window_get_flag(DisplayServerEnums::WindowFlags, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override { return false; }
	void window_move_to_foreground(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) override;
	bool window_is_focused(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	bool window_can_draw(DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;
	bool can_any_window_draw() const override;
	int64_t window_get_native_handle(DisplayServerEnums::HandleType, DisplayServerEnums::WindowID = DisplayServerEnums::MAIN_WINDOW_ID) const override;

	void process_events() override;
	void swap_buffers() override;
	void clipboard_set(const String &) override;
	String clipboard_get() const override;
	bool clipboard_has() const override;
	void mouse_set_mode(DisplayServerEnums::MouseMode) override;
	Point2i mouse_get_position() const override { return mouse_position; }

	void window_close_requested(DisplayServerEnums::WindowID);
	void window_frame_changed(HaikuWindow *);
	void window_activation_changed(HaikuWindow *, bool);
	bool handle_window_message(HaikuWindow *, BMessage *);

	DisplayServerHaiku(const String &, DisplayServerEnums::WindowMode, DisplayServerEnums::VSyncMode, uint32_t, const Vector2i *, const Vector2i &, int, DisplayServerEnums::Context, int64_t, Error &);
	~DisplayServerHaiku() override;
};
