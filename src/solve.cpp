#include "solve.h"
namespace DMY{
Solve::Solve()
{
    //ctor
}

Solve::~Solve()
{
    //dtor
}

void Solve::solve(int dim,int *Ap,int *Ai,double *Ax,double *b)
{
    klu_symbolic *Symbolic ;
	klu_numeric *Numeric ;
	klu_common Common ;
	klu_defaults (&Common) ;
	Symbolic = klu_analyze (dim, Ap, Ai, &Common) ;
	Numeric = klu_factor (Ap, Ai, Ax, Symbolic, &Common) ;
	klu_tsolve (Symbolic, Numeric, dim, 1, b, &Common) ;
	klu_free_symbolic (&Symbolic, &Common) ;
	klu_free_numeric (&Numeric, &Common) ;

}

void Solve::solveT(int dim,int *Ap,int *Ai,double *Ax,double *b)
{
    klu_symbolic *Symbolic ;
	klu_numeric *Numeric ;
	klu_common Common ;
	klu_defaults (&Common) ;
	Symbolic = klu_analyze (dim, Ap, Ai, &Common) ;
	Numeric = klu_factor (Ap, Ai, Ax, Symbolic, &Common) ;
	klu_solve (Symbolic, Numeric, dim, 1, b, &Common) ;
	klu_free_symbolic (&Symbolic, &Common) ;
	klu_free_numeric (&Numeric, &Common) ;

}


}
