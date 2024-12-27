#ifndef TEST_STUDENT_H
#define TEST_STUDENT_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <mysql/mysql.h>  // Подключаем MySQL

class TestStudent : public QWidget {
Q_OBJECT

public:
    explicit TestStudent(QWidget *parent = nullptr);
    ~TestStudent();

private slots:
    void startTest();
    void submitTest();

private:
    // Элементы интерфейса
    QRadioButton *answerRadioButton1;
    QRadioButton *answerRadioButton2;
    QRadioButton *answerRadioButton3;
    QRadioButton *answerRadioButton4;
    QPushButton *startTestButton;
    QPushButton *submitTestButton;
    QLabel *questionLabel;

    int currentQuestionIndex;
    int score;
    int correctAnswers;

    void showQuestion();

    MYSQL *dbConn;
    void saveResultsToDatabase();
};

#endif // TEST_STUDENT_H
