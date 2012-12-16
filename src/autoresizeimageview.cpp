#include "autoresizeimageview.h"

AutoResizeImageView::AutoResizeImageView(QWidget *parent) :
    QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_imageItem(0)
{
    setScene(m_scene);
}

void AutoResizeImageView::setImage(const QImage &image) {
    m_scene->clear();
    m_imageItem = m_scene->addPixmap(QPixmap::fromImage(image));
    fitInView(m_imageItem, Qt::KeepAspectRatio);
}

void AutoResizeImageView::resizeEvent(QResizeEvent *event)
{
    if(m_imageItem != 0)
        fitInView(m_imageItem, Qt::KeepAspectRatio);
}
