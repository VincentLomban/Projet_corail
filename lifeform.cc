#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#include "lifeform.h"
#include "message.h"
#include "shape.h"
#include "constantes.h"

/*====================TYPEDEF===============================*/

typedef vector<double> vec_double;

/*====================PROTOTYPE DE FONCTIONS===============================*/

void check_age(vec_double &lst);
void check_position(vec_double &lst);
void check_longueur_segment(vec_double &lst);
void check_angle(vec_double &lst);
void check_rayon_sca(vec_double &lst);
void check_id_unique(vector<Corail> &lst_corail);
void check_id_existant(vector<Corail> &lst_corail, vector<Scavenger> &lst_scavenger);
void check_segment_collision(vector<Corail> &lst_corail, bool& epsil);
void check_segment_superposition(vector<Corail> &lst_corail, bool& epsil);
void check_bout_dehors(vec_double &lst);
/*===========================FONCTIONS==========================================*/

void check_pre_build(vec_double &lst, int &etat)
{
    check_age(lst);
    check_position(lst);
    switch (etat)
    {
    case 3:
        check_bout_dehors(lst);
        check_longueur_segment(lst);
        check_angle(lst);

        break;

    case 6:
        check_rayon_sca(lst);
        break;
    }
}

void check_post_build(vector<Corail> &lst_corail,
 vector<Scavenger> &lst_scavenger, bool& epsil)
{
    check_id_unique(lst_corail);
    check_id_existant(lst_corail, lst_scavenger);
    check_segment_collision(lst_corail, epsil);
    check_segment_superposition(lst_corail, epsil);
    cout << message::success();
    exit(EXIT_SUCCESS);
}
/*=======================FONCTIONS CHECK======================================*/

// Computed outside à utiliser

void check_age(vec_double &lst)
{
    if (lst[2] <= 0)
    {
        cout << message::lifeform_age(lst[2]);
        exit(EXIT_FAILURE);
    }
}

void check_position(vec_double &lst)
{
    if ((lst[0] < 1) or (lst[0] > dmax - 1) or (lst[1] < 1) or (lst[1] > dmax - 1))
    {
        cout << message::lifeform_center_outside(lst[0], lst[1]);
        exit(EXIT_FAILURE);
    }
}

void check_bout_dehors(vec_double &lst)
{
    double x(lst[0]), y(lst[1]);

    for (unsigned i(0); i < lst[7]; ++i)
    {
        x += cos(lst[8 + 2 * i]) * lst[9 + 2 * i];
        y += sin(lst[8 + 2 * i]) * lst[9 + 2 * i];

        if ((x <= 0) or (x >= dmax) or (y <= 0) or (y >= dmax))
        {
            cout << message::lifeform_computed_outside(lst[3], x, y);
            exit(EXIT_FAILURE);
        }
    }
}

void check_longueur_segment(vec_double &lst)
{
    for (unsigned i(0); i < lst[7]; ++i)
    {
        // il nous faut vérifier pour tous les segments dans la liste de donnée
        if ((lst[9 + 2 * i] < (l_repro - l_seg_interne)) or
         (lst[9 + 2 * i] >= l_repro))
        {
            cout << message::segment_length_outside(lst[3], lst[9 + 2 * i]);
            exit(EXIT_FAILURE);
        }
    }
}

void check_angle(vec_double &lst)
{
    for (unsigned i(0); i < lst[7]; ++i)
    {
        if ((lst[8 + 2 * i] < -M_PI) or (lst[8 + 2 * i] > M_PI))
        {
            cout << message::segment_angle_outside(lst[3], lst[8 + 2 * i]);
            exit(EXIT_FAILURE);
        }
    }
}

void check_rayon_sca(vec_double &lst)
{
    if ((lst[3] < r_sca) or (lst[3] >= r_sca_repro))
    {
        cout << message::scavenger_radius_outside(lst[3]);
        exit(EXIT_FAILURE);
    }
}

/*===============CHECK APRÈS BUILD=======================================*/

void check_id_unique(vector<Corail> &lst_corail)
{
    for (unsigned i(0); i < lst_corail.size() - 1; ++i)
    {
        for (unsigned j(i + 1); j < lst_corail.size(); ++j)
        {
            if (lst_corail[i].get_identifiant() == lst_corail[j].get_identifiant())
            {
                cout << message::lifeform_duplicated_id
                (lst_corail[i].get_identifiant());
                exit(EXIT_FAILURE);
            }
        }
    }
}

void check_id_existant(vector<Corail> &lst_corail, vector<Scavenger> &lst_scavenger)
{
    // si la cible est 0 pour le scavenger, il ne doit pas être inclu dans le check
    //il faut que le test soit effectué seulement s'il mange
    for (unsigned i(0); i < lst_scavenger.size(); ++i)
    {
        if(lst_scavenger[i].getmange_free())
        {
            bool verif(true);
            if (lst_scavenger[i].getcibleid() == 0) continue;
            for (unsigned j(0); j < lst_corail.size(); ++j)
            {
                if (lst_scavenger[i].getcibleid() == lst_corail[j].get_identifiant())
                {
                    verif = false;
                }
            }
            if (verif)
            {
                cout << message::lifeform_invalid_id(lst_scavenger[i].getcibleid());
                exit(EXIT_FAILURE);
            }
        }
    }
}

