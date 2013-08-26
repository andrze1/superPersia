#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "Enums.h"

//debug flags
	const bool debugSceneMoving = true;
	const float debugScreenMovingSpeed = 10.0f; // in pixels per frame
	const DebugInfoShowMode debugInfoShowMode = DebugInfoShowMode_ToConsole;
//end of debug flags

static const char* mainWindowCaptionText = "Prince of Przemo";

const GeometryRenderMode geometryRenderMode = GeometryRenderMode_VBO;

const ViewMode viewMode = ViewMode_Orthographic;
//const ViewMode viewMode = ViewMode_Frustum;

const int startingWindowPositionX = 100; // in pixels
const int startingWindowPositionY = 100; // in pixels

const int startingScreenResolutionWidth = 1024; // in pixels
const int startingScreenResolutionHeight = 600; // in pixels

const float backgroundScreenColor[] = {1.0f, 1.0f, 1.0f, 0.0f};

#endif // CONSTANTS_H
