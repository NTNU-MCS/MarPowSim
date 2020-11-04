#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <math.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void ThdynCVNV2Zone_Output_wrapper(const real_T     *FComb,
                                 const real_T       *phi,
                                 real_T             *p,
                                 real_T             *Tu,
                                 real_T             *Fu,
                                 real_T             *Vu,
                                 real_T             *Tb,
                                 real_T             *Fb,
                                 real_T             *Vb,
								 real_T             *pb,
                                 const real_T       *xC, 
                                 const real_T       *pPrev,        
                                 const real_T       *TuPrev,
                                 const real_T       *FuPrev,
                                 const real_T       *RuPrev,
                                 const real_T       *uuPrev,
                                 const real_T       *CvuPrev,
                                 const real_T       *TbPrev,
                                 const real_T       *FbPrev,
                                 const real_T       *RbPrev,
                                 const real_T       *ubPrev,
                                 const real_T       *CvbPrev,
                                 const real_T       *mCylPrev,
                                 const real_T       *mbCylPrev,
                                 const real_T       *VCylPrev,
                                 const real_T       *QCylPrev,
                                 const real_T       *HCylPrev,        
                                 const real_T       *WCylPrev,        
                                 const real_T       *muPrev,
                                 const real_T       *mubPrev,
                                 const real_T       *VuPrev,
                                 const real_T       *mfbPrev,
                                 const real_T       *mMixPrev,
                                 const real_T       *mb0,
                                 const real_T  *fs,  const int_T  p_width0,
                                 const real_T *DPhiMix, const int_T p_width1,
                                 const real_T *phiMix0, const int_T p_width2,
                                 const real_T *alpha, const int_T p_width3,
								 const real_T  *hn, const int_T p_width4,
								 const real_T *nStroke, const int_T p_width6)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
real_T mCyl, mbCyl, VCyl, mfb, QCyl, HCyl, WCyl;
real_T phiCyc, q, mMix, dmMix;
real_T pCyl, TCyl, FCyl;
const real_T pi = 3.141592653590;

mCyl = xC[0];
mbCyl = xC[1];
VCyl = xC[2];
mfb = xC[3];
QCyl = xC[4];
HCyl = xC[5];
WCyl = xC[6];

GetPTFV1(mCyl, mbCyl, -QCyl+HCyl-WCyl, VCyl, TuPrev[0], RuPrev[0], uuPrev[0],
	CvuPrev[0], fs[0], &pCyl, &TCyl, &FCyl);

// Calculate the the mix rate
phiCyc = phi[0]*180.0/pi;
phiCyc = ((int)(floor(phiCyc)) % ((int)(nStroke[0]*180))) + phiCyc - floor(phiCyc);
q = (phiCyc - phiMix0[0]) / DPhiMix[0];
if (q < 0)
{
    q = 0;
	dmMix = 0;
}
else
{
    if (q > 1)
    {
        q = 1;
		dmMix = 0;
    }
	else
	{
		mMix = mb0[0] * q*q*(3 - 2 * q);
		dmMix = mMix - mMixPrev[0];
	}
}

// Iterate Vu and Vb for the common pressure
Get2ZonePTFIter(mCyl, mbCyl, VCyl, mfb, QCyl, HCyl, WCyl, dmMix, FComb[0], fs[0], hn[0], pPrev[0],
	TuPrev[0], FuPrev[0], RuPrev[0], uuPrev[0], CvuPrev[0], TbPrev[0], FbPrev[0], RbPrev[0],
	ubPrev[0], CvbPrev[0], mCylPrev[0], mbCylPrev[0], VCylPrev[0], QCylPrev[0], HCylPrev[0],
	WCylPrev[0], muPrev[0], mubPrev[0], VuPrev[0], mfbPrev[0], p, pb, Tu, Tb, Fu, Fb, Vu, Vb);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
  *  Derivatives function
  *
  */
void ThdynCVNV2Zone_Derivatives_wrapper(const real_T *dm,
                          const real_T *dmb,
                          const real_T *dV,
                          const real_T *dmfb,
                          const real_T *dQCyl,
                          const real_T *dHCyl,
                          const real_T *dWCyl,  
                          real_T *dx)
{
/* %%%-SFUNWIZ_wrapper_Derivatives_Changes_BEGIN --- EDIT HERE TO _END */
    dx[0] = dm[0];
    dx[1] = dmb[0];
    dx[2] = dV[0];
    dx[3] = dmfb[0];
    dx[4] = dQCyl[0];
    dx[5] = dHCyl[0];
    dx[6] = dWCyl[0];
/* %%%-SFUNWIZ_wrapper_Derivatives_Changes_END --- EDIT HERE TO _BEGIN */
}

