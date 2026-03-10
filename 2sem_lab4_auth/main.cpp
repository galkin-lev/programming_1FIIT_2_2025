#include <QApplication>
#include <QPushButton>


#include <QApplication>

#include "logger/logger.h"
#include "view/MainWindow.h"
#include "authorization/ui/view/login/loginScreen.h"
// Ваш логгер будет добавлен позже
// #include "simple_logger.h"
#include "authorization/data/local_db/db.h"

int main(int argc, char *argv[]) {

    initDatabase();

    QApplication app(argc, argv);

    logging::init();

    loginScreen mainWindow;
    mainWindow.show();

    // logging::debug("Main window displayed");

    return app.exec();
}