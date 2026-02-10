#include <iostream>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include "Counter.h"

using namespace std;

int main(int argc, char *argv[])
{

    // QApplication app(argc, argv);
    //
    // QPixmap pixmap;
    //
    // pixmap.load(":/images/image_placeholder.png");
    //
    //
    // QLabel label;
    // if (pixmap.isNull()) {
    //     label.setText("IMAGE IS NULL!");
    // } else {
    //     label.setPixmap(pixmap);
    // }
    //
    // label.show();
    // return app.exec();


    // QApplication app(argc, argv);
    //
    // QGraphicsScene scene;
    // QGraphicsView view(&scene);
    //
    //
    //
    //
    // QPixmap pixmap(":/images/image_placeholder.png");  // From embedded resources
    //
    // scene.addPixmap(pixmap);
    //
    // view.show();
    // return app.exec();


    // QLabel label("hello world");
    // label.setWindowTitle("lab1");
    // label.resize(400, 300);
    //
    // // label.setPixmap(pixmap);

    //
    //
    //
    // QPushButton button("button", &label);
    // button.setText("button.setText");
    // button.move(50, 50);
    // button.resize(100, 70);
    // button.show();
    //
    // QLabel image(&label);
    // image.setPixmap(pixmap.scaled(250, 250));
    // image.move(150, 150);
    // image.resize(250, 250);
    // image.show();

    // view.show();
    // return app.exec();

    cout << "_____ Creating objects _____" << endl;

    // Create first object using default constructor
    Counter counter1;

    // Create second object using parameterized constructor
    Counter counter2(5);

    cout << "\n_____ Initial values _____" << endl;
    cout << "counter1 value: " << counter1.getValue() << endl;
    cout << "counter2 value: " << counter2.getValue() << endl;

    cout << "\n_____ Operations on counter1 _____" << endl;
    counter1.increment();
    counter1.increment();
    counter1.increment();
    cout << "After 3 increments: " << counter1.getValue() << endl;
    counter1.decrement();
    cout << "After decrement: " << counter1.getValue() << endl;

    cout << "\n_____ Operations on counter2 _____" << endl;

    counter2.decrement();
    counter2.decrement();
    cout << "After 2 decrements: " << counter2.getValue() << endl;

    counter2.reset();
    cout << "After reset: " << counter2.getValue() << endl;

    cout << "\n_____ Create counter3 with negative value _____" << endl;
    Counter counter3(-10);
    cout << "counter3 value: " << counter3.getValue() << endl;

    cout << "\n_____ Exiting main() _____" << endl;

    return 0;

}