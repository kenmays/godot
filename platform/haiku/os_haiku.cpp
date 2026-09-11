#include "os_haiku.h"

#include <FindDirectory.h>
#include <Path.h>
#include <Roster.h>
#include <StorageKit.h>
#include <sys/utsname.h>

#include "audio_driver_haiku.h"
#include "display_server_haiku.h"
#include "servers/audio/audio_driver.h"

OS_Haiku::OS_Haiku() {
}

OS_Haiku::~OS_Haiku() {
	finalize();
}

void OS_Haiku::initialize() {
	OS_Unix::initialize_core();
	DisplayServerHaiku::register_haiku_driver_static();
	audio_driver = memnew(AudioDriverHaiku);
	AudioDriverManager::add_driver(audio_driver);
}

void OS_Haiku::finalize() {
	delete_main_loop();
	OS_Unix::finalize_core();
}

void OS_Haiku::set_main_loop(MainLoop *p_main_loop) {
	main_loop = p_main_loop;
}

void OS_Haiku::delete_main_loop() {
	if (main_loop) {
		memdelete(main_loop);
		main_loop = nullptr;
	}
}

String OS_Haiku::get_version() const {
	utsname u;
	if (uname(&u) == 0) return String::utf8(u.release);
	return "unknown";
}

String OS_Haiku::get_config_path() const {
	char path[B_PATH_NAME_LENGTH];
	if (find_directory(B_USER_SETTINGS_DIRECTORY, -1, false, path, sizeof(path)) == B_OK) return String::utf8(path).path_join("Godot");
	return get_user_data_dir("Godot");
}

String OS_Haiku::get_data_path() const {
	char path[B_PATH_NAME_LENGTH];
	if (find_directory(B_USER_NONPACKAGED_DATA_DIRECTORY, -1, false, path, sizeof(path)) == B_OK) return String::utf8(path).path_join("Godot");
	return get_config_path();
}

String OS_Haiku::get_cache_path() const {
	char path[B_PATH_NAME_LENGTH];
	if (find_directory(B_USER_CACHE_DIRECTORY, -1, false, path, sizeof(path)) == B_OK) return String::utf8(path).path_join("Godot");
	return get_config_path().path_join("cache");
}

String OS_Haiku::get_system_dir(SystemDir p_dir, bool p_shared_storage) const {
	directory_which which = B_USER_DIRECTORY;
	switch (p_dir) {
		case SYSTEM_DIR_DESKTOP: which = B_DESKTOP_DIRECTORY; break;
		case SYSTEM_DIR_DOCUMENTS: which = B_USER_DIRECTORY; break;
		case SYSTEM_DIR_DOWNLOADS: which = B_USER_DIRECTORY; break;
		case SYSTEM_DIR_MUSIC: which = B_USER_DIRECTORY; break;
		case SYSTEM_DIR_PICTURES: which = B_USER_DIRECTORY; break;
		case SYSTEM_DIR_VIDEOS: which = B_USER_DIRECTORY; break;
		default: which = B_USER_DIRECTORY; break;
	}
	char path[B_PATH_NAME_LENGTH];
	if (find_directory(which, -1, false, path, sizeof(path)) == B_OK) return String::utf8(path);
	return String();
}

String OS_Haiku::get_unique_id() const {
	char path[B_PATH_NAME_LENGTH];
	if (find_directory(B_SYSTEM_DATA_DIRECTORY, -1, false, path, sizeof(path)) == B_OK) return String::utf8(path).md5_text();
	return get_processor_name().md5_text();
}

String OS_Haiku::get_processor_name() const {
	utsname u;
	if (uname(&u) == 0) return String::utf8(u.machine);
	return "Haiku";
}

void OS_Haiku::alert(const String &p_alert, const String &p_title) {
	print_line(p_title + ": " + p_alert);
}

void OS_Haiku::run() {
	if (!main_loop) return;
	main_loop->initialize();
	bool quit = false;
	while (!quit) {
		DisplayServer *display = DisplayServer::get_singleton();
		if (display) display->process_events();
		if (main_loop->iteration(&quit)) quit = true;
	}
	main_loop->finalize();
}
