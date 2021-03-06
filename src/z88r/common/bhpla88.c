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
* Z88 should compile and run under any Windows OS and UNIX OS and 
* GTK+.
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
* diese Compilerunit umfasst: bhpla88 - Elementsteifigkeitsroutine
*                             bhab88  - Berechnung des Lastvektors
* 16-Knoten Lagrange Reissner-Mindlin- Platte
* 2.1.2010 Rieg
***********************************************************************/

/***********************************************************************
* Fuer UNIX
***********************************************************************/
#ifdef FR_UNIX
#include <z88r.h>
#endif

/***********************************************************************
* Fuer Windows
***********************************************************************/
#ifdef FR_WIN
#include <z88r.h>
#endif

/***********************************************************************
* Fuer Windows & GTK+
***********************************************************************/
#ifdef FR_GTKWIN
#include <z88r.h>
#endif

/***********************************************************************
*  Functions
***********************************************************************/
int bhab88(FR_DOUBLE *det,FR_DOUBLE *r,FR_DOUBLE *s);

/***********************************************************************
* hier beginnt Function bhpla88
***********************************************************************/
int bhpla88(void)
{
extern FR_DOUBLEAY se;

extern FR_DOUBLE xk[],yk[];
extern FR_DOUBLE xx[],be[],hi[];

extern FR_DOUBLE pree;

extern FR_INT4 intore;

FR_DOUBLE r,s,det,wt;

FR_INT4 ne=48,i,lx,ly,j;

int iret;
  
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
* xk und yk umspeichern
*---------------------------------------------------------------------*/
for(i = 1;i <= 16;i++)
  {
  xx[i]   = xk[i];
  xx[16+i]= yk[i];
  }

/*----------------------------------------------------------------------
* Elementsteifigkeitsmatrix aufstellen
*---------------------------------------------------------------------*/
for(i = 1;i <= ne;i++)
  be[i]= 0.;

for(lx = 1;lx <= intore;lx++)
  {
  r= xg[(lx-1)*4 + intore];
  for(ly = 1;ly <= intore;ly++)
    {
    s= xg[(ly-1)*4 + intore];

/*======================================================================
* Matrix b der partiellen Ableitungen & Jacobi Determinante holen
*=====================================================================*/
    iret= bhab88(&det,&r,&s);
    if(iret != 0) return(iret);
    
    wt= wgt[(lx-1)*4 + intore] * wgt[(ly-1)*4 + intore] * det;

/*======================================================================
* Element- Lastvektor be
*=====================================================================*/
    for(j = 1;j <= 48;j++)
      {
      be[j]+= hi[j]*wt*pree;
      }
    }
  }

return(0);
}

