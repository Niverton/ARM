#include <iostream>
#include <unistd.h>

#include "canvas.h"

int main(int argc, char* argv[]){
	std::cout << "arm" << std::endl;

	QApplication app(argc, argv);
    QWidget window;
    window.resize(320, 240);
    window.show();
    window.setWindowTitle(
        QApplication::translate("toplevel", "Top-level widget"));
    
	Canvas *pCanvas = new Canvas(&window);

	//Taille fixé en dur, à modifier si on a le temps
	pCanvas->resize(640, 480);
	window.resize(640, 480);

	pCanvas->show();
	
	return app.exec();
}
