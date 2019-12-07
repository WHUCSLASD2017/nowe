#include "ChangeHeaderWnd.h"
#include "NoweGlobal.h"
#include <QDesktopWidget>
#include <QBuffer>
#include <QXmppVCardManager.h>


ChangeHeaderWnd::ChangeHeaderWnd(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
{
    ui.setupUi(this);
    ui.horizontalSlider->setEnabled(false);

    QDesktopWidget *deskdop=QApplication::desktop();
    move((deskdop->width()-this->width())/2, (deskdop->height()-this->height())/2);


    m_pDivWidget = new QWidget(ui.scrollArea);
    m_pDivWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_pDivWidget->hide();
    m_pDivWidget->installEventFilter(this);

    connect(ui.pushButton_close, &QPushButton::clicked, this, &QWidget::close);
    connect(ui.pushButton_upload, &QPushButton::clicked, this, &ChangeHeaderWnd::onUpload);
    connect(ui.horizontalSlider, &QSlider::valueChanged, this, &ChangeHeaderWnd::onScaledChanged);
    connect(ui.pushButton_anti, &QPushButton::clicked, std::bind(&ChangeHeaderWnd::RotateHeader, this, false));
    connect(ui.pushButton_clock, &QPushButton::clicked, std::bind(&ChangeHeaderWnd::RotateHeader, this, true));
    connect(ui.pushButton_ok, &QPushButton::clicked, this, &ChangeHeaderWnd::onOk);
    connect(ui.pushButton_cancel, &QPushButton::clicked, this, &ChangeHeaderWnd::onCancel);

    connect(this, &ChangeHeaderWnd::updateHeader, [](const QPixmap& avatar) {
        auto myVCard =  Nowe::myVCard();
        auto myVCardManager = Nowe::myClient()->findExtension<QXmppVCardManager>();

        QBuffer avatarData;
        avatarData.open(QIODevice::WriteOnly);
        avatar.save(&avatarData, "PNG");

        myVCard.setPhoto(avatarData.buffer());

        myVCardManager->setClientVCard(myVCard);
        myVCardManager->requestClientVCard();
    });
}

ChangeHeaderWnd::~ChangeHeaderWnd()
{

}

void ChangeHeaderWnd::RotateHeader(bool bClock)
{
    QPixmap pixmap = *(ui.label_header->pixmap());
    QMatrix matrix;
    matrix.rotate(bClock ? 90 : -90);
    pixmap = pixmap.transformed(matrix);
    ui.label_header->setPixmap(pixmap);
}

void ChangeHeaderWnd::LoadHeader(const QPixmap& pixmap)
{
    m_sourceHeader = pixmap;
    int nSliderMinmum = 0;
    QPixmap scaledPixmap;
    if (m_sourceHeader.width() > m_sourceHeader.height())
    {
        scaledPixmap = m_sourceHeader.scaledToHeight(ui.scrollArea->height());
        nSliderMinmum = scaledPixmap.height() * ui.horizontalSlider->maximum() / m_sourceHeader.height();
    }
    else
    {
        scaledPixmap = m_sourceHeader.scaledToWidth(ui.scrollArea->width());
        nSliderMinmum = scaledPixmap.width() * ui.horizontalSlider->maximum() / m_sourceHeader.width();
    }
    ui.scrollAreaWidgetContents->resize(m_sourceHeader.size());
    ui.horizontalSlider->setEnabled(true);
    ui.horizontalSlider->setMinimum(nSliderMinmum);
    ui.horizontalSlider->setValue(nSliderMinmum);
    ui.label_header->setPixmap(scaledPixmap);
    m_pDivWidget->show();
}

bool ChangeHeaderWnd::eventFilter(QObject* watched, QEvent* event)
{
    //绘制遮罩层
    if (m_pDivWidget == watched && QEvent::Paint == event->type())
    {
        m_pDivWidget->resize(ui.scrollArea->size());
        QPainterPath path;
        path.addRect(m_pDivWidget->geometry());
        path.addEllipse(m_pDivWidget->geometry().adjusted(2, 2, -2, -2));
        QPainter p(m_pDivWidget);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(100, 100, 100, 200));
        p.setClipPath(path);
        p.drawRect(m_pDivWidget->geometry());
    }
    return QWidget::eventFilter(watched, event);
}

void ChangeHeaderWnd::onUpload()
{
    QString strFilePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择上传图片"), "./", tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm *.jpeg);;All files (*.*)"));
    if (strFilePath.isEmpty())
    {
        return;
    }
    LoadHeader(QPixmap(strFilePath));
}

void ChangeHeaderWnd::onScaledChanged(int nValue)
{
    QPixmap pixmap = m_sourceHeader.scaled(m_sourceHeader.size() * ((double)nValue / ui.horizontalSlider->maximum()));
    ui.scrollAreaWidgetContents->resize(pixmap.size());
    ui.label_header->setPixmap(pixmap);
}

void ChangeHeaderWnd::onOk()
{
    if (const QPixmap* pHeader = ui.label_header->pixmap())
    {
        emit updateHeader(*pHeader);
        ui.label_tip->setText(QString::fromLocal8Bit("修改头像成功"));
    }
    else
    {
        ui.label_tip->setText(QString::fromLocal8Bit("还未加载头像"));
    }
}

void ChangeHeaderWnd::onCancel()
{
    ui.label_tip->setText(QString::fromLocal8Bit("取消修改头像"));
    m_pDivWidget->hide();
    ui.label_header->clear();
    this->hide();
}

//Mouse Move
void ChangeHeaderWnd::mousePressEvent(QMouseEvent *e)
{
    /* last=e->globalPos();*/
}
void ChangeHeaderWnd::mouseMoveEvent(QMouseEvent *e)
{
    /* int dx = e->globalX() - last.x();
         int dy = e->globalY() - last.y();
         last = e->globalPos();
         move(x()+dx, y()+dy);*/
}
void ChangeHeaderWnd::mouseReleaseEvent(QMouseEvent *e)
{
    /* int dx = e->globalX() - last.x();
     int dy = e->globalY() - last.y();
     move(x()+dx, y()+dy);*/
}