/***********************************************************************
* hier beginnt Function bhab88
***********************************************************************/
int bhab88(FR_DOUBLE *det,FR_DOUBLE *r,FR_DOUBLE *s)
{
/*---------------------------------------------------------------------
* xx geht rein, unveraendert (ex)
* det geht raus, neu
* r,s gehen rein, unveraendert
*--------------------------------------------------------------------*/

extern FR_DOUBLE h[];
extern FR_DOUBLE xx[],p[],hi[];

FR_DOUBLE xj[5];          /* ist 2x2 +1 */

FR_DOUBLE gx3y3,gx3y2,gx3y,gx3,gx2y3,gx2y2,gx2y,gx2,dum;
FR_DOUBLE gxy3,gxy2,gxy,gx,gy3,gy2,gy,g256;
        
FR_INT4 i,j,k,k3;
         
/*----------------------------------------------------------------------
* Faktoren fuer Formfunktionen belegen
*---------------------------------------------------------------------*/
gx3y3= (*r)*(*r)*(*r)*(*s)*(*s)*(*s);
gx3y2= (*r)*(*r)*(*r)*(*s)*(*s);
gx3y = (*r)*(*r)*(*r)*(*s);
gx3  = (*r)*(*r)*(*r);

gx2y3= (*r)*(*r)*(*s)*(*s)*(*s);
gx2y2= (*r)*(*r)*(*s)*(*s);
gx2y = (*r)*(*r)*(*s);
gx2  = (*r)*(*r);

gxy3 = (*r)*(*s)*(*s)*(*s);
gxy2 = (*r)*(*s)*(*s);
gxy  = (*r)*(*s);
gx   = (*r);

gy3  = (*s)*(*s)*(*s);
gy2  = (*s)*(*s);
gy   = (*s);

g256 =  1./256.;

/*----------------------------------------------------------------------
* Formfunktionen
*---------------------------------------------------------------------*/
h[1]= g256*(  81.*gx3y3 -  81.*gx3y2 -   9.*gx3y  +   9.*gx3   -
              81.*gx2y3 +  81.*gx2y2 +   9.*gx2y  -   9.*gx2   -
               9.*gxy3  +   9.*gxy2  +      gxy   -      gx    +
               9.*gy3   -   9.*gy2   -      gy    +   1.);

h[2]= g256*(-243.*gx3y3 +  81.*gx3y2 + 243.*gx3y  -  81.*gx3   +
             243.*gx2y3 -  81.*gx2y2 - 243.*gx2y  +  81.*gx2   +
              27.*gxy3  -   9.*gxy2  -  27.*gxy   +   9.*gx    -
              27.*gy3   +   9.*gy2   +  27.*gy    -   9.);

h[3]= g256*( 243.*gx3y3 +  81.*gx3y2 - 243.*gx3y  -  81.*gx3   -
             243.*gx2y3 -  81.*gx2y2 + 243.*gx2y  +  81.*gx2   -
              27.*gxy3  -   9.*gxy2  +  27.*gxy   +   9.*gx    +
              27.*gy3   +   9.*gy2   -  27.*gy    -   9.);

h[4]= g256*( -81.*gx3y3 -  81.*gx3y2 +   9.*gx3y  +   9.*gx3   +
              81.*gx2y3 +  81.*gx2y2 -   9.*gx2y  -   9.*gx2   +
               9.*gxy3  +   9.*gxy2  -      gxy   -      gx    -
               9.*gy3   -   9.*gy2   +      gy    +   1.);

h[5]= g256*(-243.*gx3y3 + 243.*gx3y2 +  27.*gx3y  -  27.*gx3   +
              81.*gx2y3 -  81.*gx2y2 -   9.*gx2y  +   9.*gx2   +
             243.*gxy3  - 243.*gxy2  -  27.*gxy   +  27.*gx    -
              81.*gy3   +  81.*gy2   +   9.*gy    -   9.);

h[6]= g256*( 729.*gx3y3 - 243.*gx3y2 - 729.*gx3y  + 243.*gx3   -
             243.*gx2y3 +  81.*gx2y2 + 243.*gx2y  -  81.*gx2   -
             729.*gxy3  + 243.*gxy2  + 729.*gxy   - 243.*gx    +
             243.*gy3   -  81.*gy2   - 243.*gy    +  81.);

h[7]= g256*(-729.*gx3y3 - 243.*gx3y2 + 729.*gx3y  + 243.*gx3   +
             243.*gx2y3 +  81.*gx2y2 - 243.*gx2y  -  81.*gx2   +
             729.*gxy3  + 243.*gxy2  - 729.*gxy   - 243.*gx    -
             243.*gy3   -  81.*gy2   + 243.*gy    +  81.);

h[8]= g256*( 243.*gx3y3 + 243.*gx3y2 -  27.*gx3y  -  27.*gx3   -
              81.*gx2y3 -  81.*gx2y2 +   9.*gx2y  +   9.*gx2   -
             243.*gxy3  - 243.*gxy2  +  27.*gxy   +  27.*gx    +
              81.*gy3   +  81.*gy2   -   9.*gy    -   9.);

h[9]= g256*( 243.*gx3y3 - 243.*gx3y2 -  27.*gx3y  +  27.*gx3   +
              81.*gx2y3 -  81.*gx2y2 -   9.*gx2y  +   9.*gx2   -
             243.*gxy3  + 243.*gxy2  +  27.*gxy   -  27.*gx    -
              81.*gy3   +  81.*gy2   +   9.*gy    -   9.);

h[10]=g256*(-729.*gx3y3 + 243.*gx3y2 + 729.*gx3y  - 243.*gx3   -
             243.*gx2y3 +  81.*gx2y2 + 243.*gx2y  -  81.*gx2   +
             729.*gxy3  - 243.*gxy2  - 729.*gxy   + 243.*gx    +
             243.*gy3   -  81.*gy2   - 243.*gy    +  81.);

h[11]=g256*( 729.*gx3y3 + 243.*gx3y2 - 729.*gx3y  - 243.*gx3   +
             243.*gx2y3 +  81.*gx2y2 - 243.*gx2y  -  81.*gx2   -
             729.*gxy3  - 243.*gxy2  + 729.*gxy   + 243.*gx    -
             243.*gy3   -  81.*gy2   + 243.*gy    +  81.);

h[12]=g256*(-243.*gx3y3 - 243.*gx3y2 +  27.*gx3y  +  27.*gx3   -
              81.*gx2y3 -  81.*gx2y2 +   9.*gx2y  +   9.*gx2   +
             243.*gxy3  + 243.*gxy2  -  27.*gxy   -  27.*gx    +
              81.*gy3   +  81.*gy2   -   9.*gy    -   9.);

h[13]=g256*( -81.*gx3y3 +  81.*gx3y2 +   9.*gx3y  -   9.*gx3   -
              81.*gx2y3 +  81.*gx2y2 +   9.*gx2y  -   9.*gx2   +
               9.*gxy3  -   9.*gxy2  -      gxy   +      gx    +
               9.*gy3   -   9.*gy2   -      gy    +   1.);

h[14]=g256*( 243.*gx3y3 -  81.*gx3y2 - 243.*gx3y  +  81.*gx3   +
             243.*gx2y3 -  81.*gx2y2 - 243.*gx2y  +  81.*gx2   -
              27.*gxy3  +   9.*gxy2  +  27.*gxy   -   9.*gx    -
              27.*gy3   +   9.*gy2   +  27.*gy    -   9.);

h[15]=g256*(-243.*gx3y3 -  81.*gx3y2 + 243.*gx3y  +  81.*gx3   -
             243.*gx2y3 -  81.*gx2y2 + 243.*gx2y  +  81.*gx2   +
              27.*gxy3  +   9.*gxy2  -  27.*gxy   -   9.*gx    +
              27.*gy3   +   9.*gy2   -  27.*gy    -   9.);

h[16]=g256*(  81.*gx3y3 +  81.*gx3y2 -   9.*gx3y  -   9.*gx3   +
              81.*gx2y3 +  81.*gx2y2 -   9.*gx2y  -   9.*gx2   -
               9.*gxy3  -   9.*gxy2  +      gxy   +      gx    -
               9.*gy3   -   9.*gy2   +      gy    +   1.);

/*----------------------------------------------------------------------
* Partielle Ableitung der Formfunktionen nach r
*---------------------------------------------------------------------*/
p[1] = g256*(  27.*gx2 * (  9.*gy3 -   9.*gy2 -      gy + 1.) -
               18.*gx  * (  9.*gy3 -   9.*gy2 -      gy + 1.) -
                9.*gy3 +    9.*gy2 +      gy  -   1.);
 
p[2] = g256*(-243.*gx2 * (  3.*gy3 -      gy2 -   3.*gy + 1.) +
              162.*gx  * (  3.*gy3 -      gy2 -   3.*gy + 1.) +
               27.*gy3 -    9.*gy2 -  27.*gy  +   9.);

p[3] = g256*( 243.*gx2 * (  3.*gy3 +      gy2 -   3.*gy - 1.) -
              162.*gx  * (  3.*gy3 +      gy2 -   3.*gy - 1.) -
               27.*gy3 -    9.*gy2 +  27.*gy  +   9.);

p[4] = g256*( -27.*gx2 * (  9.*gy3 +   9.*gy2 -      gy - 1.) +
               18.*gx  * (  9.*gy3 +   9.*gy2 -      gy - 1.) +
                9.*gy3 +    9.*gy2 -      gy  -   1.);

p[5] = g256*( -81.*gx2 * (  9.*gy3 -   9.*gy2 -      gy + 1.) +
               18.*gx  * (  9.*gy3 -   9.*gy2 -      gy + 1.) +
              243.*gy3 -  243.*gy2 -  27.*gy  +  27.);

p[6] = g256*( 729.*gx2 * (  3.*gy3 -      gy2 -   3.*gy + 1.) -
              162.*gx  * (  3.*gy3 -      gy2 -   3.*gy + 1.) -
              729.*gy3 +  243.*gy2 + 729.*gy  - 243.);

p[7] = g256*(-729.*gx2 * (  3.*gy3 +      gy2 -   3.*gy - 1.) +
              162.*gx  * (  3.*gy3 +      gy2 -   3.*gy - 1.) +
              729.*gy3 +  243.*gy2 - 729.*gy  - 243.);

p[8] = g256*(  81.*gx2 * (  9.*gy3 +   9.*gy2 -      gy - 1.) -
               18.*gx  * (  9.*gy3 +   9.*gy2 -      gy - 1.) -
              243.*gy3 -  243.*gy2 +  27.*gy  +  27.);

p[9] = g256*(  81.*gx2 * (  9.*gy3 -   9.*gy2 -      gy + 1.) +
               18.*gx  * (  9.*gy3 -   9.*gy2 -      gy + 1.) -
              243.*gy3 +  243.*gy2 +  27.*gy  -  27.);

p[10]= g256*(-729.*gx2 * (  3.*gy3 -      gy2 -   3.*gy + 1.) -
              162.*gx  * (  3.*gy3 -      gy2 -   3.*gy + 1.) +
              729.*gy3 -  243.*gy2 - 729.*gy  + 243.);

p[11]= g256*( 729.*gx2 * (  3.*gy3 +      gy2 -   3.*gy - 1.) +
              162.*gx  * (  3.*gy3 +      gy2 -   3.*gy - 1.) -
              729.*gy3 -  243.*gy2 + 729.*gy  + 243.);

p[12]= g256*( -81.*gx2 * (  9.*gy3 +   9.*gy2 -      gy - 1.) -
               18.*gx  * (  9.*gy3 +   9.*gy2 -      gy - 1.) +
              243.*gy3 +  243.*gy2 -  27.*gy  -  27.);

p[13]= g256*( -27.*gx2 * (  9.*gy3 -   9.*gy2 -      gy + 1.) -
               18.*gx  * (  9.*gy3 -   9.*gy2 -      gy + 1.) +
                9.*gy3 -    9.*gy2 -      gy  +   1.);

p[14]= g256*( 243.*gx2 * (  3.*gy3 -      gy2 -   3.*gy + 1.) +
              162.*gx  * (  3.*gy3 -      gy2 -   3.*gy + 1.) -
               27.*gy3 +    9.*gy2 +  27.*gy  -   9.);

p[15]= g256*(-243.*gx2 * (  3.*gy3 +      gy2 -   3.*gy - 1.) -
              162.*gx  * (  3.*gy3 +      gy2 -   3.*gy - 1.) +
               27.*gy3 +    9.*gy2 -  27.*gy  -   9.);

p[16]= g256*(  27.*gx2 * (  9.*gy3 +   9.*gy2 -      gy - 1.) +
               18.*gx  * (  9.*gy3 +   9.*gy2 -      gy - 1.) -
                9.*gy3 -    9.*gy2 +      gy  +   1.);

/*----------------------------------------------------------------------
* Partielle Ableitung der Formfunktionen nach s
*---------------------------------------------------------------------*/
p[17]= g256*(  27.*gy2 * (  9.*gx3 -   9.*gx2 -      gx + 1.) -
               18.*gy  * (  9.*gx3 -   9.*gx2 -      gx + 1.) -
                9.*gx3 +    9.*gx2 +      gx  -   1.);

p[18]= g256*( -81.*gy2 * (  9.*gx3 -   9.*gx2 -      gx + 1.) +
               18.*gy  * (  9.*gx3 -   9.*gx2 -      gx + 1.) +
              243.*gx3 -  243.*gx2 -  27.*gx  +   27.);

p[19]= g256*(  81.*gy2 * (  9.*gx3 -   9.*gx2 -      gx + 1.) +
               18.*gy  * (  9.*gx3 -   9.*gx2 -      gx + 1.) -
              243.*gx3 +  243.*gx2 +  27.*gx  -   27.);

p[20]= g256*( -27.*gy2 * (  9.*gx3 -   9.*gx2 -      gx + 1.) -
               18.*gy  * (  9.*gx3 -   9.*gx2 -      gx + 1.) +
                9.*gx3 -    9.*gx2 -      gx  +   1.);

p[21]= g256*(-243.*gy2 * (  3.*gx3 -      gx2 -   3.*gx + 1.) +
              162.*gy  * (  3.*gx3 -      gx2 -   3.*gx + 1.) +
               27.*gx3 -    9.*gx2 -  27.*gx  +   9.);

p[22]= g256*( 729.*gy2 * (  3.*gx3 -      gx2 -   3.*gx + 1.) -
              162.*gy  * (  3.*gx3 -      gx2 -   3.*gx + 1.) -
              729.*gx3 +  243.*gx2 + 729.*gx  - 243.);

p[23]= g256*(-729.*gy2 * (  3.*gx3 -      gx2 -   3.*gx + 1.) -
              162.*gy  * (  3.*gx3 -      gx2 -   3.*gx + 1.) +
              729.*gx3 -  243.*gx2 - 729.*gx  + 243.);

p[24]= g256*( 243.*gy2 * (  3.*gx3 -      gx2 -   3.*gx + 1.) +
              162.*gy  * (  3.*gx3 -      gx2 -   3.*gx + 1.) -
               27.*gx3 +    9.*gx2 +  27.*gx  -   9.);

p[25]= g256*( 243.*gy2 * (  3.*gx3 +      gx2 -   3.*gx - 1.) -
              162.*gy  * (  3.*gx3 +      gx2 -   3.*gx - 1.) -
               27.*gx3 -    9.*gx2 +  27.*gx  +   9.);

p[26]= g256*(-729.*gy2 * (  3.*gx3 +      gx2 -   3.*gx - 1.) +
              162.*gy  * (  3.*gx3 +      gx2 -   3.*gx - 1.) +
              729.*gx3 +  243.*gx2 - 729.*gx  - 243.);

p[27]= g256*( 729.*gy2 * (  3.*gx3 +      gx2 -   3.*gx - 1.) +
              162.*gy  * (  3.*gx3 +      gx2 -   3.*gx - 1.) -
              729.*gx3 -  243.*gx2 + 729.*gx  + 243.);

p[28]= g256*(-243.*gy2 * (  3.*gx3 +      gx2 -   3.*gx - 1.) -
              162.*gy  * (  3.*gx3 +      gx2 -   3.*gx - 1.) +
               27.*gx3 +    9.*gx2 -  27.*gx  -   9.);

p[29]= g256*( -27.*gy2 * (  9.*gx3 +   9.*gx2 -      gx - 1.) +
               18.*gy  * (  9.*gx3 +   9.*gx2 -      gx - 1.) +
                9.*gx3 +    9.*gx2 -      gx  -   1.);

p[30]= g256*(  81.*gy2 * (  9.*gx3 +   9.*gx2 -      gx - 1.) -
               18.*gy  * (  9.*gx3 +   9.*gx2 -      gx - 1.) -
              243.*gx3 -  243.*gx2 +  27.*gx  +   27.);

p[31]= g256*( -81.*gy2 * (  9.*gx3 +   9.*gx2 -      gx - 1.) -
               18.*gy  * (  9.*gx3 +   9.*gx2 -      gx - 1.) +
              243.*gx3 +  243.*gx2 -  27.*gx  -   27.);

p[32]= g256*(  27.*gy2 * (  9.*gx3 +   9.*gx2 -      gx - 1.) +
               18.*gy  * (  9.*gx3 +   9.*gx2 -      gx - 1.) -
                9.*gx3 -    9.*gx2 +      gx  +   1.);

/*----------------------------------------------------------------------
* Jacobi-Matrix am Punkt (r,s) entwickeln
*---------------------------------------------------------------------*/
for(i = 1;i <= 2;i++)
  {
  for(j = 1;j <= 2;j++)
    {
    dum= 0.;
    for(k = 1;k <= 16;k++)
      {
      dum+= p[(i-1)*16 + k] * xx[(j-1)*16 + k];
      }
    xj[(i-1)*2 + j]= dum;
    }
  }

/*----------------------------------------------------------------------
* Jacobi-Determinante am Punkt (r,s) entwickeln
*---------------------------------------------------------------------*/
(*det)= xj[1] * xj[4] - xj[3] * xj[2];

if((*det) < 1e-88)
  return(AL_JACNEG);

/*----------------------------------------------------------------------
* Entwickeln der Formfunktionen fuer den Lastvektor be
*---------------------------------------------------------------------*/
for(i = 1;i <= 48;i++)
  hi[i]= 0.;

k3= 1;

for(k = 1;k <= 16;k++)
  {
  hi[k3]= h[k];  
  k3+= 3;
  }

return(0);
}
