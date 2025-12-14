#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QLabel;
class QPixmap;
QT_END_NAMESPACE

class StudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentWindow(int studentId, QWidget *parent = nullptr);
    ~StudentWindow();

signals:
    void logoutRequested();

private slots:
    void onLogout();
    void refreshData();

private:
    void setupUI();
    void setupMenuBar();
    void loadProfile();
    void loadCourses();
    void loadGrades();
    void loadPayments();
    void loadNews();
    void onChangeProfileImage();
    
    int m_studentId;
    QTabWidget* m_tabs;
    QPushButton* m_logoutButton;
    QLabel* m_profileLabel;
    QLabel* m_profileImageLabel;
    QPushButton* m_changeImageButton;
    QTableView* m_coursesTable;
    QTableView* m_gradesTable;
    QTableView* m_paymentsTable;
    QTableView* m_newsTable;
};

#endif // STUDENTWINDOW_H

