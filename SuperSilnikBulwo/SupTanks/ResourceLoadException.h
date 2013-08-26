#ifndef RESOURCELOADEXCEPTION_H
#define RESOURCELOADEXCEPTION_H

#include <string>

class ResourceLoadException {

	public:
		std::string resourceName;

		ResourceLoadException(std::string resourceName) : resourceName(resourceName) {}
};

#endif
