#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QMessageBox>
#include <mysql/mysql.h>

class Teacher : public QWidget {
Q_OBJECT

public:
    explicit Teacher(QWidget *parent = nullptr);
    ~Teacher();

private slots:
    void onTeacherClicked();


private:
    QLineEdit *TeacherLineEdit;
    QPushButton *TeacherButton;
    QLabel *statusLabel;
    QLabel *imageLabel;
    QString queryStr;

    MYSQL *dbConn;  // Добавляем dbConn как член класса
};
