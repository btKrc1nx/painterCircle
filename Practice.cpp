#pragma warning(disable:26444)
#include"Practice.h"
Practice::Practice(QWidget *parent) :QWidget(parent) {
	ui.setupUi(this);
	iniUI();
	QObject::connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuRequested(const QPoint &)));
	QObject::connect(ui.actSetColor, SIGNAL(triggered()), this, SLOT(onActSetColorTriggered()));
}
Practice::~Practice() {
	QMessageBox::information(this, "Test", "executing destructor:~Practice()", QMessageBox::Ok);
}

void Practice::iniUI() {
	setMouseTracking(true);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	this->setPalette(QPalette(Qt::black));
	setAutoFillBackground(true);
	ellipseColor = QColor(170,85,0);
}
void Practice::onCustomContextMenuRequested(const QPoint &pos) {
	QMenu *menu = new QMenu(this);
	menu->addAction(ui.actSetColor);
	menu->addAction(ui.actQuit);
	menu->exec(QCursor::pos());
	delete menu;
}
void Practice::onActSetColorTriggered() {
	QColor color = QColorDialog::getColor(ellipseColor, this, "get color");
	if (color.isValid())
		ellipseColor = color;
}
void Practice::paintEvent(QPaintEvent *event) {
	int w = width(), h = height();
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing);
	QPen pen;
	pen.setColor(ellipseColor);
	painter.setPen(pen);
	/*取一个正方形矩形，长宽选取当前窗口的宽*/
	int side = qMin(w, h);
	QRect rect((w - side) / 2, (h - side) / 2, side, side);
		/*定义一个长宽100的正方形视口*/
	painter.setViewport(rect);
	/*设置视口的原点坐标为-50，-50，长宽都为100，同步视口的长宽*/
	painter.setWindow(-100, -100, 200, 200);
		QLinearGradient linearGrad;
	linearGrad.setColorAt(0, Qt::yellow);
	linearGrad.setColorAt(1, Qt::red);
	linearGrad.setSpread(QGradient::PadSpread);
	painter.setBrush(linearGrad);
	painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
	for (int i = 0; i < 36; ++i) {
		/*设置圆心和半径*/
		painter.drawEllipse(QPoint(50,0), 50, 50);
		painter.rotate(10);
	}
	painter.drawRect(-100, -100, 200, 200);
}
void Practice::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		moving = true;
		lastRelPos = event->globalPos() - this->pos();
	}
	return QWidget::mousePressEvent(event);
}
void Practice::mouseMoveEvent(QMouseEvent *event) {
	QPoint newWindowPos = event->globalPos() - lastRelPos;
	if (moving && (event->buttons() & Qt::LeftButton) && (newWindowPos - this->pos()).manhattanLength() >= QApplication::startDragDistance()) {
		this->move(newWindowPos);
		lastRelPos = event->globalPos() - newWindowPos;
	}
	return QWidget::mouseMoveEvent(event);
}
void Practice::mouseReleaseEvent(QMouseEvent *event) {
	moving = false;
	return QWidget::mouseReleaseEvent(event);
}
void Practice::closeEvent(QCloseEvent *event) {
	QMessageBox *msg = new QMessageBox(this);
	msg->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msg->setText(QStringLiteral("这么吊的图不多看会？"));
	msg->setWindowTitle("verification");
	msg->setButtonText(QMessageBox::Yes, QStringLiteral("不看了"));
	msg->setButtonText(QMessageBox::No, QStringLiteral("好"));
	if (msg->exec() == QMessageBox::Yes)
		event->accept();
	else
		event->ignore();
}