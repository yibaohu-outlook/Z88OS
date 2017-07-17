/************************************************************************
* 
*               *****   ***    ***
*                  *   *   *  *   *
*                 *     ***    ***
*                *     *   *  *   *
*               *****   ***    ***
*
* A FREE Finite Elements Analysis Program in ANSI C for the Windows
* and UNIX OS.
*
* Composed and edited and copyright by 
* Professor Dr.-Ing. Frank Rieg, University of Bayreuth, Germany
*
* eMail: 
* frank.rieg@uni-bayreuth.de
* dr.frank.rieg@t-online.de
* 
* V15.0 November 18, 2015
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; see the file COPYING.  If not, write to
* the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
***********************************************************************/ 
/***********************************************************************
* diese Compilerunit umfasst: shad88 - Elementsteifigkeitsroutine
*                             dd88   - Berechnung der Matrix b
* diese Compilerunit enthaelt Routinen, die gedanklich an FORTRAN-
* Quellen von H.J.Bathe, MIT, Cambridge, MA, USA angelehnt sind.
* 17.7.2011 Rieg
***********************************************************************/

/***********************************************************************
* Fuer UNIX
***********************************************************************/
#ifdef FR_UNIX
#include <z88r.h>
#include <stdio.h>
#endif

/***********************************************************************
* Fuer Windows
***********************************************************************/
#ifdef FR_WIN
#include <z88r.h>
#include <stdio.h>
#endif

/***********************************************************************
* Fuer Windows & GTK+
***********************************************************************/
#ifdef FR_GTKWIN
#include <z88r.h>
#include <stdio.h>
#endif

/***********************************************************************
*  Functions
***********************************************************************/
int dd88(FR_DOUBLE *det,FR_DOUBLE *r,FR_DOUBLE *s,FR_DOUBLE *t);

