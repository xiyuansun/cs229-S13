#include <QtGui>

#include "optionsdialog.h"
#include "boarddisp.h"

OptionsDialog::OptionsDialog(AutFile* a, QWidget* parent) : QDialog(parent)
{
    BoardDisp* disp = new BoardDisp(a->get_x_high() - a->get_x_low() + 1, a->get_y_high() - a->get_y_low() + 1, a->get());

    scrollArea = new QScrollArea;
    scrollArea->setWidget(disp);
    scrollArea->viewport()->setBackgroundRole(QPalette::Dark);
    scrollArea->viewport()->setAutoFillBackground(true);
    scrollArea->setWindowTitle(tr("Icon Editor"));
    scrollArea->setWidgetResizable(true);

    zoomLabel = new QLabel(tr("Zoom factor:"));
    delayLabel = new QLabel(tr("Delay:"));
    genLabel = new QLabel(tr("Generation:"));
    genNumLabel = new QLabel(tr("0"));

    quitButton = new QPushButton(tr("Quit"));
    playButton = new QPushButton(tr("Play"));
    stepButton = new QPushButton(tr("Step"));

    zoomBox = new QSpinBox;
    delayBox = new QSpinBox;

    zoomSlide = new QSlider(Qt::Horizontal);
    delaySlide = new QSlider(Qt::Horizontal);
    
    zoomBox->setRange(1, 30);
    delayBox->setRange(1, 1000);
    zoomSlide->setRange(1, 30);
    delaySlide->setRange(1, 1000);
    
    zoomBox->setValue(8);
    delayBox->setValue(100);
    zoomSlide->setValue(8);
    delaySlide->setValue(100);
    
    connect(this, SIGNAL(changeState(bool)), disp, SLOT(changeState(bool)));
    connect(disp, SIGNAL(updateGen(QString &)), this, SLOT(changeGen(QString &)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(quitting()), scrollArea, SLOT(close()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    connect(stepButton, SIGNAL(clicked()), disp, SLOT(step()));
    connect(zoomBox, SIGNAL(valueChanged(int)), disp, SLOT(zoomChanged(int)));
    connect(delayBox, SIGNAL(valueChanged(int)), disp, SLOT(delayChanged(int)));

    connect(zoomBox, SIGNAL(valueChanged(int)), zoomSlide, SLOT(setValue(int)));
    connect(zoomSlide, SIGNAL(valueChanged(int)), zoomBox, SLOT(setValue(int)));

    connect(delayBox, SIGNAL(valueChanged(int)), delaySlide, SLOT(setValue(int)));
    connect(delaySlide, SIGNAL(valueChanged(int)), delayBox, SLOT(setValue(int)));

    QHBoxLayout* top = new QHBoxLayout;
    top->addWidget(zoomLabel);
    top->addWidget(zoomBox);
    top->addWidget(zoomSlide);

    QHBoxLayout* midTop = new QHBoxLayout;
    midTop->addWidget(delayLabel);
    midTop->addWidget(delayBox);
    midTop->addWidget(delaySlide);

    QHBoxLayout* midBottom = new QHBoxLayout;
    midBottom->addWidget(genLabel);
    midBottom->addStretch();
    midBottom->addWidget(genNumLabel);

    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(quitButton);
    bottom->addWidget(playButton);
    bottom->addWidget(stepButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(top);
    mainLayout->addLayout(midTop);
    mainLayout->addLayout(midBottom);
    mainLayout->addLayout(bottom);
    setLayout(mainLayout);

    setWindowTitle(tr("Controls"));
    setFixedHeight(sizeHint().height());

    scrollArea->show();
}

void OptionsDialog::changeGen(QString &text)
{
    genNumLabel->setText(text);
}

void OptionsDialog::playClicked()
{
    bool playState = stepButton->isEnabled();
    stepButton->setEnabled(!playState);
    if(playState)
    {
        playButton->setText("Pause");
    }
    else
    {
        playButton->setText("Play");
    }
    emit changeState(playState);
}

void OptionsDialog::closeEvent(QCloseEvent* e)
{
    QDialog::closeEvent(e);
    emit quitting();
}
