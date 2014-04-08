#ifndef RESULTAT_H
#define RESULTAT_H

#include <QSet>
#include <QDebug>

class Resultat
{
private:
    static QSet<int> m_nbRes;
    int m_num;

public:
    Resultat(int = 0);
    Resultat(const Resultat&);
    ~Resultat();

    int num() const;
    void affichage() const;

    bool operator <(const Resultat&) const;
    bool operator == (const Resultat&) const;
    Resultat& operator =(const Resultat&);
};

uint qHash(const Resultat& r);

#endif // RESULTAT_H
