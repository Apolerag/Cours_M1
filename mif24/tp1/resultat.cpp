#include "resultat.h"

QSet<int> Resultat::m_nbRes = QSet<int>();

/**
 * @brief Resultat::Resultat
 * @param num
 */
Resultat::Resultat(int num): m_num(num)
{
    m_nbRes<<num;
}

/**
 * @brief Resultat::Resultat
 * @param r
 */
Resultat::Resultat(const Resultat & r): m_num(r.num()) {}

/**
 * @brief Resultat::~Resultat
 */
Resultat::~Resultat()
{
}

/**
 * @brief Resultat::num
 * @return
 */
int Resultat::num() const
{
    return m_num;
}

/**
 * @brief Resultat::affichage
 */
void Resultat::affichage() const
{
    qDebug()<<"Resultat "<<num();
}

/**
 * @brief Resultat::operator <
 * @param r
 * @return
 */
bool Resultat::operator <(const Resultat& r) const
{
    return num() < r.num();
}

/**
 * @brief Resultat::operator ==
 * @param r
 * @return
 */
bool Resultat::operator == (const Resultat& r) const
{
    return num() == r.num();
}

/**
 * @brief Resultat::operator =
 * @param r
 * @return
 */
Resultat& Resultat::operator =(const Resultat& r)
{
    m_num = r.num();

    return *this;
}

/**
 * @brief qHash
 * @param r
 * @return
 */
uint qHash(const Resultat& r)
{
    return r.num();
}
