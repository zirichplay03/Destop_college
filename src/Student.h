#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QMessageBox>
#include <mysql/mysql.h>

class Student : public QWidget {
Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    ~Student();

private slots:
    void onStudentClicked();

private:
    QLineEdit *StudentLineEdit;
    QPushButton *StudentButton;
    QLabel *statusLabel;
    QLabel *imageLabel;
    QString queryStr;

    MYSQL *dbConn;  // Добавляем dbConn как член класса
};
