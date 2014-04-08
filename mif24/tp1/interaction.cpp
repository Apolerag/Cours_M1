#include "interaction.h"

/**
 * @brief Interaction::Interaction
 * @param experience
 * @param resultat
 * @param motiv
 */
Interaction::Interaction(const Experience* experience, const Resultat* resultat, int motiv): m_experience(experience),
    m_resultat(resultat), m_motiv(motiv)
{
}

/**
 * @brief Interaction::Interaction
 * @param i
 */
Interaction::Interaction(const Interaction & i): m_experience(i.m_experience),
    m_resultat(i.m_resultat), m_motiv(i.motivation())
{
}

/**
 * @brief Interaction::motivation
 * @return
 */
int Interaction::motivation() const
{
    return m_motiv;
}

/**
 * @brief Interaction::setMotivation
 * @param val
 */
void Interaction::setMotivation(int val)
{
    m_motiv = val;
}

/**
 * @brief Interaction::experience
 * @return
 */
const Experience& Interaction::experience() const
{
    return *m_experience;
}

/**
 * @brief Interaction::resultat
 * @return
 */
const Resultat& Interaction::resultat() const
{
    return *m_resultat;
}

/**
 * @brief Interaction::affichage
 */
void Interaction::affichage() const
{
    qDebug()<<"Experience: "<<m_experience->num()<<"Resultat: "<<m_resultat->num()<<"Motivation: "<<m_motiv;
}

/**
 * @brief Interaction::operator <
 * @param i
 * @return
 */
bool Interaction::operator <(const Interaction& i) const
{
    return motivation() < i .motivation();
}

/**
 * @brief Interaction::operator ==
 * @param i
 * @return
 */
bool Interaction::operator ==(const Interaction& i) const
{
    return (experience() == i.experience()) &&
            (resultat() == i.resultat()) &&
            (motivation() == i.motivation());
}

/**
 * @brief Interaction::operator =
 * @param i
 * @return
 */
Interaction& Interaction::operator = (const Interaction& i)
{
    m_experience = i.m_experience;
    m_resultat = i.m_resultat;
    m_motiv = i.motivation();
    return *this;
}

/**
 * @brief qHash
 * @param i
 * @return
 */
uint qHash(const Interaction& i)
{
    return i.motivation();
}
