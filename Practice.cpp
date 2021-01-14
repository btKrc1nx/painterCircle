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
	/*ȡһ�������ξ��Σ�����ѡȡ��ǰ���ڵĿ�*/
	int side = qMin(w, h);
	QRect rect((w - side) / 2, (h - side) / 2, side, side);
		/*����һ������100���������ӿ�*/
	painter.setViewport(rect);
	/*�����ӿڵ�ԭ������Ϊ-50��-50������Ϊ100��ͬ���ӿڵĳ���*/
	painter.setWindow(-100, -100, 200, 200);
		QLinearGradient linearGrad;
	linearGrad.setColorAt(0, Qt::yellow);
	linearGrad.setColorAt(1, Qt::red);
	linearGrad.setSpread(QGradient::PadSpread);
	painter.setBrush(linearGrad);
	painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
	for (int i = 0; i < 36; ++i) {
		/*����Բ�ĺͰ뾶*/
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
	msg->setText(QStringLiteral("��ô����ͼ���࿴�᣿"));
	msg->setWindowTitle("verification");
	msg->setButtonText(QMessageBox::Yes, QStringLiteral("������"));
	msg->setButtonText(QMessageBox::No, QStringLiteral("��"));
	if (msg->exec() == QMessageBox::Yes)
		event->accept();
	else
		event->ignore();
}