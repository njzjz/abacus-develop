#include "sto_wf.h"
#include "global.h"


Stochastic_WF::Stochastic_WF()
{
    chiortho  = new ComplexMatrix[1];
    chi0  = new ComplexMatrix[1];
     emax_sto = 1;
     emin_sto = -1;
     stotype = "pw";
}

Stochastic_WF::~Stochastic_WF()
{ 
    delete[] chiortho;
    delete[] chi0;
}

void Stochastic_WF::init()
{
    //wait for init

    int ndim;
    int nx,ny,nz;
    if(stotype == "pw")
    {
        ndim = kv.ngk[0]; // only GAMMA point temporarily
    }
    else
    {
        ndim = pw.nrxx;
    }
    
    //distribute nchi for each process
    bool allbase = false;
    if(nchi == 0)
    {
        nchi = kv.ngk[0]-NBANDS;
        cout<<"Using all normal bases: "<<kv.ngk[0]<<endl;
        allbase = true;
    }
    nchip = int(nchi/NPOOL);
    if(MY_POOL < nchi%NPOOL) ++nchip;

    complex<double> ui(0,1);

    //We temporarily init one group of orbitals for all k points.
    delete[] chi0;
    chi0 = new ComplexMatrix[1]; 
    chi0[0].create(nchip,ndim,false);
    
    
    srand((unsigned)time(NULL)+MY_POOL*100);
    //srand((unsigned)MY_POOL*100);
    //srand((unsigned)0);
    
    if(allbase)
    {
        int re = nchi % NPOOL;
        if(MY_POOL < re)
        for(int i = 0 ; i < nchip ; ++i)
        {
            chi0[0](i , MY_RANK * nchip + i) = 1;
        }
        else
        for(int i = 0 ; i < nchip ; ++i)
        {
            chi0[0](i , re + MY_RANK * nchip + i) = 1;
        }
    }
    else
    {
        //init with random number
        for(int i=0; i<chi0[0].size; ++i)
        {
            chi0[0].c[i]=exp(2*PI*rand()/double(RAND_MAX)*ui) / sqrt(double(nchi));
        }
    }
    

    //

    

    delete[] chiortho;
    int nkk = 1; // We temporarily use gamma k point.
    chiortho = new ComplexMatrix[1];
    if(NBANDS > 0)
    {
        chiortho[0].create(nchip,ndim,false);
    }
    
    return;
}


