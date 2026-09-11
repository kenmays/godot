#include "export.h"
#include "export_plugin.h"

#include "core/object/class_db.h"
#include "editor/export/editor_export.h"

void register_haiku_exporter_types() {
	GDREGISTER_VIRTUAL_CLASS(EditorExportPlatformHaiku);
}

void register_haiku_exporter() {
	Ref<EditorExportPlatformHaiku> platform;
	platform.instantiate();
	EditorExport::get_singleton()->add_export_platform(platform);
}

void unregister_haiku_exporter() {
}
