#ifndef GRADINGSERVICE_H
#define GRADINGSERVICE_H

#include <QString>

struct GradeEntry {
    int studentId;
    int courseId;
    double midterm1;
    double midterm2;
    double finalExam;
    double practical;
    double assignments;
    double total;
    QString letterGrade;
    QString resultStatus;
};

class GradingService
{
public:
    GradingService();
    
    // Calculate total grade from components (with weights)
    double calculateTotal(double midterm1, double midterm2, double finalExam, 
                         double practical, double assignments);
    
    // Convert numeric grade to letter grade
    QString numericToLetter(double total);
    
    // Determine pass/fail status
    QString determineStatus(double total);
    
    // Save grade entry to database
    bool saveGrade(const GradeEntry& grade);
    
    // Update student GPA after publishing grades
    bool updateStudentGPA(int studentId);
    
    // Publish grades for a course (calculate and save all)
    bool publishGradesForCourse(int courseId);
    
private:
    // Grade weights (configurable)
    static constexpr double WEIGHT_MIDTERM1 = 0.15;
    static constexpr double WEIGHT_MIDTERM2 = 0.15;
    static constexpr double WEIGHT_FINAL = 0.50;
    static constexpr double WEIGHT_PRACTICAL = 0.10;
    static constexpr double WEIGHT_ASSIGNMENTS = 0.10;
};

#endif // GRADINGSERVICE_H

