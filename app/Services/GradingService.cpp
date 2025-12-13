#include "GradingService.h"
#include "../Models/Grade.h"
#include <QList>

QString GradingService::calculateLetterGrade(double total)
{
    if (total >= 90.0) return "A";
    if (total >= 85.0) return "B+";
    if (total >= 80.0) return "B";
    if (total >= 75.0) return "C+";
    if (total >= 70.0) return "C";
    if (total >= 60.0) return "D";
    return "F";
}

double GradingService::calculateGPA(const QList<Grade>& grades)
{
    if (grades.isEmpty()) return 0.0;
    
    double totalPoints = 0.0;
    int totalCredits = 0;
    
    // This is simplified - in real implementation, you'd need course credit hours
    for (const auto& grade : grades) {
        if (grade.isPublished() && grade.resultStatus() == "pass") {
            QString letter = grade.letterGrade();
            double points = 0.0;
            
            if (letter == "A") points = 4.0;
            else if (letter == "B+") points = 3.7;
            else if (letter == "B") points = 3.0;
            else if (letter == "C+") points = 2.7;
            else if (letter == "C") points = 2.0;
            else if (letter == "D") points = 1.0;
            else points = 0.0;
            
            // Assuming 3 credit hours per course - should get from course
            int credits = 3;
            totalPoints += points * credits;
            totalCredits += credits;
        }
    }
    
    if (totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

double GradingService::calculateTotalGrade(double midterm1, double midterm2, double finalExam, 
                                           double practical, double assignments)
{
    // Simple sum - in production, apply weights per course
    return midterm1 + midterm2 + finalExam + practical + assignments;
}

QString GradingService::determineResultStatus(double total, double attendancePercentage)
{
    if (attendancePercentage < 75.0) {
        return "barred";
    }
    
    if (total >= 60.0) {
        return "pass";
    }
    
    return "fail";
}

