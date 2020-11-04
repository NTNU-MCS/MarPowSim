#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

#include <math.h>

void Get2ZonePTFIter(real_T mCyl,
					 real_T mbCyl,
					 real_T VCyl,
					 real_T mfb,
					 real_T QCyl,
					 real_T HCyl,
					 real_T WCyl,
					 real_T dmMix,
					 real_T FComb,
					 real_T fs,
					 real_T hn,
					 real_T pPrev,
					 real_T TuPrev,
					 real_T FuPrev,
					 real_T RuPrev,
					 real_T uuPrev,
					 real_T CvuPrev,
					 real_T TbPrev,
					 real_T FbPrev,
					 real_T RbPrev,
					 real_T ubPrev,
					 real_T CvbPrev,
					 real_T mCylPrev,
					 real_T mbCylPrev,
					 real_T VCylPrev,
					 real_T QCylPrev,
					 real_T HCylPrev,
					 real_T WCylPrev,
					 real_T muPrev,
					 real_T mubPrev,
					 real_T VuPrev,
					 real_T mfbPrev,
					 real_T *puTemp,
					 real_T *pbTemp,
					 real_T *TuTemp,
					 real_T *TbTemp,
					 real_T *FuTemp,
					 real_T *FbTemp,
					 real_T *VuTemp,
					 real_T *VbTemp)
{
	real_T Ru, hu, su, uu, RF1, RP1, RT1, uF1, uP1, uT1, sF1, sP1, sT1, Cp1, Cv1, K1;
	real_T Rb, hb, sb, ub, RF2, RP2, RT2, uF2, uP2, uT2, sF2, sP2, sT2, Cp2, Cv2, K2;
	real_T dmCyl, dVCyl, dmbCyl, dmfb, dQCyl, dHCyl, dWCyl;
	real_T Ffs, dmbMix;
	real_T mbPrev;
	real_T dmu, dmub, dmbmix, dQu, dHu, muTemp, mubTemp;
	real_T dmb, dmbb, dQb, dHb, mbTemp, mbbTemp;
	
	real_T delP, errP, noIter, dVuCorr, dVbCorr, dVu, dVb, dWu, dWb;

	real_T TU, TU1;
	real_T EuPrev, EbPrev, EuTemp, EbTemp;
	real_T VbPrev;

	GetThdynCombGasZachV1(pPrev, TuPrev, FuPrev, fs, &Ru, &hu, &su, &uu,
		&RF1, &RP1, &RT1, &uF1, &uP1, &uT1, &sF1, &sP1, &sT1,
		&Cp1, &Cv1, &K1);
	GetThdynCombGasZachV1(pPrev, TbPrev, FbPrev, fs, &Rb, &hb, &sb, &ub,
		&RF2, &RP2, &RT2, &uF2, &uP2, &uT2, &sF2, &sP2, &sT2,
		&Cp2, &Cv2, &K2);

	//Calculate mass, energy balance for cylinder
	dmCyl = mCyl - mCylPrev;
	dQCyl = QCyl - QCylPrev;
	dHCyl = HCyl - HCylPrev;
	dmbCyl = mbCyl - mbCylPrev;
	dVCyl = VCyl - VCylPrev;
	dWCyl = WCyl - WCylPrev;
	dmfb = mfb - mfbPrev;

	mbPrev = mCylPrev - muPrev;
	Ffs = FComb * fs;
	// Calculate mass, energy balance for unburned zone
	dmu = dmCyl - (1 + Ffs) / Ffs*dmfb + dmMix;
	dmbMix = dmMix*(FbPrev*fs/(1+FbPrev*fs));
	dmub = dmbCyl - dmfb + dmbMix;
	dQu = dQCyl * TuPrev*muPrev / (TuPrev * muPrev + TbPrev * mbPrev);
	dHu = dHCyl - (1 / Ffs*hu + hn)*dmfb + dmMix*hb;
	muTemp = muPrev + dmu;
	mubTemp = mubPrev + dmub;

	// Calculate mass, energy balance for burned zone
	dmb = (1 + Ffs) / Ffs*dmfb - dmMix;
	dmbb = dmfb - dmbMix;
	dQb = dQCyl*TbPrev * mbPrev / (TuPrev * muPrev + TbPrev * mbPrev);
	dHb = (1 / Ffs*hu + hn)*dmfb - dmMix*hb;
	mbTemp = mCyl - muTemp;
	mbbTemp = mbCyl - mubTemp;
	
	errP = 1;
	noIter = 0;
	dVuCorr = 0;
	dVbCorr = 0;
	EuPrev = muPrev * uuPrev;
	EbPrev = -QCylPrev + HCylPrev - WCylPrev - EuPrev;
	VbPrev = VCylPrev - VuPrev;
	VuTemp[0] = VuPrev;
	VbTemp[0] = VbPrev;
	while (errP > 0.001 && noIter < 1000){
		++noIter;
		if (VuPrev < VbPrev){
			VuTemp[0] = VuTemp[0] + dVuCorr;
			dVu = VuTemp[0] - VuPrev;
			VbTemp[0] = VCyl - VuTemp[0];
			dVb = VbTemp[0] - VbPrev;
			if (dVCyl == 0){
				dWu = 0;
				dWb = 0;
			}
			else {
				dWu = fabs(dWCyl / dVCyl)*dVu;
				dWb = dWCyl - dWu;
			}
		}
		else {
			VbTemp[0] = VbTemp[0] + dVbCorr;
			dVb = VbTemp[0] - VbPrev;
			VuTemp[0] = VCyl - VbTemp[0];
			dVu = VuTemp[0] - VuPrev;
			if (dVCyl == 0){
				dWu = 0;
				dWb = 0;
			}
			else {
				dWb = fabs(dWCyl / dVCyl)*dVb;
				dWu = dWCyl - dWb;
			}
		}
		EuTemp = EuPrev - dQu - dWu + dHu;
		EbTemp = EbPrev - dQb - dWb + dHb;
		GetPTFV1(muTemp, mubTemp, EuTemp, VuTemp[0], TuPrev, RuPrev, uuPrev,
			CvuPrev, fs, puTemp, TuTemp, FuTemp);
		GetPTFV1(mbTemp, mbbTemp, EbTemp, VbTemp[0], TbPrev, RbPrev, ubPrev,
			CvbPrev, fs, pbTemp, TbTemp, FbTemp);
		delP = (puTemp[0] - pbTemp[0]);
		errP = fabs(delP / puTemp[0]);
		if (dVCyl == 0){
			dVuCorr = 0;
			dVbCorr = 0;
		}
		else{
			if (VuPrev < VbPrev){
				GetThdynCombGasZachV1(puTemp[0], TuTemp[0], FuTemp[0], fs, &Ru, &hu, &su, &uu,
					&RF1, &RP1, &RT1, &uF1, &uP1, &uT1, &sF1, &sP1, &sT1, &Cp1, &Cv1, &K1);
				TU1 = (RP1*sF1 - RF1*sP1) /
					(RP1*(uT1*sF1 - uF1*sT1) + RT1*(uF1*sP1 - uP1*sF1) +
					RF1*(uP1*sT1 - uT1*sP1));
				TU = 1 / uT1;
				dVuCorr = VuTemp[0] * VuTemp[0] * delP / 2 /
					(muTemp*Ru*(VuTemp[0] * TU*fabs(dWCyl / dVCyl) + TuTemp[0]) +
					VuTemp[0] * delP / 2);
				dVbCorr = 0;
			}
			else{
				GetThdynCombGasZachV1(pbTemp[0], TbTemp[0], FbTemp[0], fs, &Rb, &hb, &sb, &ub,
					&RF2, &RP2, &RT2, &uF2, &uP2, &uT2, &sF2, &sP2, &sT2, &Cp2, &Cv2, &K2);
				TU1 = (RP2*sF2 - RF2*sP2) /
					(RP2*(uT2*sF2 - uF2*sT2) + RT2*(uF2*sP2 - uP2*sF2) +
					RF2*(uP2*sT2 - uT2*sP2));
				TU = 1 / uT2;
				dVbCorr = -VbTemp[0] * VbTemp[0] * delP / 2 /
					(mbTemp*Rb*(VbTemp[0] * TU*fabs(dWCyl / dVCyl) + TbTemp[0]) +
					VbTemp[0] * delP / 2);
				dVuCorr = 0;
			}
		}
		if (noIter > 1000){
			break;
		}
	}
}