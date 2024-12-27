#include <QApplication>
#include "LoginWindow.h"
#include "Student.h"
#include "Teacher.h"
#include "testing/teacher_check.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Создание и отображение окна входа
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
