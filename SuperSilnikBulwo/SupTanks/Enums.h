#ifndef ENUMS_H
#define ENUMS_H

// debug enums:
enum DebugInfoShowMode {
	DebugInfoShowMode_No,
	DebugInfoShowMode_ToConsole,
	DebugInfoShowMode_ToFile
};
// end of debug enums

enum GeometryRenderMode {
	GeometryRenderMode_Normal,
	GeometryRenderMode_VBO
};

enum LatelyRenderedTexturedOrColoredRectangle {
	LatelyRenderedTexturedOrColoredRectangle_Textured,
	LatelyRenderedTexturedOrColoredRectangle_Colored,
	LatelyRenderedTexturedOrColoredRectangle_NotRenderedYet,
};

enum ViewMode {
	ViewMode_Orthographic,
	ViewMode_Frustum
};

#endif // ENUMS_H
