#include "NewsRepo.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

NewsRepo::NewsRepo()
{
}

QList<News> NewsRepo::findAll()
{
    QList<News> newsList;
    QSqlQuery query("SELECT * FROM news ORDER BY news_date DESC, created_at DESC", DatabaseManager::instance().database());
    
    while (query.next()) {
        News news;
        news.setId(query.value("id").toInt());
        news.setTitle(query.value("title").toString());
        news.setContent(query.value("content").toString());
        news.setNewsDate(QDate::fromString(query.value("news_date").toString(), "yyyy-MM-dd"));
        news.setCreatedBy(query.value("created_by").toInt());
        news.setCreatedAt(QDateTime::fromString(query.value("created_at").toString(), "yyyy-MM-dd HH:mm:ss"));
        newsList.append(news);
    }
    
    return newsList;
}

bool NewsRepo::save(News& news)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "INSERT INTO news (title, content, news_date, created_by) "
        "VALUES (?, ?, ?, ?)"
    );
    
    query.addBindValue(news.title());
    query.addBindValue(news.content());
    query.addBindValue(news.newsDate().toString("yyyy-MM-dd"));
    query.addBindValue(news.createdBy());
    
    if (!query.exec()) {
        qDebug() << "NewsRepo::save error:" << query.lastError().text();
        return false;
    }
    
    news.setId(query.lastInsertId().toInt());
    return true;
}

bool NewsRepo::update(const News& news)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("UPDATE news SET title = ?, content = ?, news_date = ? WHERE id = ?");
    
    query.addBindValue(news.title());
    query.addBindValue(news.content());
    query.addBindValue(news.newsDate().toString("yyyy-MM-dd"));
    query.addBindValue(news.id());
    
    if (!query.exec()) {
        qDebug() << "NewsRepo::update error:" << query.lastError().text();
        return false;
    }
    
    return true;
}

bool NewsRepo::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM news WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

