#include "systememotivationnel.h"

/**
 * @brief SystemeMotivationnel::SystemeMotivationnel
 */
SystemeMotivationnel::SystemeMotivationnel()
{
}

/**
 * @brief SystemeMotivationnel::interaction
 * @param e
 * @param r
 * @return
 */
Interaction SystemeMotivationnel::interaction(const Experience& e, const Resultat& r) const
{
    return m_systeme[qMakePair(e,r)];
}

/**
 * @brief SystemeMotivationnel::add
 * @param e
 * @param r
 * @param m
 */
void SystemeMotivationnel::add(const Experience* e, const Resultat* r, int m)
{
    m_systeme[qMakePair(*e,*r)] = Interaction(e,r,m);
}

/**
 * @brief SystemeMotivationnel::affichage
 */
void SystemeMotivationnel::affichage() const
{
    qDebug()<<"Systeme Motivationel";
    foreach(const Interaction& it, m_systeme)
    {
        it.affichage();
    }
}

/**
 * @brief SystemeMotivationnel::systeme
 * @return
 */
QMap<QPair<const Experience, const Resultat>, Interaction> SystemeMotivationnel::systeme() const
{
    return m_systeme;
}

/**
 * @brief SystemeMotivationnel::first
 * @return
 */
Interaction SystemeMotivationnel::first() const
{
    return m_systeme.first();
}

/**
 * @brief SystemeMotivationnel::exp
 * @param r
 * @return
 */
Experience SystemeMotivationnel::exp(const Resultat & r) const
{
    Interaction ret = m_systeme.first();

    foreach (const Interaction& it, m_systeme)
    {
        if(r == it.resultat() && ret.motivation() < it.motivation())
        {
            ret = it;
        }
    }

    return ret.experience();
}

/**
 * @brief SystemeMotivationnel1::SystemeMotivationnel1
 */
SystemeMotivationnel1::SystemeMotivationnel1()
{
    add(new Experience(1), new Resultat(1), 1);
    add(new Experience(1), new Resultat(2), 1);
    add(new Experience(2), new Resultat(1), -1);
    add(new Experience(2), new Resultat(2), -1);
}

/**
 * @brief SystemeMotivationnel2::SystemeMotivationnel2
 */
SystemeMotivationnel2::SystemeMotivationnel2()
{
    add(new Experience(1), new Resultat(1), -1);
    add(new Experience(1), new Resultat(2), -1);
    add(new Experience(2), new Resultat(1), 1);
    add(new Experience(2), new Resultat(2), 1);
}

/**
 * @brief SystemeMotivationnel3::SystemeMotivationnel3
 */
SystemeMotivationnel3::SystemeMotivationnel3()
{
    add(new Experience(1), new Resultat(2), -1);
    add(new Experience(1), new Resultat(1), 1);
    add(new Experience(2), new Resultat(1), 1);
    add(new Experience(2), new Resultat(2), -1);
}
