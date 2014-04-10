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
    SystemeMotivationnel m_motivation; // l ensemble des motivations du systeme.
    int m_motivationScore; //correspondont a la somme des resultats de
			   //toutes les experiences passees.
    QList<Interaction> m_trace; //la liste de toutes les interactions 
				//effectuees par l'agent

    const Environnement& m_environnement; //l'environnement ou se situe l'agent
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
