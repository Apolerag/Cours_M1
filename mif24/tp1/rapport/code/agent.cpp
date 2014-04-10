#include "agent.h"

/**
 * @brief Agent::Agent
 * @param e
 */
Agent::Agent(const Environnement& e): m_environnement(e), m_motivationScore(0)
{
    QFile file("trace.txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"Ouverture du fichier \"trace.txt\" impossible";
        return;
    }
    file.close();
}

/**
 * @brief Agent::~Agent
 */
Agent::~Agent()
{
}

/**
 * @brief Agent::save
 */
void Agent::save()
{
    QFile file("trace.txt");
    if(!file.open(QIODevice::Append))
    {
        qDebug()<<"Ouverture du fichier \"trace.txt\" impossible";
        return;
    }

    const Interaction& it = m_trace.last();

    QString toWrite;
    toWrite = "Experience: " + QString::number(it.experience().num()) + ", ";
    toWrite += "Resultat: " + QString::number(it.resultat().num()) + ", ";
    toWrite += "Motivation: " + QString::number(it.motivation()) 
    + ", Total = " + QString::number(m_motivationScore) + "\n";

    file.write(toWrite.toLatin1());


    file.close();
}

/**
 * @brief Agent::chooseResult
 * @return
 */
Resultat Agent::chooseResult() const
{
    QMap<QPair<const Experience, const Resultat>, Interaction> temp = m_motivation.systeme();

    Resultat ret;
    int motiv = 0;
    if(!temp.isEmpty())
    {
        motiv = temp.first().motivation();
        ret = temp.first().resultat();
    }

    foreach(const Interaction& it, temp)
    {
        if(it.motivation() > motiv)
        {
            ret = it.resultat();
            motiv = it.motivation();
        }
    }

    return ret;
}

/**
 * @brief Agent::chooseExperience
 * @param rm_motivation
 * @return
 */
Experience Agent::chooseExperience(const Resultat& r) const
{
    QMap<int, Experience> temp = m_environnement.experience();

    Experience ret;
    if(r.num() == 0)
    {
        ret = temp.first();
    }
    else
    {
        if(temp != m_exp)
        {
            for(QMap<int, Experience>::iterator it = temp.begin();
                it != temp.end(); it ++)
            {
                if(!m_exp.contains(it.key()))
                {
                    ret = it.value();
                }
            }
        }
        else
        {
            ret = m_motivation.exp(r);
        }
    }

    return ret;
}

/**
 * @brief Agent::addMotivation
 * @param i
 */
void Agent::addMotivation(const Interaction& i)
{
    m_exp[i.experience().num()] = i.experience();
    m_motivation.add(&(i.experience()), &(i.resultat()), i.motivation());
    m_motivationScore += m_motivation.interaction(i.experience(), i.resultat()).motivation();
    m_trace.push_back(m_motivation.interaction(i.experience(), i.resultat()));
    save();
}

