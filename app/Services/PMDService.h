#ifndef PMDSERVICE_H
#define PMDSERVICE_H

#include <QString>

class PMDService
{
public:
    static bool createPMDRecord(int studentId, int courseId, int examId, 
                                const QString& status, const QString& notes);
    static bool hasPMD(int studentId, int courseId);
};

#endif // PMDSERVICE_H

