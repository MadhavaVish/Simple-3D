#include <QtWidgets/QApplication>

#include <vld.h>

#include "GUIMain.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	GUIMain window;
	window.resize(1280, 741);
	window.show();

	return app.exec();
}
