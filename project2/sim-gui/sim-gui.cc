#include <QApplication>
#include <QScrollArea>

#include "widgets/iconeditor.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    IconEditor *iconEditor = new IconEditor;
    
    QScrollArea scrollArea;
    scrollArea.setWidget(iconEditor);
    scrollArea.viewport()->setBackgroundRole(QPalette::Dark);
    scrollArea.viewport()->setAutoFillBackground(true);
    scrollArea.setWindowTitle(QObject::tr("Icon Editor"));

    scrollArea.show();
    return app.exec();
}
