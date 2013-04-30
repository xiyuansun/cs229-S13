#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QLabel>
#include "../../lib/io/aut.h"

class QPushButton;
class QSlider;
class QSpinBox;
class QScrollArea;
class BoardDisp;

// Provides conrols for the GUI simulation
class OptionsDialog : public QDialog
{
    Q_OBJECT

    public:
        OptionsDialog(AutFile* a, QWidget* parent = 0);

    signals:
        // Sent when switching pause/play
        void changeState(bool play);

        // Sent when quitting.
        void quitting();
    
    public slots:
        // Called when generation number is updated
        void changeGen(QString &text) { genNumLabel->setText(text); }

    private slots:
        // Called when playButton is clicked.
        void playClicked();

    protected:
        // closeEvent inherited from parent.
        void closeEvent(QCloseEvent* e);

    private:
        // Says "Zoom factor:"
        QLabel* zoomLabel;

        // Says "Delay:"
        QLabel* delayLabel;

        // Says "Generation:"
        QLabel* genLabel;

        // Generation number
        QLabel* genNumLabel;

        // Push to quit
        QPushButton* quitButton;

        // Push to play/pause
        QPushButton* playButton;

        // Push to step
        QPushButton* stepButton;

        // Displays zoom factor
        QSpinBox* zoomBox;

        // Displays delay value
        QSpinBox* delayBox;

        // Zoom slider, alters value in zoomBox
        QSlider* zoomSlide;

        // Delay slider, alters value in delayBox
        QSlider* delaySlide;

        //Simulation window
        QScrollArea* scrollArea;
};

#endif
