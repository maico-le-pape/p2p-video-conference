#ifndef AUTORESIZEIMAGEVIEW_H
#define AUTORESIZEIMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QResizeEvent>

class AutoResizeImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AutoResizeImageView(QWidget *parent = 0);
    void setImage(const QImage &image);
    
protected:
    void resizeEvent(QResizeEvent *event);
    
private:
    QGraphicsScene* m_scene;
    QGraphicsItem* m_imageItem;
};

#endif // AUTORESIZEIMAGEVIEW_H
