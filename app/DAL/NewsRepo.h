#ifndef NEWSREPO_H
#define NEWSREPO_H

#include "../Models/News.h"
#include <QList>

class NewsRepo
{
public:
    NewsRepo();
    
    QList<News> findAll();
    bool save(News& news);
    bool update(const News& news);
    bool remove(int id);
};

#endif // NEWSREPO_H

