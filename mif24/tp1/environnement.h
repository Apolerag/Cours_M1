#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include <QList>
#include <QPair>
#include <QMultiMap>
#include <QString>

#include "resultat.h"
#include "experience.h"

class Environnement
{
private:
    QList<QPair<int, int>> m_liste; //first experience, second resultat

protected:
    void addPair(int, int);
    QMap<int, Resultat> m_resultat;
    QMap<int, Experience> m_experience;

public:
    Environnement(int, int);
    const Resultat& result(const Experience&) const;
    const QMap<int, Experience>& experience() const;
    const Resultat &resultat(int) const;
    const Experience& experience(int) const;
    int nbExperience() const;
    int nbResulat() const;
    void affichage() const;
};


class Environnement1: public Environnement
{
public:
    Environnement1();
};

class Environnement2: public Environnement
{
public:
    Environnement2();
};

#endif // ENVIRONNEMENT_H
