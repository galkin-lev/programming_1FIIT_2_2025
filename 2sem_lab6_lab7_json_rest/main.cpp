#include <QApplication>
#include <QPushButton>


#include <QApplication>

#include "logger/logger.h"
#include "view/MainWindow.h"
#include "../../../features/auth/login/loginScreen.h"
// Ваш логгер будет добавлен позже
// #include "simple_logger.h"
#include "core/data/local_db/db.h"

int main(int argc, char *argv[]) {

    // initDatabase();

    QApplication app(argc, argv);

    // 2. Then initialize database
    if (!initDatabase()) {
        return -1;
    }

    logging::init();

    loginScreen mainWindow;
    mainWindow.show();

    // logging::debug("Main window displayed");

    return app.exec();
}