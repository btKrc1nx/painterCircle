#include"Practice.h"
#include<QApplication>
int main(int argc, char *argv[]) {
	/*…Ë÷√locale±‡¬Î∑Ω Ω*/
	QTextCodec *codec = QTextCodec::codecForName("utf-8");
	QTextCodec::setCodecForLocale(codec);
	QApplication app(argc, argv);
	Practice w;
	w.show();
	return app.exec();
}