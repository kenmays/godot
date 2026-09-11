#include "export_plugin.h"

void register_haiku_exporter_types() {
	GDREGISTER_CLASS(EditorExportPlatformHaiku);
}

void register_haiku_exporter() {
	Ref<EditorExportPlatformHaiku> platform;
	platform.instantiate();
	EditorExport::get_singleton()->add_export_platform(platform);
}

void unregister_haiku_exporter() {
}
