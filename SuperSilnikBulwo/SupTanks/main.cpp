#include "RenderWindow.h"

int main(int argc, char* argv[]) {
	RenderWindow renderWindow;
    renderWindow.initialize(argc, argv);
	renderWindow.applicationMainLoop();
}
