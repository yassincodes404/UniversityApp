#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include "../Models/User.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(const User& admin, QWidget* parent = nullptr);
    ~AdminWindow();

private slots:
    void onMenuSelectionChanged();
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onAddCourse();
    void onEditCourse();
    void onDeleteCourse();
    void onRefreshStudents();
    void onRefreshCourses();
    void onSearchStudents();
    void onSearchCourses();

private:
    void setupUI();
    void setupStudentsPage();
    void setupCoursesPage();
    void setupDashboardPage();
    void loadStudents();
    void loadCourses();
    
    User m_admin;
    QStackedWidget* m_stackedWidget;
    QListWidget* m_menuList;
    
    // Students page
    QTableView* m_studentsTable;
    QPushButton* m_addStudentBtn;
    QPushButton* m_editStudentBtn;
    QPushButton* m_deleteStudentBtn;
    QPushButton* m_refreshStudentsBtn;
    QLineEdit* m_searchStudentsEdit;
    
    // Courses page
    QTableView* m_coursesTable;
    QPushButton* m_addCourseBtn;
    QPushButton* m_editCourseBtn;
    QPushButton* m_deleteCourseBtn;
    QPushButton* m_refreshCoursesBtn;
    QLineEdit* m_searchCoursesEdit;
};

#endif // ADMINWINDOW_H

