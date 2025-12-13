#include "PasswordHasher.h"
#include <QByteArray>

QString PasswordHasher::generateSalt()
{
    const int saltLength = 16;
    QByteArray saltBytes;
    saltBytes.resize(saltLength);
    
    for (int i = 0; i < saltLength; ++i) {
        saltBytes[i] = static_cast<char>(QRandomGenerator::global()->bounded(256));
    }
    
    return saltBytes.toHex();
}

QString PasswordHasher::hashPassword(const QString& password, const QString& salt)
{
    QString salted = salt + password;
    QByteArray hash = QCryptographicHash::hash(salted.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}

bool PasswordHasher::verifyPassword(const QString& password, const QString& hash, const QString& salt)
{
    QString computedHash = hashPassword(password, salt);
    return computedHash == hash;
}

