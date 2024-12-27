
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QRadioButton>
#include <mysql/mysql.h>

class LoginWindow : public QWidget {
Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QLabel *statusLabel;
    QLabel *imageLabel;
    // Declare radio buttons
    QRadioButton *studentRadioButton;
    QRadioButton *teacherRadioButton;

    MYSQL *dbConn;  // Добавляем dbConn как член класса
};
