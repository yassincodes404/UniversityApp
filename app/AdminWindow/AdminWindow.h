#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(int adminId, QWidget *parent = nullptr);
    ~AdminWindow();

signals:
    void logoutRequested();

private slots:
    void onLogout();
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onAddCourse();
    void onEditCourse();
    void onDeleteCourse();
    void refreshStudents();
    void refreshCourses();

private:
    void setupUI();
    void setupMenuBar();
    
    int m_adminId;
    QTabWidget* m_tabs;
    QTableView* m_studentsTable;
    QTableView* m_coursesTable;
    QTableView* m_profileImageRequestsTable;
    QPushButton* m_logoutButton;
    QLineEdit* m_studentSearchEdit;
    QLineEdit* m_courseSearchEdit;

private slots:
    void onApproveProfileImage();
    void onRejectProfileImage();
    void refreshProfileImageRequests();
};

#endif // ADMINWINDOW_H

