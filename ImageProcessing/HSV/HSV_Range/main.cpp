#include "HSV_RangeWindow.h"
#include <QtWidgets/QApplication>
#include "HSV_RangeApplication.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	HSV_Range::HSV_RangeWindow window;
	window.show();

	h13::HSV_RangeApplication app(window);
	app.Run();

	return a.exec();
}
