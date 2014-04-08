#include "experience.h"

QSet<int> Experience::m_nbExp = QSet<int>();

/**
 * @brief Experience::Experience
 * @param num
 */
Experience::Experience(int num): m_num(num)
{
    m_nbExp<<num;
}

/**
 * @brief Experience::Experience
 * @param e
 */
Experience::Experience(const Experience & e): m_num(e.num()) {}

/**
 * @brief Experience::~Experience
 */
Experience::~Experience()
{
}

/**
 * @brief Experience::num
 * @return
 */
int Experience::num() const
{
    return m_num;
}

/**
 * @brief Experience::affichage
 */
void Experience::affichage() const
{
    qDebug()<<"Experience "<<num();
}

/**
 * @brief Experience::operator <
 * @param e
 * @return
 */
bool Experience::operator <(const Experience& e) const
{
    return num() < e.num();
}

/**
 * @brief Experience::operator ==
 * @param e
 * @return
 */
bool Experience::operator == (const Experience& e) const
{
    return num() == e.num();
}

/**
 * @brief Experience::operator =
 * @param e
 * @return
 */
Experience& Experience::operator =(const Experience& e)
{
    m_num = e.num();

    return *this;
}

/**
 * @brief qHash
 * @param e
 * @return
 */
uint qHash(const Experience& e)
{
    return e.num();
}

