/***********************************************************************
* 
*               *****   ***    ***
*                  *   *   *  *   *
*                 *     ***    ***
*                *     *   *  *   *
*               *****   ***    ***
*
* A FREE Finite Elements Analysis Program in ANSI C for the Windows &
* the UNIX OS.
*
* Composed and edited and copyright by 
* Professor Dr.-Ing. Frank Rieg, University of Bayreuth, Germany
*
* eMail: 
* frank.rieg@uni-bayreuth.de
* dr.frank.rieg@t-online.de
* 
* V15.0  November 18, 2015
*
* Z88 should compile and run under any Windows & UNIX OS and GTK+.
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
* clr88 loescht den Bildschirm
* Fuer Windows und UNIX und GTK+
* 16.4.2009
***********************************************************************/

/***********************************************************************
* Fuer Win 
***********************************************************************/
#ifdef FR_WIN95
#include <stdlib.h>

void clr88(void)
{
system("cls");
return;
}
#endif

/***********************************************************************
* Fuer UNIX 
***********************************************************************/
#ifdef FR_UNIX
#include <stdlib.h>

void clr88(void)
{
system("tput clear");
return;
}
#endif

/***********************************************************************
* GTK fuer Windows
***********************************************************************/
#ifdef FR_GTKWIN
#include <stdlib.h>

void clr88(void)
{
system("cls");
return;
}
#endif

