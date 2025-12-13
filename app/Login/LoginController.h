#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QWidget>
#include <QStackedWidget>
#include "../Services/AuthService.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QComboBox;
class QLabel;
QT_END_NAMESPACE

class LoginController : public QWidget
{
    Q_OBJECT

public:
    explicit LoginController(QWidget *parent = nullptr);

signals:
    void loginSuccess(int userId, UserRole role, const QString& name);

private slots:
    void onLoginClicked();
    void onRoleChanged(int index);

private:
    void setupUI();
    
    QComboBox* m_roleCombo;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QLabel* m_statusLabel;
};

#endif // LOGINCONTROLLER_H

