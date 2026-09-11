#pragma once

#include "drivers/unix/os_unix.h"

class AudioDriverHaiku;

class OS_Haiku : public OS_Unix {
	MainLoop *main_loop = nullptr;
	AudioDriverHaiku *audio_driver = nullptr;

protected:
	void initialize() override;
	void finalize() override;
	void set_main_loop(MainLoop *p_main_loop) override;

public:
	String get_identifier() const override { return "haiku"; }
	String get_name() const override { return "Haiku"; }
	String get_distribution_name() const override { return "Haiku"; }
	String get_version() const override;
	MainLoop *get_main_loop() const override { return main_loop; }
	String get_config_path() const override;
	String get_data_path() const override;
	String get_cache_path() const override;
	String get_system_dir(SystemDir p_dir, bool p_shared_storage = true) const override;
	String get_unique_id() const override;
	String get_processor_name() const override;
	bool is_sandboxed() const override { return false; }
	void alert(const String &p_alert, const String &p_title = "ALERT!") override;
	void run();
	void delete_main_loop();

	OS_Haiku();
	~OS_Haiku() override;
};
