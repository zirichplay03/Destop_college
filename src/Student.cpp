#include "Student.h"
#include "testing/test_student.h"

Student::Student(QWidget *parent)
        : QWidget(parent), dbConn(nullptr) {
    setWindowTitle("Login Window");
    resize(728, 728);

    StudentLineEdit = new QLineEdit(this);
    StudentLineEdit->setPlaceholderText("Номер студенческого");

    StudentButton = new QPushButton("Подтвердить", this);
    statusLabel = new QLabel(this);

    // Инициализация imageLabel для отображения картинки
    imageLabel = new QLabel(this);

    // Загрузка изображения
    QPixmap pixmap(":/picture/bart.jpg");
    if (pixmap.isNull()) {
        qDebug() << "Ошибка загрузки изображения!";
    } else {
        imageLabel->setPixmap(pixmap);
        imageLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        imageLabel->adjustSize();  // Подгоняем размер QLabel по изображению
    }
    // Расположение элементов на форме
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(imageLabel);
    layout->addWidget(StudentLineEdit);
    layout->addWidget(StudentButton);
    layout->addWidget(statusLabel);

    // Связываем сигнал кнопки с нашим слотом
    connect(StudentButton, &QPushButton::clicked, this, &Student::onStudentClicked);
}

Student::~Student() {
    // Закрываем соединение с БД
    if (dbConn != nullptr) {
        mysql_close(dbConn);
    }
}

void Student::onStudentClicked() {

    QString id_student = StudentLineEdit->text();

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
    if (id_student.isEmpty()){
        QMessageBox::warning(this, "Error", "Пусто");
        mysql_close(conn);
        return;
    }

    queryStr = QString("SELECT * FROM students WHERE ticket_number = '%1'").arg(id_student);

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
        QMessageBox::information(this, "Login", "Успешно");
        TestStudent *testStudent= new TestStudent();  // Создаем окно студента
        testStudent->show();
        this->close();  // Закрываем LoginWindow после успешного входа
    } else {
        // Если вход неудачный
        QMessageBox::warning(this, "Login", "Login failed");
    }

    // Освобождаем память и закрываем соединение
    mysql_free_result(result);
    mysql_close(conn);

}
