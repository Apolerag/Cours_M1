#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <QSet>
#include <QDebug>

class Experience
{
private:
    static QSet<int> m_nbExp;
    int m_num;

public:
    Experience(int = 0);
    Experience(const Experience&);
    ~Experience();
    int num() const;

    void affichage() const;

    bool operator <(const Experience&) const;
    bool operator == (const Experience&) const;
    Experience& operator =(const Experience&);
};

uint qHash(const Experience&);

#endif // EXPERIENCE_H
