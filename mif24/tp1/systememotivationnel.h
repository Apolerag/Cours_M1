#ifndef SYSTEMEMOTIVATIONNEL_H
#define SYSTEMEMOTIVATIONNEL_H

#include "interaction.h"

class SystemeMotivationnel
{
private:
    QMap<QPair<const Experience, const Resultat>, Interaction> m_systeme;

protected:

public:
    SystemeMotivationnel();
    Interaction first() const;
    QMap<QPair<const Experience, const Resultat>, Interaction> systeme() const;
    Experience exp(const Resultat&) const;
    Interaction interaction(const Experience&, const Resultat&) const;
    void add(const Experience*, const Resultat*, int);
    void affichage() const;
};



class SystemeMotivationnel1: public SystemeMotivationnel
{
public:
    SystemeMotivationnel1();
};

class SystemeMotivationnel2: public SystemeMotivationnel
{
public:
    SystemeMotivationnel2();
};

class SystemeMotivationnel3: public SystemeMotivationnel
{
public:
    SystemeMotivationnel3();
};

#endif // SYSTEMEMOTIVATIONNEL_H
