#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include "../Models/Student.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QLabel;
QT_END_NAMESPACE

class StudentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentWindow(const Student& student, QWidget* parent = nullptr);
    ~StudentWindow();

private slots:
    void onMenuSelectionChanged();
    void onRefreshData();

private:
    void setupUI();
    void setupProfilePage();
    void setupCoursesPage();
    void setupGradesPage();
    void setupPaymentsPage();
    void setupNewsPage();
    void loadProfile();
    void loadCourses();
    void loadGrades();
    void loadPayments();
    void loadNews();
    
    Student m_student;
    QStackedWidget* m_stackedWidget;
    QListWidget* m_menuList;
    
    // Profile page
    QLabel* m_nameLabel;
    QLabel* m_codeLabel;
    QLabel* m_emailLabel;
    QLabel* m_phoneLabel;
    QLabel* m_levelLabel;
    QLabel* m_gpaLabel;
    QLabel* m_creditsLabel;
    
    // Courses page
    QTableView* m_coursesTable;
    
    // Grades page
    QTableView* m_gradesTable;
    
    // Payments page
    QTableView* m_paymentsTable;
};

#endif // STUDENTWINDOW_H

