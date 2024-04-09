#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

#include "lifeform.h"
#include "simulation.h"
#include "shape.h"
#include "constantes.h"



/*=================PROTOTYPE FONCTIONS====================================*/
void build(vector<double> &liste_donnee, int etat, bool &epsil);
Lifeform build_lifeforme(vector<double> const &liste);
void build_branches(vector<Segment> &seg, vector<double> const &liste);
void start( vector<Algue> &liste_al, vector<Corail> &liste_co,
            vector<Scavenger> &liste_sc, int x);
void simulate(bool &epsil);
void liste_creator(string line, vector<double> &liste_donnee);

void decode(string line, bool &epsil);
void liste_creator(string line, vector<double> &liste_donnee);

/*================================FONCTIONS======================================*/
void build(vector<double> &liste_donnee, int etat, bool &epsil)
{
    static vector<Algue> liste_algue;
    static vector<Corail> liste_corail;
    static vector<Scavenger> liste_scavenger;
    switch (etat)
    {
    case 1:
    { // INI algue
        check_pre_build(liste_donnee, etat);
        Algue x(build_lifeforme(liste_donnee));
        liste_algue.push_back(x);
    }
    break;
    case 3:
    { // INI corail+
        check_pre_build(liste_donnee, etat);
        vector<Segment> seg;
        build_branches(seg, liste_donnee);
        Corail y(build_lifeforme(liste_donnee), liste_donnee[3], liste_donnee[4],
                 liste_donnee[5], liste_donnee[6], liste_donnee[7], seg);
        liste_corail.push_back(y);
    }
    break;
    case 6:
    { // INI scavenger
        check_pre_build(liste_donnee, etat);
        Scavenger z(build_lifeforme(liste_donnee), liste_donnee[3],
                    liste_donnee[4], liste_donnee[5]);
        liste_scavenger.push_back(z);
    }
    break;
    case 7:
    { // finnish
        start(liste_algue, liste_corail, liste_scavenger, 0);
        simulate(epsil);
    }
    break;
    }
}

Lifeform build_lifeforme(vector<double> const &liste)
{
    return Lifeform(S2d{liste[0], liste[1]}, liste[2]);
}

void build_branches(vector<Segment> &seg, vector<double> const &liste)
{
    Segment x(S2d{liste[0], liste[1]}, liste[8], liste[9]);
    seg.push_back(x);
    int n = 10;
    for (unsigned i(1); i < liste[7]; i++)
    {
        Segment y(seg[i - 1].getpoint_bout(), liste[n], liste[n + 1]);
        seg.push_back(y);
        n += 2;
    }
}
void start( vector<Algue> &liste_al, 
            vector<Corail> &liste_co, 
            vector<Scavenger> &liste_sc, int x)
{
    static vector<Algue> liste_algue;
    static vector<Corail> liste_corail;
    static vector<Scavenger> liste_scavenger;
    switch (x)
    {
    case 0:
        liste_algue = liste_al;
        liste_corail = liste_co;
        liste_scavenger = liste_sc;
        break;
    case 1:
        liste_al = liste_algue;
        liste_co = liste_corail;
        liste_sc = liste_scavenger;
        break;
    }
}

void simulate(bool &epsil)
{
    static vector<Algue> liste_algue;
    static vector<Corail> liste_corail;
    static vector<Scavenger> liste_scavenger;
    start(liste_algue, liste_corail, liste_scavenger, 1);
    check_post_build(liste_corail, liste_scavenger, epsil);
}

/*=========================LECTURE DE FICHIERS=========================*/


void decode(string line, bool &epsil)
{
    istringstream data(line);
    enum etat_lecture{NB_al,INI_al,NB_co,INI_co,INI_co_br,NB_sc,INI_sc,FIN};
    static vector<double> liste_donnee;
    static int i(0), total(0), j(0), total_br(0), etat(NB_al);
    switch (etat)
    {
    case NB_al:
        data >> total;
        etat = INI_al;
        break;
    case INI_al:
        liste_creator(line, liste_donnee);
        build(liste_donnee, etat, epsil);
        liste_donnee.clear();
        ++i;
        if (i == total)
        {
            total = 0;
            i = 0;
            etat = NB_co;
        }
        break;
    case NB_co:
        data >> total;
        etat = INI_co;
        break;
    case INI_co:
        liste_creator(line, liste_donnee);
        total_br = liste_donnee.back();
        etat = INI_co_br;
        if (i == total)
        {
            total = 0;
            i = 0;
            etat = NB_sc;
        }
        ++i;
        break;
    case INI_co_br:
        liste_creator(line, liste_donnee);
        ++j;
        if (j == total_br)
        {
            etat = INI_co;
            build(liste_donnee, etat, epsil);
            j = 0;
            total_br = 0;
            liste_donnee.clear();
            if (i == total)
            {
                etat = NB_sc;
                total = 0;
                i = 0;
            }
        }
        break;
    case NB_sc:
        data >> total;
        etat = INI_sc;
        break;
    case INI_sc:
        liste_creator(line, liste_donnee);
        build(liste_donnee, etat, epsil);
        liste_donnee.clear();
        ++i;
        if (i == total)
        {
            total = 0;
            i = 0;
            etat = FIN;
            build(liste_donnee, etat, epsil);
            liste_donnee.clear();
        }
        break;
    }
}
void liste_creator(string line, vector<double> &liste_donnee)
{
    istringstream data(line);
    double x;
    while (data >> x)
    {
        liste_donnee.push_back(x);
    }
}
/*===================================PROTO CLASSE================================*/
    void Simulation::lecture(char *nomfichier, bool epsil)
    {
          string line;
    ifstream fichier(nomfichier);
    if (!fichier.fail())
    {

        while (getline(fichier >> ws, line))
        {
            if (line[0] == '#')
            {
                continue;
            }
            decode(line, epsil);
        }
    }
    }
