
#include "agent.h"
#include "systememotivationnel.h"
#include "environnement.h"

int main(int, char**)
{
    Environnement* env = new Environnement1;
    SystemeMotivationnel* systemeBase = new SystemeMotivationnel1;
    Agent a(*env);

    for(int i = 0; i < 100; i ++)
    {
        Resultat r = a.chooseResult();
        Experience e = a.chooseExperience(r);
        Resultat rt = env->result(e);
        Interaction it = systemeBase->interaction(e, rt);
        a.addMotivation(it);
    }

    delete env;
    delete systemeBase;
    return 0;
}
