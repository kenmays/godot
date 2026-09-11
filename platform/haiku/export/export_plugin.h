#pragma once

#include "editor/export/editor_export_platform_pc.h"

class EditorExportPlatformHaiku : public EditorExportPlatformPC {
	GDCLASS(EditorExportPlatformHaiku, EditorExportPlatformPC);
public:
	EditorExportPlatformHaiku();
	String get_template_file_name(const String &p_target, const String &p_arch) const override;
};
