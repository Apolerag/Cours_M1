#ifndef INTERACTION_H
#define INTERACTION_H

#include <QMultiMap>
#include <QDebug>

#include "resultat.h"
#include "experience.h"

class Interaction
{
private:
    const Experience* m_experience;
    const Resultat* m_resultat;
    int m_motiv;

public:
    Interaction(const Experience* = nullptr, const Resultat* = nullptr, int = 0);// Experience, resultat, motiv
    Interaction(const Interaction&);

    int motivation() const;
    void setMotivation(int);
    const Experience& experience() const;
    const Resultat& resultat() const;

    void affichage() const;

    bool operator == (const Interaction&) const;
    bool operator < (const Interaction&) const;
    Interaction& operator = (const Interaction&);
};

uint qHash(const Interaction&);
#endif // INTERACTION_H
