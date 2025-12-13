#ifndef PASSWORDHASHER_H
#define PASSWORDHASHER_H

#include <QString>
#include <QCryptographicHash>
#include <QDateTime>

class PasswordHasher
{
public:
    static QString generateSalt();
    static QString hashPassword(const QString& password, const QString& salt);
    static bool verifyPassword(const QString& password, const QString& hash, const QString& salt);
};

#endif // PASSWORDHASHER_H

