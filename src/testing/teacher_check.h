#ifndef TEACHER_CHECK_H
#define TEACHER_CHECK_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <mysql/mysql.h>

class TeacherCheck : public QWidget
{
Q_OBJECT

public:
    explicit TeacherCheck(QWidget *parent = nullptr);
    ~TeacherCheck();

private slots:
    void refreshResults();

private:
    void setupUI();
    void loadDataFromDatabase();

    QTableWidget *resultsTable;  // Таблица для отображения результатов студентов
    QPushButton *refreshButton;  // Кнопка для обновления результатов
    MYSQL *dbConn;              // Соединение с базой данных
};

#endif // TEACHER_CHECK_H
