#ifndef NEWS_H
#define NEWS_H

#include <QString>
#include <QDate>
#include <QDateTime>

class News
{
public:
    News();
    
    int id() const { return m_id; }
    void setId(int id) { m_id = id; }
    
    QString title() const { return m_title; }
    void setTitle(const QString& title) { m_title = title; }
    
    QString content() const { return m_content; }
    void setContent(const QString& content) { m_content = content; }
    
    QDate newsDate() const { return m_newsDate; }
    void setNewsDate(const QDate& date) { m_newsDate = date; }
    
    int createdBy() const { return m_createdBy; }
    void setCreatedBy(int adminId) { m_createdBy = adminId; }
    
    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime& dt) { m_createdAt = dt; }

private:
    int m_id = 0;
    QString m_title;
    QString m_content;
    QDate m_newsDate;
    int m_createdBy = 0;
    QDateTime m_createdAt;
};

#endif // NEWS_H

