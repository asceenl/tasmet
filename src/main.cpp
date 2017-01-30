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

    // each of these signals calls the handler (quits the
    // QCoreApplication).
    for ( int sig : quitSignals )
        signal(sig, handler);
}


int main(int argc, char *argv[]) {

    // This can be used to store files in the application. Very useful
    // when our application grows bigger
    // Q_INIT_RESOURCE(application);
    INITTRACE(15);

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

    if(argc>1) {
        string filepath = argv[1];
        win.loadModel(&filepath);
    }
    
    return app.exec();
}
//////////////////////////////////////////////////////////////////////
