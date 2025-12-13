#include "Course.h"

Course::Course()
{
}

Course::Course(int id, const QString& code, const QString& name)
    : m_id(id), m_code(code), m_name(name)
{
}

