#include "CourseRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CourseRepo::CourseRepo()
{
}

std::optional<Course> CourseRepo::findByCode(const QString& code)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites FROM courses WHERE code = :code");
    query.bindValue(":code", code);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::findByCode error:" << query.lastError().text();
        return std::nullopt;
    }
    
    if (query.next()) {
        Course course;
        course.id = query.value(0).toInt();
        course.code = query.value(1).toString();
        course.name = query.value(2).toString();
        course.creditHours = query.value(3).toInt();
        course.programId = query.value(4).toInt();
        course.year = query.value(5).toInt();
        course.semester = Course::semesterFromString(query.value(6).toString());
        course.courseType = Course::courseTypeFromString(query.value(7).toString());
        course.description = query.value(8).toString();
        course.prerequisites = query.value(9).toString();
        return course;
    }
    
    return std::nullopt;
}

std::optional<Course> CourseRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites FROM courses WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::findById error:" << query.lastError().text();
        return std::nullopt;
    }
    
    if (query.next()) {
        Course course;
        course.id = query.value(0).toInt();
        course.code = query.value(1).toString();
        course.name = query.value(2).toString();
        course.creditHours = query.value(3).toInt();
        course.programId = query.value(4).toInt();
        course.year = query.value(5).toInt();
        course.semester = Course::semesterFromString(query.value(6).toString());
        course.courseType = Course::courseTypeFromString(query.value(7).toString());
        course.description = query.value(8).toString();
        course.prerequisites = query.value(9).toString();
        return course;
    }
    
    return std::nullopt;
}

QList<Course> CourseRepo::findAll()
{
    QList<Course> courses;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites FROM courses ORDER BY code");
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::findAll error:" << query.lastError().text();
        return courses;
    }
    
    while (query.next()) {
        Course course;
        course.id = query.value(0).toInt();
        course.code = query.value(1).toString();
        course.name = query.value(2).toString();
        course.creditHours = query.value(3).toInt();
        course.programId = query.value(4).toInt();
        course.year = query.value(5).toInt();
        course.semester = Course::semesterFromString(query.value(6).toString());
        course.courseType = Course::courseTypeFromString(query.value(7).toString());
        course.description = query.value(8).toString();
        course.prerequisites = query.value(9).toString();
        courses.append(course);
    }
    
    return courses;
}

QList<Course> CourseRepo::findByProgram(int programId)
{
    QList<Course> courses;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites FROM courses WHERE program_id = :program_id ORDER BY code");
    query.bindValue(":program_id", programId);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::findByProgram error:" << query.lastError().text();
        return courses;
    }
    
    while (query.next()) {
        Course course;
        course.id = query.value(0).toInt();
        course.code = query.value(1).toString();
        course.name = query.value(2).toString();
        course.creditHours = query.value(3).toInt();
        course.programId = query.value(4).toInt();
        course.year = query.value(5).toInt();
        course.semester = Course::semesterFromString(query.value(6).toString());
        course.courseType = Course::courseTypeFromString(query.value(7).toString());
        course.description = query.value(8).toString();
        course.prerequisites = query.value(9).toString();
        courses.append(course);
    }
    
    return courses;
}

QList<Course> CourseRepo::findByYearAndSemester(int year, Semester semester)
{
    QList<Course> courses;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites FROM courses WHERE year = :year AND semester = :semester ORDER BY code");
    query.bindValue(":year", year);
    Course temp;
    temp.semester = semester;
    query.bindValue(":semester", temp.semesterString());
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::findByYearAndSemester error:" << query.lastError().text();
        return courses;
    }
    
    while (query.next()) {
        Course course;
        course.id = query.value(0).toInt();
        course.code = query.value(1).toString();
        course.name = query.value(2).toString();
        course.creditHours = query.value(3).toInt();
        course.programId = query.value(4).toInt();
        course.year = query.value(5).toInt();
        course.semester = Course::semesterFromString(query.value(6).toString());
        course.courseType = Course::courseTypeFromString(query.value(7).toString());
        course.description = query.value(8).toString();
        course.prerequisites = query.value(9).toString();
        courses.append(course);
    }
    
    return courses;
}

bool CourseRepo::save(const Course& course)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("INSERT INTO courses (code, name, credit_hours, program_id, year, semester, "
                  "course_type, description, prerequisites) "
                  "VALUES (:code, :name, :hours, :program, :year, :semester, :type, :desc, :prereq)");
    
    query.bindValue(":code", course.code);
    query.bindValue(":name", course.name);
    query.bindValue(":hours", course.creditHours);
    query.bindValue(":program", course.programId);
    query.bindValue(":year", course.year);
    query.bindValue(":semester", course.semesterString());
    query.bindValue(":type", course.courseTypeString());
    query.bindValue(":desc", course.description);
    query.bindValue(":prereq", course.prerequisites);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::save error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool CourseRepo::update(const Course& course)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE courses SET name = :name, credit_hours = :hours, program_id = :program, "
                  "year = :year, semester = :semester, course_type = :type, "
                  "description = :desc, prerequisites = :prereq WHERE id = :id");
    
    query.bindValue(":id", course.id);
    query.bindValue(":name", course.name);
    query.bindValue(":hours", course.creditHours);
    query.bindValue(":program", course.programId);
    query.bindValue(":year", course.year);
    query.bindValue(":semester", course.semesterString());
    query.bindValue(":type", course.courseTypeString());
    query.bindValue(":desc", course.description);
    query.bindValue(":prereq", course.prerequisites);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool CourseRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM courses WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::remove error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

