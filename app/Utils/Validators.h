#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <QString>

class Validators
{
public:
    static bool isValidEmail(const QString& email);
    static bool isValidPhone(const QString& phone);
    static bool isValidStudentCode(const QString& code);
    static bool isValidGrade(double grade);
};

#endif // VALIDATORS_H

