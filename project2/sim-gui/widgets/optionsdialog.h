#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "../../lib/io/aut.h"

class QLabel;
class QPushButton;
class QSlider;
class QSpinBox;
class QScrollArea;
class BoardDisp;

class OptionsDialog : public QDialog
{
    Q_OBJECT

    public:
        OptionsDialog(AutFile* a, QWidget* parent = 0);

    signals:
        void changeState(bool play);
        void quitting();
    
    public slots:
        void changeGen(QString &text);

    private slots:
        void playClicked();

    protected:
        void closeEvent(QCloseEvent* e);

    private:
        QLabel* zoomLabel;
        QLabel* delayLabel;
        QLabel* genLabel;
        QLabel* genNumLabel;
        QPushButton* quitButton;
        QPushButton* playButton;
        QPushButton* stepButton;
        QSpinBox* zoomBox;
        QSpinBox* delayBox;
        QSlider* zoomSlide;
        QSlider* delaySlide;
        QScrollArea* scrollArea;
};

#endif
