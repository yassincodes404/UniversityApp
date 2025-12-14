#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QMainWindow>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QComboBox;
class QLineEdit;
class QModelIndex;
QT_END_NAMESPACE

class DoctorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DoctorWindow(int doctorId, QWidget *parent = nullptr);
    ~DoctorWindow();

signals:
    void logoutRequested();

private slots:
    void onLogout();
    void onCourseSelected(int index);
    void onEnterGrades();
    void onMarkAttendance();
    void refreshData();
    void onGradeCellDoubleClicked(const QModelIndex &index);

private:
    void setupUI();
    void setupMenuBar();
    void loadCourses();
    void loadStudents();
    
    int m_doctorId;
    QTabWidget* m_tabs;
    QPushButton* m_logoutButton;
    QComboBox* m_courseCombo;
    QTableView* m_studentsTable;
    QTableView* m_gradesTable;
    QLineEdit* m_studentSearchEdit;
    QLineEdit* m_gradeSearchEdit;
};

#endif // DOCTORWINDOW_H

