#include "Student.h"

Student::Student()
{
}

Student::Student(int id, const QString& studentCode, const QString& name)
    : m_id(id), m_studentCode(studentCode), m_name(name)
{
}

