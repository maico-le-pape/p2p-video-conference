#include "autoresizeimageview.h"

AutoResizeImageView::AutoResizeImageView(QWidget *parent) :
    QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_imageItem(0),
    m_border(0),
    m_delayed(false)
{
    setScene(m_scene);
}

void AutoResizeImageView::setImage(const QImage &image) {
    m_scene->clear();
    m_imageItem = m_scene->addPixmap(QPixmap::fromImage(image));
    QPen pen;
    pen.setWidth(5);
    if(m_delayed)
	pen.setColor(Qt::red);
    else
	pen.setColor(Qt::green);
    m_border = m_scene->addRect(m_imageItem->boundingRect(),pen);
    fitInView(m_imageItem, Qt::KeepAspectRatio);
}

void AutoResizeImageView::resizeEvent(QResizeEvent *event)
{
    if(m_imageItem != 0)
        fitInView(m_imageItem, Qt::KeepAspectRatio);
}

void AutoResizeImageView::setDelayed ( bool delayed )
{
    m_delayed = delayed;
}
