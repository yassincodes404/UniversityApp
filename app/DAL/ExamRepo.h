#ifndef EXAMREPO_H
#define EXAMREPO_H

#include "../Models/Exam.h"
#include <QList>

class ExamRepo
{
public:
    ExamRepo();
    
    QList<Exam> findByCourse(int courseId);
    bool save(Exam& exam);
};

#endif // EXAMREPO_H

