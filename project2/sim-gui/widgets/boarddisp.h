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

        // Sets the zoom factor to newZoom
        void setZoomFactor(int newZoom);

        // Returns zoomFactor
        int zoomFactor() const { return zoom; }

        // Size hint for QScrollArea
        QSize sizeHint() const;
    
    signals:
        // Sent when generation number is updated
        void updateGen(QString &text); 

    public slots:
        // Go forward one generation
        void step();

        // Change zoom
        void zoomChanged(int newZoom);

        // Change delay
        void delayChanged(int newDelay);

        // Toggle play/pause
        void changeState(bool play);

    protected:
        // Update widget
        void paintEvent(QPaintEvent *event);

    private:
        // Update backing image
        void updateImage();

        // Set pixel in backing image
        void setImagePixel(int x, int y, Color c);

        // Rectangle of pixel on screen
        QRect pixelRect(int i, int j) const;

        // Backing image
        QImage image;

        // Zoom factor
        int zoom;

        // Generation number
        int generation;

        // Timer for play state
        QTimer* timer;

        // Backing board.
        Board* b;
};

#endif
