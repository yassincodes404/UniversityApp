#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QDateTime>

class Payment
{
public:
    Payment();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    int studentId() const { return m_studentId; }
    void setStudentId(int id) { m_studentId = id; }
    
    double amount() const { return m_amount; }
    void setAmount(double amount) { m_amount = amount; }
    
    int semester() const { return m_semester; }
    void setSemester(int sem) { m_semester = sem; }
    
    QDateTime paymentDate() const { return m_paymentDate; }
    void setPaymentDate(const QDateTime& dt) { m_paymentDate = dt; }
    
    QString method() const { return m_method; }
    void setMethod(const QString& method) { m_method = method; }
    
    QString status() const { return m_status; }
    void setStatus(const QString& status) { m_status = status; }
    
    QString notes() const { return m_notes; }
    void setNotes(const QString& notes) { m_notes = notes; }

private:
    int m_id = 0;
    int m_studentId = 0;
    double m_amount = 0.0;
    int m_semester = 0;
    QDateTime m_paymentDate;
    QString m_method = "Online";
    QString m_status = "Pending";
    QString m_notes;
};

#endif // PAYMENT_H

