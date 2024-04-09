#ifndef LIFEFORM
#define LIFEFORM

#include "message.h"
#include "shape.h"
#include "constantes.h"

typedef vector<double> vec_double;



class Lifeform
{
private:
    S2d position;
    double age;

public:
    // constructeur par défaut
    Lifeform(S2d point, double age = 1);
    // setter
    void setposition(S2d x);
    void setage(double x);
    // getter

    S2d getposition() const ;
    double getage() const ;
};
// on doit pouvoir récupérer sa position et son age
class Algue
{
private:
    Lifeform entite;

public:
    // constructeur

    Algue(Lifeform entite);

    // il n'y a pas besoin de setter pour sa position
    // vu que celui ci ne se met pas à jour
    // setter
    void setentite_age(double& x) ;
    // à se demander s'il est nécessaire de connaître la position à chaque insant
    // getter
    double getentite_age() const;
    S2d getentite_position() const;
};

// le corail doit contenir plusieurs segments, 
// donc on écrit une liste de plusieurs segments, 
// soit on écrit une liste de points et un segment
class Corail
{
private:
    Lifeform entite;
    int statut;         // dead = 0, alive = 1, en train de se faire bouffer = 2
    int identifiant;
    bool sens_rot;     // trigo = 0, invtrigo = 1
    bool repro_extend; // repro = 1, extend = 0
    int nb_segment;


    vector<Segment> branches;

public:
    // constructeur
    Corail(Lifeform entite, int identifiant, int statut, bool sens_rot,
     bool repro_extend, int nb_segment, vector<Segment> branches);

    // setter
    // La position ne change pas au cours du temps, pas besoin de setter pour ça
    void setentite_age(double& x) ;
    void setstatut(int& x) ;
    void setsens_rot(bool& x) ;
    void setrepro_extend(bool& x);
    void setnb_segment(int& x) ;
    void setbranches(vector<Segment>& x);


    // getter

    S2d getentite_position() const;
    double getentite_age() const ;

    int getstatut() const;
    bool getsens_rot() const ;
    bool getrepro_extend() const ;
    int  get_identifiant() const ;

    vector<Segment> getbranches() const ;

};

class Scavenger
{
private:
    Lifeform entite;
    bool mange_free;  //mange = 1, free = 0
    int corail_id_cible;
    double rayon;

public:
    // constructeur
    Scavenger(Lifeform entite, double rayon, bool mange_free, int corail_id_cible = 0);

    // setter
    void setentite_position(S2d &x);
    void setentite_age(double &x) ;
    void setmange_free(bool &x);
    void setrayon(double &x) ;
    void setcibleid(int& x);



    // getter
    S2d getentite_position() const ;
    double getentite_age() const ;
    bool getmange_free() const ;
    double getrayon() const ;
    int getcibleid() const ;
};


/*==========================PROTOTYPE FONCTIONS========================*/
void check_pre_build(vec_double &lst, int &etat);
void check_post_build(vector<Corail> &lst_corail, vector<Scavenger> &lst_scavenger,
 bool& epsil);

#endif