#include "teacher_check.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>

TeacherCheck::TeacherCheck(QWidget *parent)
        : QWidget(parent), dbConn(nullptr)
{
    setupUI();
    loadDataFromDatabase();
}

TeacherCheck::~TeacherCheck()
{
    if (dbConn != nullptr) {
        mysql_close(dbConn);  // Закрыть соединение с БД
    }
}

void TeacherCheck::setupUI()
{
    // Создаем таблицу для отображения результатов
    resultsTable = new QTableWidget(this);
    resultsTable->setColumnCount(3);  // 3 столбца: Имя, Номер билета, Результат
    resultsTable->setHorizontalHeaderLabels({"Имя студента", "Номер билета", "Результат"});
    resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Кнопка для обновления данных
    refreshButton = new QPushButton("Обновить", this);

    // Расположение элементов на форме
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(resultsTable);
    layout->addWidget(refreshButton);

    // Подключаем сигнал для обновления данных
    connect(refreshButton, &QPushButton::clicked, this, &TeacherCheck::refreshResults);
}

void TeacherCheck::loadDataFromDatabase()
{
    // Подключаемся к базе данных
    dbConn = mysql_init(nullptr);
    if (dbConn == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Не удалось инициализировать MySQL!");
        return;
    }

    if (mysql_real_connect(dbConn, "localhost", "root", "....", "school_database", 0, nullptr, 0) == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к базе данных MySQL!");
        mysql_close(dbConn);
        return;
    }

    // Выполняем запрос на получение данных о студентах
    const char *query = "SELECT login, ticket_number, correct_answers FROM students";
    if (mysql_query(dbConn, query)) {
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса: " + QString(mysql_error(dbConn)));
        mysql_close(dbConn);
        return;
    }

    MYSQL_RES *result = mysql_store_result(dbConn);
    if (result == nullptr) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить результаты!");
        mysql_close(dbConn);
        return;
    }

    int row = 0;
    MYSQL_ROW rowData;
    while ((rowData = mysql_fetch_row(result)) != nullptr) {
        // Добавляем строку в таблицу
        resultsTable->insertRow(row);
        resultsTable->setItem(row, 0, new QTableWidgetItem(rowData[0]));  // Имя студента
        resultsTable->setItem(row, 1, new QTableWidgetItem(rowData[1]));  // Номер билета
        resultsTable->setItem(row, 2, new QTableWidgetItem(rowData[2]));  // Результат теста (правильные ответы)
        row++;
    }

    mysql_free_result(result);  // Освобождаем память для результата запроса
}

void TeacherCheck::refreshResults()
{
    // Очищаем таблицу и загружаем данные заново
    resultsTable->setRowCount(0);
    loadDataFromDatabase();
}
