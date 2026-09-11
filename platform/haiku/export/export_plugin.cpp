#include "export_plugin.h"

EditorExportPlatformHaiku::EditorExportPlatformHaiku() {
	set_name("Haiku");
	set_os_name("Haiku");
	add_platform_feature("haiku");
	add_platform_feature("desktop");
}

String EditorExportPlatformHaiku::get_template_file_name(const String &p_target, const String &p_arch) const {
	return "haiku_" + p_target + "." + p_arch;
}