/***********************************************************************
* hier beginnt Function shad88
***********************************************************************/
int shad88(void)
{
extern FR_DOUBLEAY se;

extern FR_DOUBLE xk[],yk[],zk[];
extern FR_DOUBLE b[],xx[],d[];

extern FR_DOUBLE emode,rnuee;

extern FR_INT4 intore;

FR_DOUBLE db[7];

FR_DOUBLE r,s,t,det,wt,stiff;
FR_DOUBLE f0,f1,f2;

FR_INT4 ne= 36,i,igauss,lz,j,k,l,intor_t;

int iret;

/*----------------------------------------------------------------------
* Gauss-Legendre Stuetzstellen fuer r
*---------------------------------------------------------------------*/
static FR_DOUBLE rg[40]= 
{ 
0.,0.,0.,0.,0.,0.,0.,   /* Elemente 0 - 6 leer              */
0.1666666666667,        /* intore = 3, 1.Ele Start bei i=7  */
0.6666666666667,
0.1666666666667,
0.,0.,0.,0.,0.,         /* Elemente 10-14 leer              */
0.1012865073235,        /* intore = 7, 1.Ele Start bei i=15 */  
0.7974269853531,
0.1012865073235,
0.4701420641051,
0.4701420641051,
0.0597158717898,
0.3333333333333,
0.,0.,0.,0.,0.,         /* Elemente 22-26 leer              */
0.0651301029022,        /* intore =13, 1.Ele Start bei i=27 */
0.8697397941956,
0.0651301029022,
0.3128654960049,
0.6384441885698,
0.0486903154253,
0.6384441885698,
0.3128654960049,
0.0486903154253,
0.2603459660790,
0.4793080678419,
0.2603459660790,
0.3333333333333
};

/*----------------------------------------------------------------------
* Gauss-Legendre Stuetzstellen fuer s
*---------------------------------------------------------------------*/
static FR_DOUBLE sg[40]= 
{ 
0.,0.,0.,0.,0.,0.,0.,   /* Elemente 0 - 6 leer              */
0.1666666666667,        /* intore = 3, 1.Ele Start bei i=7  */
0.1666666666667,
0.6666666666667,
0.,0.,0.,0.,0.,         /* Elemente 10-14 leer              */
0.1012865073235,        /* intore = 7, 1.Ele Start bei i=15 */
0.1012865073235,
0.7974269853531,
0.0597158717898,
0.4701420641051,
0.4701420641051,
0.3333333333333,
0.,0.,0.,0.,0.,         /* Elemente 22-26 leer              */
0.0651301029022,        /* intore =13, 1.Ele Start bei i=27 */
0.0651301029022,
0.8697397941956,
0.0486903154253,
0.3128654960049,
0.6384441885698,
0.0486903154253,
0.6384441885698,
0.3128654960049,
0.2603459660790,
0.2603459660790,
0.4793080678419,
0.3333333333333
};

/*----------------------------------------------------------------------
* Gauss-Legendre Integrationsgewichte
*---------------------------------------------------------------------*/
static FR_DOUBLE wg[40]= 
{ 
0.,0.,0.,0.,0.,0.,0.,   /* Elemente 0 - 6 leer              */
0.3333333333333,        /* intore = 3, 1.Ele Start bei i=7  */
0.3333333333333,
0.3333333333333,
0.,0.,0.,0.,0.,         /* Elemente 10-14 leer              */
0.1259391805448,        /* intore = 7, 1.Ele Start bei i=15 */
0.1259391805448,
0.1259391805448,
0.1323941527885,
0.1323941527885,
0.1323941527885,
0.225,
0.,0.,0.,0.,0.,         /* Elemente 22-26 leer              */
0.0533472356088,        /* intore =13, 1.Ele Start bei i=27 */
0.0533472356088,
0.0533472356088,
0.0771137608903,
0.0771137608903,
0.0771137608903,
0.0771137608903,
0.0771137608903,
0.0771137608903,
0.1756152574332,
0.1756152574332,
0.1756152574332,
-0.1495700444677
};
  
/*----------------------------------------------------------------------
* Gauss-Legendre Stuetzstellen
*---------------------------------------------------------------------*/
static FR_DOUBLE xg[17]= { 0.,
   0., -.5773502691896, -.7745966692415, -.8611363115941,
   0., +.5773502691896,              0., -.3399810435849,
   0.,              0., +.7745966692415, +.3399810435849,
   0.,              0.,              0., +.8611363115941 };

/*----------------------------------------------------------------------
* Gauss-Legendre Integrationsgewichte
*---------------------------------------------------------------------*/
static FR_DOUBLE wgt[17]= { 0.,
   2.,              1., +.5555555555556, +.3478548451375,
   0.,              1., +.8888888888889, +.6521451548625,
   0.,              0., +.5555555555556, +.6521451548625,
   0.,              0.,              0., +.3478548451375 };

/*----------------------------------------------------------------------
* xk, yk und zk umspeichern
*---------------------------------------------------------------------*/
for(i = 1;i <= 12;i++)
  {
  xx[i]   = xk[i];
  xx[12+i]= yk[i];
  xx[24+i]= zk[i];
  }

/*----------------------------------------------------------------------
* Materialkonstanten
*---------------------------------------------------------------------*/
f0= emode*(1.-rnuee) / ((1.+rnuee)*(1.-2.*rnuee));

f1= rnuee/(1.-rnuee) * f0;
f2= (1.-2.*rnuee) / (2.*(1.-rnuee)) * f0;

/*----------------------------------------------------------------------
* Elastizitaetsmatrix aufstellen
*---------------------------------------------------------------------*/
for(i = 1;i <= 36;i++)
  d[i]= 0.;

d[1] = f0; 
d[7] = f1;
d[13]= f1; 
d[2] = f1;
d[8] = f0; 
d[14]= f1; 
d[3] = f1; 
d[9] = f1; 
d[15]= f0; 
d[22]= f2;
d[29]= f2;
d[36]= f2;

/*----------------------------------------------------------------------
* Elementsteifigkeitsmatrix aufstellen
*---------------------------------------------------------------------*/
intor_t= 2;

for(i = 1;i <= 1296;i++)  /* 36 x 36 */
  se[i]= 0.;

for(igauss = 1;igauss <= intore;igauss++)
  {
  r= rg[igauss+2*intore];
  s= sg[igauss+2*intore];
  for(lz = 1;lz <= intor_t;lz++)
    {
    t= xg[(lz-1)*4 + intor_t];
  
/*======================================================================
* Matrix b der partiellen Ableitungen & Jacobi Determinante holen
*=====================================================================*/
    iret= dd88(&det,&r,&s,&t);
    if(iret != 0) return(iret);
    
    wt= wg[igauss+2*intore]*0.5 * wgt[(lz-1)*4+intor_t] * det; 

    for(j = 1;j <= 36;j++)
      {
      for(k = 1;k <= 6;k++) 
        {
        db[k]= 0.;
        for(l = 1;l <= 6;l++) 
          {
          db[k]= db[k] + d[(k-1)*6 + l] * b[(l-1)*36 + j];
          } 
        }  
 
      for(i = j;i <= 36;i++) 
        {
        stiff= 0.;
        for(l = 1;l <= 6;l++) 
          {
          stiff+= b[(l-1)*36 + i] * db[l];
          }   
        se[i+ne*(j-1)]= se[i+ne*(j-1)] + stiff * wt;
        } 
      }
    } 
  } 
  
for(j = 1;j <= 36;j++)
  {  
  for(i = j;i <= 36;i++)
    {
    se[j+ne*(i-1)]= se[i+ne*(j-1)];
    } 
  } 

return(0);
}

