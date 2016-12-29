// main.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Main program to run
//////////////////////////////////////////////////////////////////////
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

// For using python from within Qt
#include <PythonQt.h>


#include "tasmet_config.h"
#include "tasmet_tracer.h"
#include "gui/mainwindow.h"

#include <signal.h>
#include <unistd.h>

void catchUnixSignals(const std::vector<int>& quitSignals,
                      const std::vector<int>& ignoreSignals = std::vector<int>()) {

    auto handler = [](int sig) ->void {
        printf("\nquit the application (user request signal = %d).\n", sig);
        QCoreApplication::quit();
    };

    // all these signals will be ignored.
    for ( int sig : ignoreSignals )
        signal(sig, SIG_IGN);

    // each of these signals calls the handler (quits the QCoreApplication).
    for ( int sig : quitSignals )
        signal(sig, handler);
}


int main(int argc, char *argv[]) {

    // This can be used to store files in the application. Very useful
    // when our application grows bigger
    // Q_INIT_RESOURCE(application);
    INITTRACE(15);

    std::cout << "hoid" << std::endl;
    // Initialize PythonQt
    // PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    PythonQt::init();
    std::cout << "hoid" << std::endl;
    PythonQt* pyqt = PythonQt::self();
    PythonQtObjectPtr context = pyqt->getMainModule();
    std::cout << "hoid" << std::endl;    
    QVariant rv = context.evalScript("from math import *\n");
    if(pyqt->hadError()) {
        return -1;
    }
    std::cout << "hoid" << std::endl;    
    // std::cout << rv.typeName() << std::endl;
    std::cout << (rv.isNull()?"true": "false") << std::endl;
    std::cout << "hoid" << std::endl;    
    
    QApplication app(argc, argv);
    
    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP});
    
    // QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName(appname);
    QCoreApplication::setApplicationVersion(appversion);
    // QCommandLineParser parser;
    // parser.setApplicationDescription(QCoreApplication::applicationName());
    // parser.addHelpOption();
    // parser.addVersionOption();
    // parser.addPositionalArgument("file", "The file to open.");
    // parser.process(app);

    // if (!parser.positionalArguments().isEmpty())
    //     mainWin.loadFile(parser.positionalArguments().first());



    TaSMETMainWindow win;

    win.show();
    
    return app.exec();
}
//////////////////////////////////////////////////////////////////////