void check_segment_collision(vector<Corail> &lst_corail, bool& epsil)
{
    for (unsigned i1(0); i1 < lst_corail.size() - 1; ++i1)
    {
        for (unsigned i2(0); i2 < lst_corail[i1].getbranches().size(); ++i2)
        {
            for (unsigned j1(i1 + 1); j1 < lst_corail.size(); ++j1)
            {
                for (unsigned j2(0); j2 < lst_corail[j1].getbranches().size(); ++j2)
                {
                    if (superposition_2_segments(lst_corail[i1].getbranches()[i2],
                     lst_corail[j1].getbranches()[j2], epsil))
                    {
                        cout << message::segment_collision(lst_corail[i1]
                        .get_identifiant(), i2, lst_corail[j1].get_identifiant(), j2);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
}

void check_segment_superposition(vector<Corail> &lst_corail, bool& epsil)
{
    for (unsigned i(0); i < lst_corail.size(); ++i)
    {
        if (lst_corail[i].getbranches().size() == 1)
            continue;

        for (unsigned j1(0); j1 < lst_corail[i].getbranches().size() - 1; ++j1)
        {
            for (unsigned j2(j1 + 1); j2 < lst_corail[i].getbranches().size(); ++j2)
            {
                if ((j2 - j1) == 1)
                {
                    double a1, a2;
                    a1 = lst_corail[i].getbranches()[j1].getangle();
                    a2 = lst_corail[i].getbranches()[j2].getangle();
                    if (ecart_angulaire(a1, a2) == 0)
                    {
                        cout << message::segment_superposition(lst_corail[i]
                        .get_identifiant(), j1, j2);
                        exit(EXIT_FAILURE);
                    }
                }

                if (superposition_2_segments(lst_corail[i].getbranches()[j1],
                 lst_corail[i].getbranches()[j2], epsil))
                {
                    cout << message::segment_collision(lst_corail[i].get_identifiant(),
                     j1, lst_corail[i].get_identifiant(), j2);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

/*======================FONCTIONS CLASS====================*/

Lifeform::Lifeform(S2d point, double age)
    : position(point), age(age) {}
void Lifeform::setposition(S2d x) { position = x; };

void Lifeform::setage(double x) { age = x; };

S2d Lifeform::getposition() const { return position; };

double Lifeform::getage() const { return age; };

/*===========================ALGUE=====================================*/
Algue::Algue(Lifeform entite)
    : entite(entite) {}

void Algue::setentite_age(double &x) { entite.setage(x); };

double Algue::getentite_age() const { return entite.getage(); };
S2d Algue::getentite_position() const { return entite.getposition(); };

/*===================================CORAIL=========================================*/
Corail::Corail(Lifeform entite, int identifiant, int statut, bool sens_rot,
 bool repro_extend, int nb_segment, vector<Segment> branches)
    : entite(entite), identifiant(identifiant), statut(statut), sens_rot(sens_rot),
     repro_extend(repro_extend), nb_segment(nb_segment), branches(branches)
{
}
// SETTER

void Corail::setentite_age(double &x) { entite.setage(x); };
void Corail::setstatut(int &x) { statut = x; };
void Corail::setsens_rot(bool &x) { sens_rot = x; };
void Corail::setrepro_extend(bool &x) { repro_extend = x; };
void Corail::setnb_segment(int &x) { nb_segment = x; };
void Corail::setbranches(vector<Segment> &x) { branches = x; };
// GETTER

S2d Corail::getentite_position() const { return entite.getposition(); };
double Corail::getentite_age() const { return entite.getage(); };
int Corail::getstatut() const { return statut; };
bool Corail::getsens_rot() const { return sens_rot; };
bool Corail::getrepro_extend() const { return repro_extend; };
int Corail::get_identifiant() const { return identifiant; };
vector<Segment> Corail::getbranches() const { return branches; };

/*=================SCAVENGER====================*/
Scavenger::Scavenger(Lifeform entite, double rayon, bool mange_free,
 int corail_id_cible)
    : entite(entite), rayon(rayon), mange_free(mange_free),
     corail_id_cible(corail_id_cible) {}
// SETTER

void Scavenger::setentite_position(S2d &x) { entite.setposition(x); };
void Scavenger::setentite_age(double &x) { entite.setage(x); };
void Scavenger::setmange_free(bool &x) { mange_free = x; };
void Scavenger::setrayon(double &x) { rayon = x; };
void Scavenger::setcibleid(int &x) { corail_id_cible = x; };
// GETTER

S2d Scavenger::getentite_position() const { return entite.getposition(); };
double Scavenger::getentite_age() const { return entite.getage(); };
bool Scavenger::getmange_free() const { return mange_free; };
double Scavenger::getrayon() const { return rayon; };
int Scavenger::getcibleid() const { return corail_id_cible; };
