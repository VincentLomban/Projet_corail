#include <iostream>
#include <cmath>

using namespace std;

#include "shape.h"

// variables globales
constexpr double epsil_zero(0.5);
// Il faut créer les classes segments, carrés et cercles

// Fera partie de l'interface


// Prototype des fonctions
int orientation(S2d &p, S2d &q, S2d &r, bool& epsil);
bool onSegment(S2d &p, S2d &q, S2d &r, bool& epsil);

/*====================FONCTIONS=====================================*/
double ecart_angulaire(double &angle_1, double &angle_2)
{
    double ecart(M_PI - angle_2 + angle_1);

    // correcteur d'angle pour l'intervalle
    if (ecart > M_PI)
    {
        ecart -= 2 * M_PI;
    }
    else if (ecart < -M_PI)
    {
        ecart += 2 * M_PI;
    }

    return ecart;
}

//2 segments
bool superposition_point_commun(Segment &ligne1, Segment &ligne2, bool &epsil) 
{
    S2d p(ligne1.getpoint()), q(ligne1.getpoint_bout()), r(ligne2.getpoint_bout());
    if ((orientation(p, q, r, epsil) == 0) and (onSegment(p, q, r, epsil)))
    {
        return true;
    }

    return false;
}

// Rien qu'avec le nom, on comprend qu'il faut deux instances de segment.

bool superposition_2_segments(Segment &ligne1, Segment &ligne2, bool &epsil)
{
    S2d l1_1 = ligne1.getpoint();
    S2d l1_2 = ligne1.getpoint_bout();
    S2d l2_1 = ligne2.getpoint();
    S2d l2_2 = ligne2.getpoint_bout();
    int o1(orientation(l1_1, l1_2, l2_1, epsil)),
        o2(orientation(l1_1, l1_2, l2_2, epsil)),
        o3(orientation(l2_1, l2_2, l1_1, epsil)),
        o4(orientation(l2_1, l2_2, l1_2, epsil));

    if (!((l1_2 == l2_1)))
    {
        if (o1 != o2 && o3 != o4)
        {
            return true;
        }
        if (o1 == 0 && onSegment(l1_1, l1_2, l2_1, epsil))
        {
            return true;
        }
        if (o2 == 0 && onSegment(l1_1, l2_2, l2_1, epsil))
        {
            return true;
        }
        if (o3 == 0 && onSegment(l1_2, l1_1, l2_2, epsil))
        {
            return true;
        }
        if (o4 == 0 && onSegment(l1_2, l2_1, l2_2, epsil))
        {
            return true;
        }
    }

    return false;
}

// Cette fonction s'occupe de vérifier si le point r fait partie de 
//l'espace créé par le segment pq de manière parrallèle
int orientation(S2d &p, S2d &q, S2d &r, bool &epsil)
{
    double val = (q.coor_y - p.coor_y) * (r.coor_x - q.coor_x) - (q.coor_x - p.coor_x)
     * (r.coor_y - q.coor_y);

    val = val / sqrt((p.coor_x - q.coor_x) * (p.coor_x - q.coor_x) + 
    (p.coor_y - q.coor_y) * (p.coor_y - q.coor_y));
    // Il faut vérifier les deux cotés, c'est pour ça le -epsil_zero
    if(epsil)
    {
    if ((val < epsil_zero) and (val > -epsil_zero)) return 0;
    }
    else
    {
        if((val < 0)and (val > 0)) return 0; 
    }

    return (val > 0) ? 1 : 2;
}

// Cette fonction s'occupe de vérifier si le point r fait partie de 
//l'espace créé par le segment pq de manière perpendiculaire.
bool onSegment(S2d &p, S2d &q, S2d &r, bool &epsil)
{
    // on doit calculer le produit scalaire entre pq et pr,
    // que l'on définit par s. puis la valeur de la projection 
    //de x de pq sur pr vaut s diviser par la norme de pr.
    // la fonction à satisfaire est  : -epsil_zero <= x <= norme de pr + epsil_zero

    // pour calculer le vecteur pr, on peut prendre les coordoones du deuxième point ,
    //puis soustraire à chaque coordonnées les données du premier point
    // même si c'est un abus de langage, on utilisera la classe s2d 
    //pour la définition du vecteur

    S2d pr(r - p), pq(q - p);

    // la fonction scalaire retourne un double

    double s(pr.coor_x * pq.coor_x + pr.coor_y * pq.coor_y);

    // la norme de pr s'écrit par pythagore
    double n_pr(sqrt(pr.coor_x * pr.coor_x + pr.coor_y * pr.coor_y));

    // on pose désormais notre x

    double x(s / n_pr);

    // maintenant la condition
    if (epsil)
    {
        if ((x >= -epsil_zero) and (x <= n_pr + epsil_zero)) return true;
    }
    else
    {
        if ((x >= 0) and (x <= n_pr)) return true;
    }
    return false;
}

/*=====================================PROTO CLASSE=======================*/

S2d &S2d::operator-=(S2d &z)
{
    this->coor_x -= z.coor_x;
    this->coor_y -= z.coor_y;
    return *this;
}
S2d &S2d::operator-(S2d &z)
{
    *this -= z;
    return *this;
}

bool S2d::operator==(S2d &z)
{
    if ((this->coor_x - z.coor_x == 0) and (this->coor_y - z.coor_y == 0))
        return true;
    return false;
}

// constructeur
Segment::Segment(S2d x, double y, double z)
    : point(x), angle(y), longueur(z)
{
}

// getter
S2d Segment::getpoint() const { return point; };
S2d Segment::getpoint_bout() const
{
    S2d temp;

    temp.coor_x = point.coor_x + longueur * cos(angle);
    temp.coor_y = point.coor_y + longueur * sin(angle);

    return temp;
}
double Segment::getangle() const { return angle; };
double Segment::getlongueur() const { return longueur; };

// setter
void Segment::setpoint(S2d &x) { point = x; };
void Segment::setangle(double &x)
{
    if (x < -M_PI)
    {
        angle = -M_PI;
    }
    else if (x > M_PI)
    {
        angle = M_PI;
    }
    else
    {
        angle = x;
    }
}
void Segment::setlongueur(double &x)
{
    if (x < 0)
    {

        longueur = 0;
    }
    else
    {
        longueur = x;
    }
}
// cercle
Cercle::Cercle(S2d x, double y)
    : point(x), rayon(y) {}

S2d Cercle::getpoint() const { return point; };
double Cercle::getrayon() const { return rayon; };

void Cercle::setpoint(S2d &x) { point = x; };

void Cercle::setrayon(double &x)
{
    if (x < 0)
    {

        rayon = 0;
    }
    else
    {
        rayon = x;
    }
}

Carre::Carre(S2d x, double y)
    : point(x), cote(y) {}

S2d Carre::getpoint() const { return point; };
double Carre::getcote() const { return cote; };

void Carre::setpoint(S2d &x) { point = x; };

void Carre::setcote(double &x)
{
    if (x < 0)
    {

        cote = 0;
    }
    else
    {
        cote = x;
    }
}