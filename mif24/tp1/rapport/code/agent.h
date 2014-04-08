#ifndef AGENT_H
#define AGENT_H

#include <QFile>
#include <QList>
#include "resultat.h"
#include "experience.h"
#include "systememotivationnel.h"
#include "environnement.h"


class Agent
{
private:
<<<<<<< HEAD
    SystemeMotivationnel m_motivation; //l'ensemble des motivations du système.
    int m_motivationScore; //correspondont à la somme des résultats de toutes les experiences passées.
    QList<Interaction> m_trace; //
=======
    SystemeMotivationnel m_motivation;
    int m_motivationScore;
    QList<Interaction> m_trace;
>>>>>>> 283a34f487b1bd6016d3e0a91a1f82526d5813de
    const Environnement& m_environnement;
    QMap<int, Experience> m_exp;

    void save();

public:
    Agent(const Environnement&);
    ~Agent();

    Resultat chooseResult() const;
    Experience chooseExperience(const Resultat&) const;
    void addMotivation(const Interaction&);
};

#endif // AGENT_H
