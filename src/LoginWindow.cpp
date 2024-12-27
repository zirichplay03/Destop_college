#include "LoginWindow.h"
#include <QPixmap>
#include <QMessageBox>
#include <QRadioButton>
#include <QVBoxLayout>
#include "Student.h"
#include "Teacher.h"

LoginWindow::LoginWindow(QWidget *parent)
        : QWidget(parent), dbConn(nullptr) {
    setWindowTitle("Login Window");
    resize(728, 728);

    // Инициализация элементов
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Login");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Password");

    loginButton = new QPushButton("Login", this);
    statusLabel = new QLabel(this);

    // Инициализация imageLabel для отображения картинки
    imageLabel = new QLabel(this);

    // Загрузка изображения
    QPixmap pixmap(":/picture/user.png");
    if (pixmap.isNull()) {
        qDebug() << "Ошибка загрузки изображения!";
    } else {
        imageLabel->setPixmap(pixmap);
        imageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        imageLabel->adjustSize();  // Подгоняем размер QLabel по изображению
    }

    // Инициализация радиокнопок для выбора типа пользователя
    studentRadioButton = new QRadioButton("Студент", this);
    teacherRadioButton = new QRadioButton("Преподаватель", this);
    studentRadioButton->setChecked(true);  // По умолчанию выбран студент

    // Расположение элементов на форме
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(imageLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(studentRadioButton);
    layout->addWidget(teacherRadioButton);
    layout->addWidget(loginButton);
    layout->addWidget(statusLabel);

    // Связываем сигнал кнопки с нашим слотом
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

LoginWindow::~LoginWindow() {
    // Закрываем соединение с БД
    if (dbConn != nullptr) {
        mysql_close(dbConn);
    }
}

void LoginWindow::onLoginClicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    // Подключаемся к базе данных
    MYSQL *conn;
    conn = mysql_init(NULL);  // Инициализация структуры подключения

    if (conn == NULL) {
        QMessageBox::warning(this, "Ошибка", "MySQL initialization failed!");
        return;
    }

    if (mysql_real_connect(conn, "localhost", "root", "....", "school_database", 0, NULL, 0) == NULL) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к базе данных MySQL!");
        mysql_close(conn);
        return;
    }

    // Проверяем, что логин и пароль не пустые
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Пусто");
        mysql_close(conn);
        return;
    }

    // Проверка, какой тип пользователя выбран
    QString userType;
    if (studentRadioButton->isChecked()) {
        userType = "Студент";
    } else if (teacherRadioButton->isChecked()) {
        userType = "Преподаватель";
    } else {
        QMessageBox::warning(this, "Error", "Please select a user type.");
        mysql_close(conn);
        return;
    }

    // Формируем запрос
    QString queryStr;
    if (userType == "Студент") {
        queryStr = QString("SELECT * FROM students WHERE login = '%1' AND password = '%2'").arg(username).arg(password);
    } else if (userType == "Преподаватель") {
        queryStr = QString("SELECT * FROM teachers WHERE login = '%1' AND password = '%2'").arg(username).arg(password);
    }

    // Преобразуем queryStr в обычную строку C
    const char* query = qPrintable(queryStr);

    // Выполняем запрос
    if (mysql_query(conn, query)) {
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса: " + QString(mysql_error(conn)));
        mysql_close(conn);
        return;
    }

    // Получаем результаты
    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить результаты запроса: " + QString(mysql_error(conn)));
        mysql_close(conn);
        return;
    }

    // Проверяем, есть ли строки в результате
    if (mysql_num_rows(result) > 0) {
        // Если вход успешный
        QMessageBox::information(this, "Login", "Вход выполнен");
        if (userType == "Студент") {
            Student *studentWindow = new Student();  // Создаем окно студента
            studentWindow->show();                  // Показываем окно студента
        } else if (userType == "Преподаватель") {
            Teacher *teacherWindow = new Teacher(); // Создаем окно преподавателя
            teacherWindow->show();                  // Показываем окно преподавателя
        }
        this->close();
    } else {
        // Если вход неудачный
        QMessageBox::warning(this, "Login", "Что-то не так");
    }

    // Освобождаем память и закрываем соединение
    mysql_free_result(result);
    mysql_close(conn);
}
