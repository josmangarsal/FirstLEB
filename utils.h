/******************************************************************************
			utils.h  - description
			----------------------
                    L.G. Casado leo@ual.es. 2014. GPLv3.

     ********************************************************************
     Purpose: Define new types of data and implement routines that will
     	be used by other modules.

	OpenFile(): Open a file given by "name" in the mode given by "mode".
	CloseFile(): Check the correct close.
	ParametersError(): Shows the correct use of the comand line if some
		error has been detected.
	PrintVR(): Print the contents od a real vector.
	CopyVR(): Copy two real vectors.
	CompareVR(): Compare in order each component of two real vectors.
		If there is a difference it returns if the first is smaller or
		greater, otherwise both vectors are equal.
	NewSQMatrixR(): Get memory for a square matrix.
	FreeSQMatrixR(): Free the memory allocated by a sqare matrix.
	PrintSQMatrixR(): Print the square matrix values.
	CopySQMatrixR(): Copy a square matrix.
	RandomInt(): Generates a real random number between 0 an n with an
		integer value.
	XInWindow(): Given a point in the simplex space, calculates its X
		coordinate in the graphical window space.
	YInWindow(): Given a point in the simplex space, calculates its Y
		coordinate in the graphical window space.
*******************************************************************************/
#ifndef __UTILS__
#define __UTILS__

/*---------------------------------------------------------------------------*/
typedef size_t		SIZE;
typedef	void		VOID;
typedef VOID 	*	PVOID;
typedef	PVOID	* 	PPVOID;
typedef PPVOID  *       PPPVOID;

typedef int		INT;
typedef INT 	*	PINT;
typedef	PINT	*	PPINT;

typedef long long int	LLINT;
typedef LLINT 	*		PLLINT;

typedef char		CHAR;
typedef CHAR	*	PCHAR;
typedef	PCHAR	* 	PPCHAR;

typedef	double		REAL;
typedef REAL	 *	PREAL;
typedef PREAL	 * 	PPREAL;
typedef PPREAL	 * 	PPPREAL;
typedef PPPREAL	 * 	PPPPREAL;

#define MAXVALUE	MAXDOUBLE
#define MINVALUE	MINDOUBLE
#define	REALPrec	10e-14  

typedef unsigned char 	UCHAR;

typedef UCHAR 		BOOL;
#define	False	0
#define	True	1
typedef BOOL * PBOOL;

typedef CHAR 		COMPARE;
#define	Greater	  1
#define Equal	  0
#define	Smaller	 -1

/*---------------------------------------------------------------------------*/
FILE 	*OpenFile       (char *name, char *mode);
void 	CloseFile       (FILE *f);
void 	ParametersError ();
void 	PrintVR 	(FILE * FOut, PREAL vr, INT NDim);
void    CopyVR 		(PREAL PD, PREAL PO, INT NDim);
COMPARE CompareVR 	(PREAL VR1, PREAL VR2, INT NDim);
REAL    RandomInt       (INT n);
BOOL    PointInX123     (PREAL pX, INT NDim);
REAL    XInWindow       (PREAL pX, INT WWidth);
REAL    YInWindow       (PREAL pX, INT WWidth);
VOID    DrawCircle      (PREAL pX, REAL Radius, INT NCircle,
                         INT WWidth, PCHAR color, PCHAR dash);
VOID    DelCircle       (INT NCircle);
VOID    DrawEpsCircle   (PREAL pX, REAL Radius, INT NCircle, INT WWidth,
                         PCHAR OutLineColor, PCHAR FillColor);
VOID    DelEpsCircle    (INT NCircle);
VOID    DrawPCover      (PREAL pX, INT NPC, INT WWidth, PCHAR color);
VOID    DelPCover       (INT NPC);
INT     Fact            (INT n);
PPINT   CombNoRep       (INT n, INT k, PINT pNComb);
INT     CombN2ToIndex   (INT N, INT x, INT y);
VOID    IndexToCombN2   (INT N, INT Index, PINT px, PINT py);
REAL    DistPoints      (PREAL p1, PREAL p2, INT NDim);
BOOL    EQ              (REAL a, REAL b);
BOOL    LT              (REAL a, REAL b);
BOOL    GT              (REAL a, REAL b);
BOOL    LE              (REAL a, REAL b);
BOOL    GE              (REAL a, REAL b);
void    MiddlePoint     (PREAL pA, PREAL pB, PREAL pMiddle, INT NDim);
PCHAR   ColorConst      (INT NConst);
PCHAR   DashConst       (INT NConst);
PCHAR   ColorCover      (INT WhichCover);

#endif /*__UTILS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
