#include <QtGui>

#include "optionsdialog.h"
#include "boarddisp.h"

OptionsDialog::OptionsDialog(AutFile* a, QWidget* parent) : QDialog(parent)
{
    // Create a new board display
    BoardDisp* disp = new BoardDisp(a->get_x_high() - a->get_x_low() + 1, a->get_y_high() - a->get_y_low() + 1, a->get());
    
    // Set board display in scrollArea
    scrollArea = new QScrollArea;
    scrollArea->setWidget(disp);
    scrollArea->viewport()->setBackgroundRole(QPalette::Dark);
    scrollArea->viewport()->setAutoFillBackground(true);
    scrollArea->setWindowTitle(QString::fromStdString(a->get_name()));
    scrollArea->setWidgetResizable(true);
    
    // Setup labels
    zoomLabel = new QLabel(tr("Zoom factor:"));
    delayLabel = new QLabel(tr("Delay:"));
    genLabel = new QLabel(tr("Generation:"));
    genNumLabel = new QLabel(tr("0"));
    
    // Setup buttons
    quitButton = new QPushButton(tr("Quit"));
    playButton = new QPushButton(tr("Play"));
    stepButton = new QPushButton(tr("Step"));
    
    // Setup spin boxes
    zoomBox = new QSpinBox;
    delayBox = new QSpinBox;

    // Setup sliders
    zoomSlide = new QSlider(Qt::Horizontal);
    delaySlide = new QSlider(Qt::Horizontal);
    
    // Set ranges
    zoomBox->setRange(1, 30);
    delayBox->setRange(1, 1000);
    zoomSlide->setRange(1, 30);
    delaySlide->setRange(1, 1000);
    
    // Set default values.
    zoomBox->setValue(8);
    delayBox->setValue(100);
    zoomSlide->setValue(8);
    delaySlide->setValue(100);
    
    // Connect signals. This is the magic.
    connect(this, SIGNAL(changeState(bool)), disp, SLOT(changeState(bool)));
    connect(disp, SIGNAL(updateGen(QString &)), this, SLOT(changeGen(QString &)));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(this, SIGNAL(quitting()), scrollArea, SLOT(close()));
    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));
    connect(stepButton, SIGNAL(clicked()), disp, SLOT(step()));
    connect(zoomBox, SIGNAL(valueChanged(int)), disp, SLOT(zoomChanged(int)));
    connect(delayBox, SIGNAL(valueChanged(int)), disp, SLOT(delayChanged(int)));
    
    // Signals so zoomBox and zoomSlide are intertwined
    connect(zoomBox, SIGNAL(valueChanged(int)), zoomSlide, SLOT(setValue(int)));
    connect(zoomSlide, SIGNAL(valueChanged(int)), zoomBox, SLOT(setValue(int)));
    
    // Signals so delayBox and delaySlide are intertwined
    connect(delayBox, SIGNAL(valueChanged(int)), delaySlide, SLOT(setValue(int)));
    connect(delaySlide, SIGNAL(valueChanged(int)), delayBox, SLOT(setValue(int)));
    
    // Layout the window.
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
    // Necessary for right-aligning genNumLabel
    midBottom->addStretch();
    midBottom->addWidget(genNumLabel);
    
    QHBoxLayout* bottom = new QHBoxLayout;
    bottom->addWidget(quitButton);
    bottom->addWidget(playButton);
    bottom->addWidget(stepButton);
    
    // Set the layouts in the main layout.
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(top);
    mainLayout->addLayout(midTop);
    mainLayout->addLayout(midBottom);
    mainLayout->addLayout(bottom);
    setLayout(mainLayout);
    
    // Set window title and size hint
    setWindowTitle(QString::fromStdString(std::string("Controls: ") + a->get_name()));
    setFixedHeight(sizeHint().height());
    
    // Show scroll area.
    scrollArea->show();
}

void OptionsDialog::playClicked()
{
    // Change playButton text
    // and if stepButton is enabled
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
    // Close me and scrollArea.
    QDialog::closeEvent(e);
    emit quitting();
}
