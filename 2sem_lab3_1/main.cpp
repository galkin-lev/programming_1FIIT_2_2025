// // #include <iostream>
// // #include <QApplication>
// // #include <QPushButton>
// // #include <QMessageBox>
// // #include <QMainWindow>
// // #include <QVBoxLayout>
// // #include <QLineEdit>
// //
// //
// // #include <QApplication>
// // #include <QPushButton>
// // #include "simple_logger.h"
// //
// // int main(int argc, char *argv[]) {
// //
// //     logging::init();
// //
// //     // QApplication a(argc, argv);
// //     // QPushButton button("Hello world!", nullptr);
// //     // button.resize(200, 100);
// //     // button.show();
// //     // return QApplication::exec();
// //
// //
// //     QApplication app(argc, argv);
// //
// //     QMainWindow *window = new QMainWindow();
// //     window->setGeometry(QRect(0, 0, 800, 600));
// //     QPushButton *pushButton = new QPushButton(window);
// //
// //     QApplication a(argc, argv);
// //     QPushButton button("btn", nullptr);
// //     button.resize(200, 100);
// //
// //
// //     window->setCentralWidget(pushButton);
// //
// //
// //     // QLineEdit *textField = new QLineEdit(window);
// //     // textField->setPlaceholderText(QString::number(i));
// //
// //
// //
// //
// //     QVBoxLayout *layout = new QVBoxLayout(window);
// //     // layout->addWidget(textField);
// //     window->setLayout(layout);
// //
// //     // window->setCentralWidget(textField);
// //
// //     button.show();
// //     // QObject::connect(&button, &QPushButton::clicked, [=]() {
// //     //     onButtonClicked(textField);
// //     // });    button.show();
// //
// //     QObject::connect(&button, &QPushButton::clicked, [&]() {
// //         logging::info("you pressed the button");
// //         // button.setText(QString::number(i));
// //         // textField->setPlaceholderText(QString::number(i));
// //     });
// //
// //     //This captures textField and calls the function when the button is clicked
// //
// //     return app.exec();
// // }
//
//
// #include <QApplication>
// #include <QMainWindow>
// #include <QPushButton>
// #include <QVBoxLayout>
// #include <QWidget>
// #include <QLabel>
// #include <QMessageBox>
// #include <QLineEdit>
//
// #include "main.moc"
//
// // Ваш логгер будет добавлен позже
// // #include "simple_logger.h"
//
// class MainWindow : public QMainWindow {
//     Q_OBJECT
//
// private:
//     QPushButton* openWindowBtn;
//     QPushButton* closeWindowBtn;
//     QLineEdit* textField;
//     QLabel* windowCountLabel;
//     int windowCounter = 0;
//     QWidget* currentExtraWindow = nullptr;
//
// public:
//     MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
//         setupUI();
//         connectSignals();
//
//         // logging::info("Main window created"); // Будет добавлено позже
//     }
//
// private:
//     void setupUI() {
//         QWidget* centralWidget = new QWidget(this);
//         QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
//
//         // Заголовок
//         QLabel* titleLabel = new QLabel("Simple Window Manager", this);
//         titleLabel->setAlignment(Qt::AlignCenter);
//         QFont titleFont = titleLabel->font();
//         titleFont.setPointSize(16);
//         titleFont.setBold(true);
//         titleLabel->setFont(titleFont);
//
//         // Информационная метка
//         windowCountLabel = new QLabel("No extra windows open", this);
//         windowCountLabel->setAlignment(Qt::AlignCenter);
//
//         // Текстовое поле
//         textField = new QLineEdit(this);
//         textField->setPlaceholderText("Enter some text here...");
//         textField->setMaximumWidth(300);
//
//         // Кнопки
//         openWindowBtn = new QPushButton("Open New Window", this);
//         closeWindowBtn = new QPushButton("Close Extra Window", this);
//         QPushButton* showMessageBtn = new QPushButton("Show Message", this);
//         QPushButton* clearTextBtn = new QPushButton("Clear Text", this);
//
//         // Стилизация кнопок
//         openWindowBtn->setStyleSheet(
//             "QPushButton {"
//             "   background-color: #4CAF50;"
//             "   color: white;"
//             "   padding: 8px;"
//             "   border-radius: 4px;"
//             "}"
//             "QPushButton:hover {"
//             "   background-color: #45a049;"
//             "}"
//         );
//
//         closeWindowBtn->setStyleSheet(
//             "QPushButton {"
//             "   background-color: #f44336;"
//             "   color: white;"
//             "   padding: 8px;"
//             "   border-radius: 4px;"
//             "}"
//             "QPushButton:hover {"
//             "   background-color: #d32f2f;"
//             "}"
//         );
//
//         showMessageBtn->setStyleSheet(
//             "QPushButton {"
//             "   background-color: #2196F3;"
//             "   color: white;"
//             "   padding: 8px;"
//             "   border-radius: 4px;"
//             "}"
//         );
//
//         clearTextBtn->setStyleSheet(
//             "QPushButton {"
//             "   background-color: #FF9800;"
//             "   color: white;"
//             "   padding: 8px;"
//             "   border-radius: 4px;"
//             "}"
//         );
//
//         // Добавляем виджеты в layout
//         mainLayout->addWidget(titleLabel);
//         mainLayout->addSpacing(20);
//         mainLayout->addWidget(windowCountLabel);
//         mainLayout->addSpacing(20);
//         mainLayout->addWidget(textField, 0, Qt::AlignCenter);
//         mainLayout->addSpacing(10);
//         mainLayout->addWidget(openWindowBtn);
//         mainLayout->addWidget(closeWindowBtn);
//         mainLayout->addWidget(showMessageBtn);
//         mainLayout->addWidget(clearTextBtn);
//         mainLayout->addStretch();
//
//         // Начальное состояние кнопки закрытия
//         closeWindowBtn->setEnabled(false);
//
//         setCentralWidget(centralWidget);
//         setWindowTitle("Simple GUI Application");
//         resize(400, 400);
//     }
//
//     void connectSignals() {
//         // Открытие нового окна
//         connect(openWindowBtn, &QPushButton::clicked, this, &MainWindow::openNewWindow);
//
//         // Закрытие окна
//         connect(closeWindowBtn, &QPushButton::clicked, this, &MainWindow::closeExtraWindow);
//
//         // Показать сообщение
//         connect(openWindowBtn, &QPushButton::clicked, [this]() {
//             // logging::info("Open Window button clicked"); // Будет добавлено позже
//         });
//
//         connect(closeWindowBtn, &QPushButton::clicked, [this]() {
//             // logging::warn("Close Window button clicked"); // Будет добавлено позже
//         });
//
//         // Кнопка Show Message
//         connect(openWindowBtn, &QPushButton::clicked, this, [this]() {
//             QString text = textField->text().isEmpty() ?
//                           "No text entered" : textField->text();
//             QMessageBox::information(this, "Message",
//                 "You entered: " + text + "\n\n" +
//                 "Total windows opened: " + QString::number(windowCounter));
//
//             // logging::debug("Message shown: " + text.toStdString()); // Будет добавлено позже
//         });
//
//         // Кнопка Clear Text
//         connect(openWindowBtn, &QPushButton::clicked, this, [this]() {
//             textField->clear();
//             textField->setFocus();
//
//             // logging::trace("Text field cleared"); // Будет добавлено позже
//         });
//     }
//
//     void openNewWindow() {
//         windowCounter++;
//
//         // Закрываем предыдущее окно, если оно есть
//         if (currentExtraWindow) {
//             currentExtraWindow->close();
//             currentExtraWindow->deleteLater();
//         }
//
//         // Создаем новое окно
//         currentExtraWindow = new QWidget(this, Qt::Window);
//         currentExtraWindow->setWindowTitle("Extra Window #" + QString::number(windowCounter));
//         currentExtraWindow->resize(300, 200);
//
//         QVBoxLayout* layout = new QVBoxLayout(currentExtraWindow);
//
//         QLabel* label = new QLabel(
//             "This is extra window #" + QString::number(windowCounter) +
//             "\n\nIt will close when you:\n"
//             "1. Click 'Close Extra Window'\n"
//             "2. Or close this window manually",
//             currentExtraWindow
//         );
//         label->setAlignment(Qt::AlignCenter);
//         label->setWordWrap(true);
//
//         QPushButton* closeBtn = new QPushButton("Close Me", currentExtraWindow);
//         QPushButton* errorBtn = new QPushButton("Simulate Error", currentExtraWindow);
//
//         layout->addWidget(label);
//         layout->addWidget(closeBtn);
//         layout->addWidget(errorBtn);
//
//         currentExtraWindow->setLayout(layout);
//         currentExtraWindow->show();
//
//         // Обновляем статус
//         windowCountLabel->setText("Extra window #" + QString::number(windowCounter) + " is open");
//         closeWindowBtn->setEnabled(true);
//
//         // Подключаем кнопку закрытия в новом окне
//         connect(closeBtn, &QPushButton::clicked, [this]() {
//             // logging::info("Close button clicked in extra window"); // Будет добавлено позже
//             closeExtraWindow();
//         });
//
//         connect(errorBtn, &QPushButton::clicked, [this]() {
//             // logging::error("Simulated error from extra window"); // Будет добавлено позже
//
//             QMessageBox::critical(currentExtraWindow, "Error",
//                 "Simulated error!\n\n"
//                 "This is where you would add error logging.\n"
//                 "Check the console for error messages.");
//         });
//
//         // Следим за закрытием окна
//         connect(currentExtraWindow, &QWidget::destroyed, this, [this]() {
//             if (!currentExtraWindow) {
//                 windowCountLabel->setText("Window was closed manually");
//                 closeWindowBtn->setEnabled(false);
//                 currentExtraWindow = nullptr;
//
//                 // logging::warn("Extra window closed manually"); // Будет добавлено позже
//             }
//         });
//
//         // logging::info("New window opened: #" + std::to_string(windowCounter)); // Будет добавлено позже
//     }
//
//     void closeExtraWindow() {
//         if (currentExtraWindow) {
//             // logging::debug("Closing extra window"); // Будет добавлено позже
//
//             currentExtraWindow->close();
//             currentExtraWindow->deleteLater();
//             currentExtraWindow = nullptr;
//
//             windowCountLabel->setText("No extra windows open");
//             closeWindowBtn->setEnabled(false);
//
//             // logging::info("Extra window closed successfully"); // Будет добавлено позже
//         } else {
//             // logging::warn("Attempted to close non-existent window"); // Будет добавлено позже
//             QMessageBox::warning(this, "Warning", "No extra window is open!");
//         }
//     }
// };
//
// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//
//     // Инициализация логгера будет добавлена позже
//     // logging::init();
//     // logging::info("Application started");
//
//     MainWindow mainWindow;
//     mainWindow.show();
//
//     // logging::debug("Main window displayed");
//
//     return app.exec();
// }
//
// // Для MOC необходимо добавить


#include <QApplication>

#include "logger/simple_logger.h"
#include "view/MainWindow.h"
// Ваш логгер будет добавлен позже
// #include "simple_logger.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    logging::init();

    MainWindow mainWindow;
    mainWindow.show();

    // logging::debug("Main window displayed");

    return app.exec();
}