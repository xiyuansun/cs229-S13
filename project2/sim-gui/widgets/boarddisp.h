#ifndef BOARDDISP_H
#define BOARDDISP_H

#include <QColor>
#include <QImage>
#include <QWidget>
#include "../../lib/board.h"
#include "../../lib/color.h"

class OptionsDialog;
class QTimer;

class BoardDisp : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor)

    public:
        BoardDisp(int x_size, int y_size, Board* b, QWidget *parent = 0);
        void setZoomFactor(int newZoom);
        int zoomFactor() const { return zoom; }
        QSize sizeHint() const;
    
    signals:
       void updateGen(QString &text); 

    public slots:
        void step();
        void zoomChanged(int newZoom);
        void delayChanged(int newDelay);
        void changeState(bool play);

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        void updateImage();
        void setImagePixel(int x, int y, Color c);
        QRect pixelRect(int i, int j) const;
        QImage image;
        int zoom;
        int generation;
        QTimer* timer;
        Board* b;
};

#endif
