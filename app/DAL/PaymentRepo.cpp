#include "PaymentRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PaymentRepo::PaymentRepo()
{
}

QList<Payment> PaymentRepo::findByStudent(int studentId)
{
    QList<Payment> payments;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM payments WHERE student_id = ? ORDER BY payment_date DESC");
    query.addBindValue(studentId);
    
    if (query.exec()) {
        while (query.next()) {
            Payment payment;
            payment.setId(query.value("id").toInt());
            payment.setStudentId(query.value("student_id").toInt());
            payment.setAmount(query.value("amount").toDouble());
            payment.setSemester(query.value("semester").toInt());
            payment.setPaymentDate(QDateTime::fromString(query.value("payment_date").toString(), "yyyy-MM-dd HH:mm:ss"));
            payment.setMethod(query.value("method").toString());
            payment.setStatus(query.value("status").toString());
            payment.setNotes(query.value("notes").toString());
            payments.append(payment);
        }
    }
    
    return payments;
}

bool PaymentRepo::save(Payment& payment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO payments (student_id, amount, semester, payment_date, method, status, notes) "
        "VALUES (?, ?, ?, ?, ?, ?, ?)"
    );
    
    query.addBindValue(payment.studentId());
    query.addBindValue(payment.amount());
    query.addBindValue(payment.semester());
    query.addBindValue(payment.paymentDate().toString("yyyy-MM-dd HH:mm:ss"));
    query.addBindValue(payment.method());
    query.addBindValue(payment.status());
    query.addBindValue(payment.notes());
    
    if (!query.exec()) {
        qDebug() << "PaymentRepo::save error:" << query.lastError().text();
        return false;
    }
    
    payment.setId(query.lastInsertId().toInt());
    return true;
}

bool PaymentRepo::update(const Payment& payment)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "UPDATE payments SET student_id = ?, amount = ?, semester = ?, payment_date = ?, "
        "method = ?, status = ?, notes = ? WHERE id = ?"
    );
    
    query.addBindValue(payment.studentId());
    query.addBindValue(payment.amount());
    query.addBindValue(payment.semester());
    query.addBindValue(payment.paymentDate().toString("yyyy-MM-dd HH:mm:ss"));
    query.addBindValue(payment.method());
    query.addBindValue(payment.status());
    query.addBindValue(payment.notes());
    query.addBindValue(payment.id());
    
    if (!query.exec()) {
        qDebug() << "PaymentRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

