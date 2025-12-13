#ifndef GRADINGSERVICE_H
#define GRADINGSERVICE_H

#include "../Models/Grade.h"

class GradingService
{
public:
    static QString calculateLetterGrade(double total);
    static double calculateGPA(const QList<Grade>& grades);
    static double calculateTotalGrade(double midterm1, double midterm2, double finalExam, 
                                      double practical, double assignments);
    static QString determineResultStatus(double total, double attendancePercentage);
};

#endif // GRADINGSERVICE_H

