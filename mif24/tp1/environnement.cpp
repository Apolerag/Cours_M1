#include "environnement.h"

/**
 * @brief Environnement::Environnement
 * @param nbExp
 * @param nbRes
 */
Environnement::Environnement(int nbExp, int nbRes)
{
    for(int i = 1; i <= nbExp; i ++)
    {
        m_experience.insert(i, Experience(i));
    }

    for(int i = 1; i <= nbRes; i ++)
    {
        m_resultat.insert(i, Resultat(i));
    }
}

/**
 * @brief Environnement::result
 * @param e
 * @return
 */
const Resultat& Environnement::result(const Experience &e) const
{
    QList<QPair<int, int>>::const_iterator it;
    for (it = m_liste.begin(); it != m_liste.end(); it ++)
    {
        if((*it).first == e.num())
        {
            return m_resultat[(*it).second];
        }
    }

    throw QString("Couple non trouver");
}

/**
 * @brief Environnement::addPair
 * @param e
 * @param r
 */
void Environnement::addPair(int e, int r)
{
    m_liste.push_back(qMakePair(e, r));
}

/**
 * @brief Environnement::resultat
 * @param num
 * @return
 */
const Resultat& Environnement::resultat(int num) const
{
    if(m_resultat.contains(num))
    {
        return m_resultat[num];
    }

    throw "Le resultat " + QString::number(num) + "n'existe pas";
}

/**
 * @brief Environnement::experience
 * @param num
 * @return
 */
const Experience& Environnement::experience(int num) const
{
    if(m_experience.contains(num))
    {
        return m_experience[num];
    }

    throw "L'experience " + QString::number(num) + "n'existe pas";
}

/**
 * @brief Environnement::experience
 * @return
 */
const QMap<int, Experience>& Environnement::experience () const
{
    return m_experience;
}

/**
 * @brief Environnement::affichage
 */
void Environnement::affichage() const
{
    qDebug()<<"Affichage environnement";
    qDebug()<<"Experience";
    foreach (const Experience& it, m_experience)
    {
        it.affichage();
    }

    qDebug()<<"Resultat";
    foreach (const Resultat& it, m_resultat)
    {
        it.affichage();
    }

    qDebug()<<"List";
    QPair<int, int> it;
    foreach(it, m_liste)
    {
        qDebug()<<it.first<<", "<<it.second;
    }
}

/**
 * @brief Environnement1::Environnement1
 */
Environnement1::Environnement1(): Environnement(2, 2)
{
    addPair(1, 1);
    addPair(2, 2);
}

/**
 * @brief Environnement2::Environnement2
 */
Environnement2::Environnement2(): Environnement(2,2)
{
    addPair(1, 2);
    addPair(2, 1);
}

