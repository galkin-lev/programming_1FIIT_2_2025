// #include <QApplication>
// #include <QPushButton>
//
// int main(int argc, char *argv[]) {
//     QApplication a(argc, argv);
//     QPushButton button("Hello world!", nullptr);
//     button.resize(200, 100);
//     button.show();
//     return QApplication::exec();
// }
//
// #include <QApplication>
// #include <QLabel>
// #include <QMainWindow>
// #include <QPushButton>
//
//
//
//
// class MainWindow : public QMainWindow {
//     Q_OBJECT
// public:
//     MainWindow() {
//         QPushButton *button = new QPushButton("Click Me!", this);
//         button->setGeometry(100, 80, 100, 40);
//
//         connect(button, &QPushButton::clicked, this, [this]() {
//             i++;
//             // Optionally update a label to show the new value
//             // label->setText("Count: " + QString::number(i));
//         });
//     }
// private:
//     int i = 0; // Member variable, initialized to 0
// };
// //
// // int main(int argc, char *argv[])
// // {
// //     // QApplication app(argc, argv);
// //     //
// //     // QLabel label("Привет, мир!");
// //     // label.setWindowTitle("Моё первое окно");
// //     // label.resize(200, 100);
// //     // label.show();
// //
// //
// //     QApplication app(argc, argv);
// //
// //     QMainWindow window;
// //     window.setWindowTitle("Simple Qt App");
// //     window.resize(300, 200);
// //
// //     QPushButton *button = new QPushButton("Click Me!", &window);
// //     button->setGeometry(100, 80, 100, 40);
// //
// //
// //     window.show();
// //     return app.exec();
// //
// //
// //     return app.exec();
// // }
//
// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);
//     MainWindow window;
//     window.show();
//     return app.exec();
// }
//
//
//
#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>


int i = 0;

 // void onButtonClicked(QLineEdit* textField) {
 //        QMessageBox::information(nullptr, "Info", "Button clicked!");
 //        i += 2;
 //        std::cout << i << std::endl;
 //     // textField->setPlaceholderText(QString::number(i));
 //     textField->repaint(); // Force UI update
 //
 //
 //    }





int main(int argc, char *argv[]) {


    QApplication app(argc, argv);

    QMainWindow *window = new QMainWindow();
    window->setGeometry(QRect(0, 0, 800, 600));
    QPushButton *pushButton = new QPushButton(window);

    QApplication a(argc, argv);
    QPushButton button(QString::number(i), nullptr);
    button.resize(200, 100);


    window->setCentralWidget(pushButton);


    // QLineEdit *textField = new QLineEdit(window);
    // textField->setPlaceholderText(QString::number(i));




    QVBoxLayout *layout = new QVBoxLayout(window);
    // layout->addWidget(textField);
    window->setLayout(layout);

     // window->setCentralWidget(textField);

    button.show();
     // QObject::connect(&button, &QPushButton::clicked, [=]() {
     //     onButtonClicked(textField);
     // });    button.show();

     QObject::connect(&button, &QPushButton::clicked, [&]() {
         i += 2;
         button.setText(QString::number(i));
         // textField->setPlaceholderText(QString::number(i));
     });

     //This captures textField and calls the function when the button is clicked

    return app.exec();
};

