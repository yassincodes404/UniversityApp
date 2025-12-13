#ifndef COURSE_H
#define COURSE_H

#include <QString>

class Course
{
public:
    Course();
    Course(int id, const QString& code, const QString& name);
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    QString code() const { return m_code; }
    void setCode(const QString& code) { m_code = code; }
    
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    int creditHours() const { return m_creditHours; }
    void setCreditHours(int hours) { m_creditHours = hours; }
    
    int programId() const { return m_programId; }
    void setProgramId(int id) { m_programId = id; }
    
    int year() const { return m_year; }
    void setYear(int year) { m_year = year; }
    
    QString semester() const { return m_semester; }
    void setSemester(const QString& semester) { m_semester = semester; }
    
    QString courseType() const { return m_courseType; }
    void setCourseType(const QString& type) { m_courseType = type; }
    
    QString description() const { return m_description; }
    void setDescription(const QString& desc) { m_description = desc; }
    
    QString prerequisites() const { return m_prerequisites; }
    void setPrerequisites(const QString& prereq) { m_prerequisites = prereq; }

private:
    int m_id = 0;
    QString m_code;
    QString m_name;
    int m_creditHours = 3;
    int m_programId = 0;
    int m_year = 0;
    QString m_semester;
    QString m_courseType = "Theory";
    QString m_description;
    QString m_prerequisites;
};

#endif // COURSE_H

