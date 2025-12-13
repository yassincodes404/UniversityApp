#include "PasswordHasher.h"
#include <QRandomGenerator>

QString PasswordHasher::generateSalt()
{
    // Generate a random 32-character salt
    const QString chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString salt;
    salt.reserve(32);
    
    for (int i = 0; i < 32; ++i) {
        salt.append(chars.at(QRandomGenerator::global()->bounded(chars.length())));
    }
    
    return salt;
}

QString PasswordHasher::hashPassword(const QString& password, const QString& salt)
{
    QString salted = salt + password;
    QByteArray hash = QCryptographicHash::hash(salted.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromLatin1(hash.toHex());
}

bool PasswordHasher::verifyPassword(const QString& password, const QString& hash, const QString& salt)
{
    QString computedHash = hashPassword(password, salt);
    return computedHash == hash;
}