/***********************************************************************
* hier beginnt Function dd88
***********************************************************************/
int dd88(FR_DOUBLE *det,FR_DOUBLE *r,FR_DOUBLE *s,FR_DOUBLE *t)
{
/*---------------------------------------------------------------------
* xx geht rein, unveraendert (ex)
* b  geht raus, neu (ex)
* det geht raus, neu
* r,s,t gehen rein, unveraendert
*--------------------------------------------------------------------*/

extern FR_DOUBLE b[],xx[],h[],p[];

FR_DOUBLE xj[10], xji[10];              /* ist 3x3 +1 */

FR_DOUBLE rr2,ss2,r4,r3,s4,s3,rs4,dum,ept,emt;
        
FR_INT4 i,j,k,k3;
        
/*----------------------------------------------------------------------
* Klammern der Formfunktionen belegen
*---------------------------------------------------------------------*/
rr2= 2. * (*r) * (*r);
ss2= 2. * (*s) * (*s);
r4 = 4. * (*r);
r3 = 3. * (*r);
s4 = 4. * (*s);
s3 = 3. * (*s);
rs4= 4. * (*r) * (*s);
ept= 0.5* (1. + (*t));
emt= 0.5* (1. - (*t));

/*----------------------------------------------------------------------
* Formfunktionen obere und untere Ebene
*---------------------------------------------------------------------*/
h[ 1]= (rr2 + ss2 + rs4 - r3 - s3 + 1.) * ept;
h[ 2]= (rr2 - (*r)) * ept;
h[ 3]= (ss2 - (*s)) * ept;
h[ 4]= (r4 - 2*rr2 - rs4) * ept;
h[ 5]= rs4 * ept;
h[ 6]= (s4 - 2*ss2 - rs4) * ept;

h[ 7]= (rr2 + ss2 + rs4 - r3 - s3 + 1.) * emt;
h[ 8]= (rr2 - (*r)) * emt;
h[ 9]= (ss2 - (*s)) * emt;
h[10]= (r4 - 2*rr2 - rs4) * emt;
h[11]= rs4 * emt;
h[12]= (s4 - 2*ss2 - rs4) * emt;

/*----------------------------------------------------------------------
* Partielle Ableitung der Formfunktionen nach r
*---------------------------------------------------------------------*/
p[ 1]= (r4 + s4 - 3.) * ept;
p[ 2]= (r4 - 1.) * ept;
p[ 3]= 0.;
p[ 4]= (4. - 8*(*r) -s4) * ept;
p[ 5]= s4 * ept;
p[ 6]= -s4 * ept;

p[ 7]= (r4 + s4 - 3.) * emt;
p[ 8]= (r4 - 1.) * emt;
p[ 9]= 0.;
p[10]= (4. - 8*(*r) -s4) * emt;
p[11]= s4 * emt;
p[12]= -s4 * emt;

/*----------------------------------------------------------------------
* Partielle Ableitung der Formfunktionen nach s
*---------------------------------------------------------------------*/
p[13] = (s4 + r4 - 3.) * ept;
p[14] = 0.;
p[15] = (s4 - 1.) * ept;
p[16]= -r4 * ept;
p[17]= r4 * ept;
p[18]= (4. - r4 - 8*(*s)) * ept;

p[19] = (s4 + r4 - 3.) * emt;
p[20] = 0.;
p[21] = (s4 - 1.) * emt;
p[22]= -r4 * emt;
p[23]= r4 * emt;
p[24]= (4. - r4 - 8*(*s)) * emt;

/*----------------------------------------------------------------------
* Partielle Ableitung der Formfunktionen nach t
*---------------------------------------------------------------------*/
p[25]= (rr2 + ss2 + rs4 - r3 - s3 + 1.) * 0.5;
p[26]= (rr2 - (*r)) * 0.5;
p[27]= (ss2 - (*s)) * 0.5;
p[28]= (r4 - 2*rr2 - rs4) * 0.5;
p[29]= rs4 * 0.5;
p[30]= (s4 - 2*ss2 - rs4) * 0.5;

p[31]= -(rr2 + ss2 + rs4 - r3 - s3 + 1.) * 0.5;
p[32]= -(rr2 - (*r)) * 0.5;
p[33]= -(ss2 - (*s)) * 0.5;
p[34]= -(r4 - 2*rr2 - rs4) * 0.5;
p[35]= -rs4 * 0.5;
p[36]= -(s4 - 2*ss2 - rs4) * 0.5;

/*----------------------------------------------------------------------
* Jacobi-Matrix am Punkt (r,s,t) entwickeln
*---------------------------------------------------------------------*/
for(i = 1;i <= 3;i++)
  {
  for(j = 1;j <= 3;j++)
    {
    dum= 0.;
    for(k = 1;k <= 12;k++)
      {
      dum+= p[(i-1)*12 + k] * xx[(j-1)*12 + k];
      }
    xj[(i-1)*3 + j]= dum;
    }
  }

/*----------------------------------------------------------------------
* Jacobi-Determinante am Punkt (r,s,t) entwickeln
*---------------------------------------------------------------------*/
(*det)= (xj[1] * xj[5] * xj[9]) - (xj[1] * xj[6] * xj[8]) +
        (xj[2] * xj[6] * xj[7]) - (xj[2] * xj[4] * xj[9]) +
        (xj[3] * xj[4] * xj[8]) - (xj[3] * xj[5] * xj[7]);

if(FR_FABS(*det) < 0.00000000001)
  {
  return(AL_JACNEG);
  }

/*----------------------------------------------------------------------
* Berechnung der inversen Jacobi-Matrix
*---------------------------------------------------------------------*/
dum= 1./(*det);

xji[1]=  (xj[5] * xj[9] - xj[8] * xj[6])*dum;
xji[2]= -(xj[2] * xj[9] - xj[8] * xj[3])*dum;
xji[3]=  (xj[2] * xj[6] - xj[5] * xj[3])*dum;
xji[4]= -(xj[4] * xj[9] - xj[7] * xj[6])*dum;
xji[5]=  (xj[1] * xj[9] - xj[7] * xj[3])*dum;
xji[6]= -(xj[1] * xj[6] - xj[4] * xj[3])*dum;
xji[7]=  (xj[4] * xj[8] - xj[7] * xj[5])*dum;
xji[8]= -(xj[1] * xj[8] - xj[7] * xj[2])*dum;
xji[9]=  (xj[1] * xj[5] - xj[4] * xj[2])*dum;

/*----------------------------------------------------------------------
* Entwickeln der Matrix b
*---------------------------------------------------------------------*/
for(i = 1;i <= 216;i++)  /* 12 Knoten * 3 FG * 6 */
  b[i]= 0.;

k3= 0;

for(k = 1;k <= 12;k++)
  {
  k3+= 3;

  for(i = 1;i <= 3;i++)
    {
    b[     k3-2]+= xji[    i] * p[(i-1)*12 + k];
    b[36 + k3-1]+= xji[3 + i] * p[(i-1)*12 + k];
    b[72 + k3  ]+= xji[6 + i] * p[(i-1)*12 + k];
    }
  b[108 + k3-2]= b[36 + k3-1];
  b[108 + k3-1]= b[     k3-2];
            
  b[144 + k3-1]= b[72 + k3  ];
  b[144 + k3  ]= b[36 + k3-1];

  b[180 + k3-2]= b[72 + k3  ];
  b[180 + k3  ]= b[     k3-2];
  }

return(0);
}

