#ifndef PAYMENTREPO_H
#define PAYMENTREPO_H

#include "../Models/Payment.h"
#include <QList>

class PaymentRepo
{
public:
    PaymentRepo();
    
    QList<Payment> findByStudent(int studentId);
    bool save(Payment& payment);
    bool update(const Payment& payment);
};

#endif // PAYMENTREPO_H

