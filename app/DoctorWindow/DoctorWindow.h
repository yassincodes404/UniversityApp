#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QListWidget>
#include "../Models/User.h"

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QComboBox;
QT_END_NAMESPACE

class DoctorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoctorWindow(const User& doctor, QWidget* parent = nullptr);
    ~DoctorWindow();

private slots:
    void onMenuSelectionChanged();
    void onCourseSelected();
    void onEnterGrades();
    void onMarkAttendance();

private:
    void setupUI();
    void setupDashboardPage();
    void setupCoursesPage();
    void setupGradeEntryPage();
    void setupAttendancePage();
    void loadCourses();
    void loadStudentsForCourse(int courseId);
    
    User m_doctor;
    QStackedWidget* m_stackedWidget;
    QListWidget* m_menuList;
    
    // Courses page
    QTableView* m_coursesTable;
    QComboBox* m_courseCombo;
    
    // Grade entry page
    QTableView* m_studentsTable;
    QPushButton* m_saveGradesBtn;
    
    // Attendance page
    QTableView* m_attendanceTable;
    QPushButton* m_markAttendanceBtn;
};

#endif // DOCTORWINDOW_H

