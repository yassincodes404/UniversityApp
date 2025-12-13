#include "CourseRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CourseRepo::CourseRepo()
{
}

std::optional<Course> CourseRepo::findById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM courses WHERE id = ?");
    query.addBindValue(id);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

std::optional<Course> CourseRepo::findByCode(const QString& code)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM courses WHERE code = ?");
    query.addBindValue(code);
    
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    
    return mapFromQuery(query);
}

QList<Course> CourseRepo::findAll()
{
    QList<Course> courses;
    QSqlQuery query("SELECT * FROM courses ORDER BY code", DatabaseManager::instance().database());
    
    while (query.next()) {
        courses.append(mapFromQuery(query));
    }
    
    return courses;
}

QList<Course> CourseRepo::findByProgram(int programId)
{
    QList<Course> courses;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM courses WHERE program_id = ? ORDER BY code");
    query.addBindValue(programId);
    
    if (query.exec()) {
        while (query.next()) {
            courses.append(mapFromQuery(query));
        }
    }
    
    return courses;
}

QList<Course> CourseRepo::findByYearAndSemester(int year, const QString& semester)
{
    QList<Course> courses;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM courses WHERE year = ? AND semester = ? ORDER BY code");
    query.addBindValue(year);
    query.addBindValue(semester);
    
    if (query.exec()) {
        while (query.next()) {
            courses.append(mapFromQuery(query));
        }
    }
    
    return courses;
}

bool CourseRepo::save(Course& course)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO courses (code, name, credit_hours, program_id, year, semester, "
        "course_type, description, prerequisites) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(course.code());
    query.addBindValue(course.name());
    query.addBindValue(course.creditHours());
    query.addBindValue(course.programId());
    query.addBindValue(course.year());
    query.addBindValue(course.semester());
    query.addBindValue(course.courseType());
    query.addBindValue(course.description());
    query.addBindValue(course.prerequisites());
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::save error:" << query.lastError().text();
        return false;
    }
    
    course.setId(query.lastInsertId().toInt());
    return true;
}

bool CourseRepo::update(const Course& course)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE courses SET code = ?, name = ?, credit_hours = ?, program_id = ?, "
        "year = ?, semester = ?, course_type = ?, description = ?, prerequisites = ? "
        "WHERE id = ?"
    );
    
    query.addBindValue(course.code());
    query.addBindValue(course.name());
    query.addBindValue(course.creditHours());
    query.addBindValue(course.programId());
    query.addBindValue(course.year());
    query.addBindValue(course.semester());
    query.addBindValue(course.courseType());
    query.addBindValue(course.description());
    query.addBindValue(course.prerequisites());
    query.addBindValue(course.id());
    
    if (!query.exec()) {
        qDebug() << "CourseRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool CourseRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM courses WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

Course CourseRepo::mapFromQuery(const QSqlQuery& query)
{
    Course course;
    course.setId(query.value("id").toInt());
    course.setCode(query.value("code").toString());
    course.setName(query.value("name").toString());
    course.setCreditHours(query.value("credit_hours").toInt());
    course.setProgramId(query.value("program_id").toInt());
    course.setYear(query.value("year").toInt());
    course.setSemester(query.value("semester").toString());
    course.setCourseType(query.value("course_type").toString());
    course.setDescription(query.value("description").toString());
    course.setPrerequisites(query.value("prerequisites").toString());
    
    return course;
}

