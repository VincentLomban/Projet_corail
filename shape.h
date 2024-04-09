#ifndef SHAPE
#define SHAPE

#include "constantes.h"


struct S2d
{
    double coor_x;
    double coor_y;
    S2d& operator-=(S2d& z);
    S2d& operator-(S2d& z);
    bool operator==(S2d& z);


};

class Segment
{
private:
    S2d point;
    double angle;
    double longueur;

public:

    //constructeur
    Segment(S2d x, double y, double z);  
    // getter
    S2d getpoint() const;
    S2d getpoint_bout() const;
   
    double getangle() const ;
    double getlongueur() const;

    // setter
    void setpoint(S2d &x);
    void setangle(double &x);
   
    void setlongueur(double &x);
};


class Cercle
{
private:
    S2d point;
    double rayon;

public:
    Cercle(S2d x, double y);
   
    S2d getpoint() const ;
    double getrayon() const ;

    void setpoint(S2d &x) ;

    void setrayon(double &x);
 
};

class Carre
{
private:
    S2d point;
    double cote;

public:
    Carre(S2d x, double y);
   
    S2d getpoint() const ;
    double getcote() const ;

    void setpoint(S2d &x);

    void setcote(double &x);
  
};


double ecart_angulaire(double &angle_1, double &angle_2);
bool superposition_point_commun(Segment& ligne1, Segment& ligne2, bool& epsil);
bool superposition_2_segments(Segment& ligne1, Segment&  ligne2, bool& epsil);


#endif