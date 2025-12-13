#ifndef DOCTOR_H
#define DOCTOR_H

#include <QString>

class Doctor
{
public:
    Doctor();
    Doctor(int id, const QString& name);
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    QString email() const { return m_email; }
    void setEmail(const QString& email) { m_email = email; }
    
    QString phone() const { return m_phone; }
    void setPhone(const QString& phone) { m_phone = phone; }
    
    QString passwordHash() const { return m_passwordHash; }
    void setPasswordHash(const QString& hash) { m_passwordHash = hash; }
    
    QString salt() const { return m_salt; }
    void setSalt(const QString& salt) { m_salt = salt; }
    
    int departmentId() const { return m_departmentId; }
    void setDepartmentId(int id) { m_departmentId = id; }

private:
    int m_id = 0;
    QString m_name;
    QString m_email;
    QString m_phone;
    QString m_passwordHash;
    QString m_salt;
    int m_departmentId = 0;
};

#endif // DOCTOR_H

