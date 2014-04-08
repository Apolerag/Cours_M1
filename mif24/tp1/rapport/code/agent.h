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
    SystemeMotivationnel m_motivation;
    int m_motivationScore;
    QList<Interaction> m_trace;
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
