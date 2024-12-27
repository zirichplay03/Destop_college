#include "test_student.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <mysql/mysql.h>  // Подключаем mysql.h

TestStudent::TestStudent(QWidget *parent)
        : QWidget(parent), currentQuestionIndex(0), score(0), correctAnswers(0), dbConn(nullptr) {

    // Инициализация интерфейса
    questionLabel = new QLabel(this);
    answerRadioButton1 = new QRadioButton(this);
    answerRadioButton2 = new QRadioButton(this);
    answerRadioButton3 = new QRadioButton(this);
    answerRadioButton4 = new QRadioButton(this);

    startTestButton = new QPushButton("Начать тест", this);
    submitTestButton = new QPushButton("Ответить", this);  // Переименована кнопка

    submitTestButton->setEnabled(false);  // Кнопка "Ответить" изначально отключена

    // Расположение элементов на форме
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(questionLabel);
    layout->addWidget(answerRadioButton1);
    layout->addWidget(answerRadioButton2);
    layout->addWidget(answerRadioButton3);
    layout->addWidget(answerRadioButton4);
    layout->addWidget(startTestButton);
    layout->addWidget(submitTestButton);

    // Соединение сигналов и слотов
    connect(startTestButton, &QPushButton::clicked, this, &TestStudent::startTest);
    connect(submitTestButton, &QPushButton::clicked, this, &TestStudent::submitTest);
}

TestStudent::~TestStudent() {
    // Закрытие соединения с БД
    if (dbConn != nullptr) {
        mysql_close(dbConn);
    }
}

void TestStudent::startTest() {
    // Прячем кнопку "Начать тест" и показываем "Ответить"
    startTestButton->setVisible(false);
    submitTestButton->setEnabled(true);  // Включаем кнопку "Ответить"
    currentQuestionIndex = 0;  // Начинаем с первого вопроса
    score = 0;  // Обнуляем счет
    correctAnswers = 0;  // Обнуляем количество правильных ответов
    showQuestion();
}

void TestStudent::showQuestion() {
    // Массив вопросов и ответов
    struct Question {
        QString question;
        QString answer1;
        QString answer2;
        QString answer3;
        QString answer4;
        int correctAnswerIndex;  // Индекс правильного ответа (1, 2, 3 или 4)
    };

    Question questions[] = {
            {"Сколько будет 2+2?", "3", "4", "5", "6", 2},
            {"Сколько будет 3+3?", "5", "6", "7", "8", 2},
            {"Сколько будет 4+4?", "7", "8", "9", "10", 2},
            {"Сколько будет 5+5?", "9", "10", "11", "12", 2},
            {"Сколько будет 6+6?", "11", "12", "13", "14", 2}
    };

    // Показываем текущий вопрос и варианты ответов
    questionLabel->setText(questions[currentQuestionIndex].question);
    answerRadioButton1->setText(questions[currentQuestionIndex].answer1);
    answerRadioButton2->setText(questions[currentQuestionIndex].answer2);
    answerRadioButton3->setText(questions[currentQuestionIndex].answer3);
    answerRadioButton4->setText(questions[currentQuestionIndex].answer4);

    // Сбрасываем выборы ответов
    answerRadioButton1->setChecked(false);
    answerRadioButton2->setChecked(false);
    answerRadioButton3->setChecked(false);
    answerRadioButton4->setChecked(false);
}

void TestStudent::submitTest() {
    // Массив вопросов и правильных ответов
    struct Question {
        int correctAnswerIndex;
    };

    Question questions[] = {
            {2}, {2}, {2}, {2}, {2}  // Все правильные ответы — 2-й вариант
    };

    // Подсчитываем количество правильных ответов
    if (answerRadioButton1->isChecked() && questions[currentQuestionIndex].correctAnswerIndex == 1) {
        correctAnswers++;
    } else if (answerRadioButton2->isChecked() && questions[currentQuestionIndex].correctAnswerIndex == 2) {
        correctAnswers++;
    } else if (answerRadioButton3->isChecked() && questions[currentQuestionIndex].correctAnswerIndex == 3) {
        correctAnswers++;
    } else if (answerRadioButton4->isChecked() && questions[currentQuestionIndex].correctAnswerIndex == 4) {
        correctAnswers++;
    }

    // Переходим к следующему вопросу или завершаем тест
    currentQuestionIndex++;

    if (currentQuestionIndex < 5) {
        showQuestion();  // Переходим к следующему вопросу
    } else {
        // Завершаем тест и сохраняем результаты в БД
        saveResultsToDatabase();
    }
}

void TestStudent::saveResultsToDatabase() {
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


    QString updateQuery = QString("UPDATE students SET correct_answers = %1 WHERE ticket_number = 222")
            .arg(correctAnswers);



    const char* query = qPrintable(updateQuery);

    if (mysql_query(dbConn, query)) {
        QMessageBox::warning(this, "Ошибка", "Ошибка выполнения запроса: " + QString(mysql_error(dbConn)));
        mysql_close(dbConn);
        return;
    }

    // Информируем пользователя
    QMessageBox::information(this, "Результаты", QString("Тест завершен. Ваши правильные ответы: %1 из 5").arg(correctAnswers));

    mysql_close(dbConn);  // Закрываем соединение с БД
    this->close();  // Закрыть окно теста
}

