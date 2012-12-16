#ifndef SOLVE_H
#define SOLVE_H
#include <klu.h>
namespace DMY{
class Solve
{
    public:
        /** Default constructor */
        Solve();
        /** Default destructor */
        virtual ~Solve();
    static void solveT(int dim,int *Ap,int *Ai,double *Ax,double *b);
    static void solve(int dim,int *Ap,int *Ai,double *Ax,double *b);
    protected:
    private:
};

}
#endif // SOLVE_H
