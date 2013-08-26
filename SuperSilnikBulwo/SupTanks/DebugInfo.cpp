#include "DebugInfo.h"

void DebugInfo::printDebugInfo(std::string debugText) {
	if (debugInfoShowMode == DebugInfoShowMode_ToConsole) {
		std::cout << debugText;
	} else if (debugInfoShowMode == DebugInfoShowMode_ToFile) {
		// TODO: code it
	}
}
