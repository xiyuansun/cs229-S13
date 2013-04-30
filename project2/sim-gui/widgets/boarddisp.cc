#include <QtGui>
#include <string>

#include "boarddisp.h"
#include "optionsdialog.h"

BoardDisp::BoardDisp(int x_size, int y_size, Board* b, QWidget* parent) : QWidget(parent)
{
    // Some QT size/resize rules
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    //default zoom, generation
    zoom = 8;
    generation = 0;
    
    // Set up timer with default delay
    timer = new QTimer(this);
    timer->setInterval(100);
    
    // Connect timer to step()
    connect(timer, SIGNAL(timeout()), this, SLOT(step()));
    
    // Setup backing image
    image = QImage(x_size, y_size, QImage::Format_ARGB32);
    image.fill(qRgba(255, 255, 255, 255));
    
    // Store backing board
    this->b = b;
    
    // Display generation 0
    updateImage();
}

void BoardDisp::setZoomFactor(int newZoom)
{
    if(newZoom < 1) newZoom = 1;
    
    // If zoom changed, we need to let
    // the QT magic fairies know.
    if(newZoom != zoom)
    {
        zoom = newZoom;
        update();
        updateGeometry();
    }
}

QSize BoardDisp::sizeHint() const
{
    QSize size = zoom * image.size();

    if(zoom >= 3) size += QSize(1, 1);

    return size;
}

void BoardDisp::step()
{
    //nextGen on board
    this->b->next_generation();
   
    updateImage();
    
    // Cause label to update on control panel
    ++generation;
    QString text = QString::number(generation);
    emit updateGen(text);
}

void BoardDisp::zoomChanged(int newZoom)
{
    // Set new zoom
    setZoomFactor(newZoom);
}

void BoardDisp::delayChanged(int newDelay)
{
    // Set new delay
    timer->setInterval(newDelay);
}

void BoardDisp::changeState(bool play)
{
    if(play)
    {
        timer->start();
    }
    else
    {
        timer->stop();
    }
}

void BoardDisp::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    // Draw lines
    if(zoom >= 3)
    {
        painter.setPen(palette().foreground().color());
        
        for(int i = 0; i <= image.width(); ++i)
        {
            painter.drawLine(zoom * i, 0, zoom * i, zoom * image.height());
        }

        for(int j = 0; j <= image.height(); ++j)
        {
            painter.drawLine(0, zoom * j, zoom * image.width(), zoom * j);
        }
    }
    
    // Update only areas that need it.
    for(int i = 0; i < image.width(); ++i)
    {
        for(int j = 0; j < image.height(); ++j)
        {
            QRect rect = pixelRect(i, j);
            if(!event->region().intersect(rect).isEmpty())
            {
                QColor color = QColor::fromRgba(image.pixel(i, j));
                painter.fillRect(rect, color);
            }
        }
    }
}

void BoardDisp::updateImage()
{
    // Loop over board, pulling values for image.
    for(int y = image.height() - 1; y >= 0; --y)
    {
        for(int x = 0; x < image.width(); ++x)
        {
            Color c = b->get_state_color(b->get_state(x, y, false));
            QRgb img_clr = image.pixel(x, image.height() - (y + 1));
            if(qRed(img_clr) != c.get_red() || qGreen(img_clr) != c.get_green() || qBlue(img_clr) != c.get_blue())
            {
                //Update window.
                setImagePixel(x, image.height() - (y + 1), c);
            }
        }
    }
}

void BoardDisp::setImagePixel(int x, int y, Color state)
{
    if(image.rect().contains(x, y))
    {
        image.setPixel(x, y, qRgba(state.get_red(), state.get_green(), state.get_blue(), 255));
    }

    update(pixelRect(x, y));
}

QRect BoardDisp::pixelRect(int i, int j) const
{
    if(zoom >= 3)
    {
        return QRect(zoom * i + 1, zoom * j + 1, zoom - 1, zoom - 1);
    }
    else
    {
        return QRect(zoom * i, zoom * j, zoom, zoom);
    }
}