void ThdynCVNV2Zone_Update_wrapper(const real_T *phi,
                          const real_T *FComb,
                          const real_T *combState,
                          const real_T *p,
                          const real_T *Tu,
                          const real_T *Fu,
                          const real_T *Vu,
                          const real_T *Tb,
                          const real_T *Fb,
                          const real_T *Vb,
                          const real_T *xC,        
                          real_T *pPrev,        
                          real_T *TuPrev,
                          real_T *FuPrev,
                          real_T *RuPrev,
                          real_T *uuPrev,
                          real_T *CvuPrev,
                          real_T *TbPrev,
                          real_T *FbPrev,
                          real_T *RbPrev,
                          real_T *ubPrev,
                          real_T *CvbPrev,
                          real_T *mCylPrev,
                          real_T *mbCylPrev,
                          real_T *VCylPrev,
                          real_T *QCylPrev,
                          real_T *HCylPrev,        
                          real_T *WCylPrev,        
                          real_T *muPrev,
                          real_T *mubPrev,
                          real_T *VuPrev,
                          real_T *mfbPrev,
                          real_T *mMixPrev,
                          real_T *mb0,
                          real_T *ResetMix,        
                          const real_T  *fs,  const int_T  p_width1,
                          const real_T *DPhiMix, const int_T p_width3,
                          const real_T *phiMix0, const int_T p_width4,
                          const real_T *alpha, const int_T p_width5,
						  const real_T *hn, const int_T p_width6,
                          const real_T *nStroke, const int_T p_width7)
{
	real_T R, h, s, u, RF, RP, RT, uF, uP, uT, sF, sP, sT, Cp, Cv, K;
	real_T phiCyc, q;
	const real_T pi = 3.141592653590;


	pPrev[0] = p[0];    
    TuPrev[0] = Tu[0];    FuPrev[0] = Fu[0];      VuPrev[0] = Vu[0];
    GetThdynCombGasZachV1(p[0],Tu[0],Fu[0],fs[0],&R,&h,&s,&u,
                &RF,&RP,&RT,&uF,&uP,&uT,&sF,&sP,&sT,&Cp,&Cv,&K);
    RuPrev[0] = R;    uuPrev[0] = u;    CvuPrev[0] = Cv;
    muPrev[0] = p[0]*Vu[0]/(R*Tu[0]);    
    mubPrev[0] = muPrev[0]*Fu[0]*fs[0]/(1+Fu[0]*fs[0]);
    TbPrev[0] = Tb[0];    FbPrev[0] = Fb[0];     
    GetThdynCombGasZachV1(p[0],Tb[0],Fb[0],fs[0],&R,&h,&s,&u,
                &RF,&RP,&RT,&uF,&uP,&uT,&sF,&sP,&sT,&Cp,&Cv,&K);
    RbPrev[0] = R;    ubPrev[0] = u,	CvbPrev[0] = Cv;
    mCylPrev[0] = xC[0];  mbCylPrev[0] = xC[1];   VCylPrev[0] = xC[2];
    mfbPrev[0] = xC[3];
    
	QCylPrev[0] = xC[4];
	HCylPrev[0] = xC[5];
	WCylPrev[0] = xC[6];

    // Calculate the the mix rate
	phiCyc = phi[0] * 180.0 / pi;
	phiCyc = ((int)(floor(phiCyc)) % ((int)(nStroke[0] * 180))) + phiCyc - floor(phiCyc);
	q = (phiCyc - phiMix0[0]) / DPhiMix[0];
	if (q < 0)
	{
		q = 0;
		mMixPrev[0] = 0;
	}
	else
	{
		if (q > 1)
		{
			q = 1;
			mMixPrev[0] = 0;
		}
		else
		{
			mMixPrev[0] = mb0[0] * q*q*(3 - 2 * q);
		}
	}
    if(combState[0] == 2 && ResetMix[0] == 0)
    {
        ResetMix[0] = 1;
    }
    if(combState[0] == 5 && ResetMix[0] == 1)
    {
        ResetMix[0] = 0;
        mb0[0] = xC[0] - muPrev[0] - alpha[0]*(xC[0]);
        mMixPrev[0] = 0;
    }
 }

