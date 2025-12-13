#include "GradingService.h"
#include "../DAL/DatabaseManager.h"
#include "../DAL/StudentRepo.h"
#include "../DAL/CourseRepo.h"
#include "../DAL/EnrollmentRepo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <cmath>

GradingService::GradingService()
{
}

double GradingService::calculateTotal(double midterm1, double midterm2, double finalExam, 
                                      double practical, double assignments)
{
    return (midterm1 * WEIGHT_MIDTERM1) +
           (midterm2 * WEIGHT_MIDTERM2) +
           (finalExam * WEIGHT_FINAL) +
           (practical * WEIGHT_PRACTICAL) +
           (assignments * WEIGHT_ASSIGNMENTS);
}

QString GradingService::numericToLetter(double total)
{
    if (total >= 90.0) return "A";
    if (total >= 85.0) return "B+";
    if (total >= 80.0) return "B";
    if (total >= 75.0) return "C+";
    if (total >= 70.0) return "C";
    if (total >= 60.0) return "D";
    return "F";
}

QString GradingService::determineStatus(double total)
{
    return total >= 60.0 ? "pass" : "fail";
}

bool GradingService::saveGrade(const GradeEntry& grade)
{
    QSqlQuery query(DatabaseManager::instance().database());
    
    // Check if grade already exists
    query.prepare("SELECT id FROM grades WHERE student_id = :student AND course_id = :course");
    query.bindValue(":student", grade.studentId);
    query.bindValue(":course", grade.courseId);
    
    if (query.exec() && query.next()) {
        // Update existing
        int gradeId = query.value(0).toInt();
        query.prepare("UPDATE grades SET midterm1 = :m1, midterm2 = :m2, final_exam = :final, "
                     "practical = :prac, assignments = :assign, total = :total, "
                     "letter_grade = :letter, result_status = :status WHERE id = :id");
        query.bindValue(":id", gradeId);
    } else {
        // Insert new
        query.prepare("INSERT INTO grades (student_id, course_id, midterm1, midterm2, final_exam, "
                     "practical, assignments, total, letter_grade, result_status) "
                     "VALUES (:student, :course, :m1, :m2, :final, :prac, :assign, :total, :letter, :status)");
    }
    
    query.bindValue(":student", grade.studentId);
    query.bindValue(":course", grade.courseId);
    query.bindValue(":m1", grade.midterm1);
    query.bindValue(":m2", grade.midterm2);
    query.bindValue(":final", grade.finalExam);
    query.bindValue(":prac", grade.practical);
    query.bindValue(":assign", grade.assignments);
    query.bindValue(":total", grade.total);
    query.bindValue(":letter", grade.letterGrade);
    query.bindValue(":status", grade.resultStatus);
    
    if (!query.exec()) {
        qDebug() << "GradingService::saveGrade error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool GradingService::updateStudentGPA(int studentId)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT g.course_id, g.total, g.result_status, c.credit_hours "
                  "FROM grades g "
                  "JOIN courses c ON g.course_id = c.id "
                  "WHERE g.student_id = :student AND g.result_status = 'pass'");
    query.bindValue(":student", studentId);
    
    if (!query.exec()) {
        qDebug() << "GradingService::updateStudentGPA error:" << query.lastError().text();
        return false;
    }
    
    double totalPoints = 0.0;
    int totalCredits = 0;
    
    while (query.next()) {
        double grade = query.value(1).toDouble();
        int credits = query.value(3).toInt();
        
        // Convert grade to GPA points
        double points = 0.0;
        if (grade >= 90.0) points = 4.0;
        else if (grade >= 85.0) points = 3.7;
        else if (grade >= 80.0) points = 3.0;
        else if (grade >= 75.0) points = 2.7;
        else if (grade >= 70.0) points = 2.0;
        else if (grade >= 60.0) points = 1.0;
        
        totalPoints += points * credits;
        totalCredits += credits;
    }
    
    double gpa = totalCredits > 0 ? totalPoints / totalCredits : 0.0;
    
    StudentRepo repo;
    return repo.updateGPA(studentId, gpa, totalCredits);
}

bool GradingService::publishGradesForCourse(int courseId)
{
    EnrollmentRepo enrollmentRepo;
    auto enrollments = enrollmentRepo.findByCourse(courseId);
    
    DatabaseManager::instance().beginTransaction();
    
    for (const auto& enrollment : enrollments) {
        // Get grade for this enrollment
        QSqlQuery query(DatabaseManager::instance().database());
        query.prepare("SELECT midterm1, midterm2, final_exam, practical, assignments "
                     "FROM grades WHERE student_id = :student AND course_id = :course");
        query.bindValue(":student", enrollment.studentId);
        query.bindValue(":course", courseId);
        
        if (!query.exec() || !query.next()) {
            continue;
        }
        
        double m1 = query.value(0).toDouble();
        double m2 = query.value(1).toDouble();
        double final = query.value(2).toDouble();
        double prac = query.value(3).toDouble();
        double assign = query.value(4).toDouble();
        
        double total = calculateTotal(m1, m2, final, prac, assign);
        QString letter = numericToLetter(total);
        QString status = determineStatus(total);
        
        // Update grade
        QSqlQuery updateQuery(DatabaseManager::instance().database());
        updateQuery.prepare("UPDATE grades SET total = :total, letter_grade = :letter, "
                           "result_status = :status WHERE student_id = :student AND course_id = :course");
        updateQuery.bindValue(":total", total);
        updateQuery.bindValue(":letter", letter);
        updateQuery.bindValue(":status", status);
        updateQuery.bindValue(":student", enrollment.studentId);
        updateQuery.bindValue(":course", courseId);
        updateQuery.exec();
        
        // Update student GPA
        updateStudentGPA(enrollment.studentId);
    }
    
    return DatabaseManager::instance().commitTransaction();
}

