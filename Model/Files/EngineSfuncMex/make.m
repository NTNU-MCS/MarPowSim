%
%  Copyright: 	NTNU, Trondheim, Norway
%  Licensed under GPL-3.0
%  Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013
%  Revised:	    <date>	<author> <description>
%		   		<date>	<author> <description>
fprintf('\n Now compiling all the code for c-mex s-functions\n');
currFolder = pwd;
delete '*.mex*'
cd c_code
mex AirWaterCoolerPTF.c AirWaterCoolerPTF_wrapper.c GetHTCoeffHTX.c GetAirViscosity.c GetThdynCombGasZachV1.c GetAirDensity.c GetAirThermalConduct.c ThdynPack_rtwutil.c rt_nonfinite.c rtGetInf.c rtGetNAN.c -outdir ..
%mex AirWaterCoolerPTF.c AirWaterCoolerPTF_wrapper.c GetHTCoeffHTX.c GetAirViscosity.c GetThdynCombGasZachV1.c GetAirDensity.c GetAirThermalConduct.c GetCompCombGas_rtwutil.c rt_nonfinite.c rtGetInf.c rtGetNAN.c -outdir ..
mex compressor_calc_PTF.c compressor_calc_PTF_wrapper.c GetThdynCombGasZachV1.c -outdir ..
%mex compressor_calc_PTF.c compressor_calc_PTF_wrapper.c GetThdynCombGasZachV1.c -outdir ..
mex IdealNozzlePTF.c IdealNozzlePTF_wrapper.c GetThdynCombGasZachV1.c GetIdealNozzleFlowPTF.c ThdynPack_rtwutil.c rt_nonfinite.c rtGetInf.c rtGetNAN.c sqrt.c power.c rdivide.c -outdir ..
%mex IdealNozzlePTF.c IdealNozzlePTF_wrapper.c GetThdynCombGasZachV1.c GetIdealNozzleFlow.c GetCompCombGas_rtwutil.c rt_nonfinite.c rtGetInf.c rtGetNAN.c -outdir ..
mex ThdynCVNV.c GetPTFV1.c GetThdynCombGasZachV1.c -outdir ..
%mex ThdynCVNV.c GetPTF.c GetThdynCombGasZachV1.c -outdir ..
mex turbine_calc_MV_PTF.c turbine_calc_MV_PTF_wrapper.c GetThdynCombGasZachV1.c -outdir ..
%mex turbine_calc_MV_PTF.c turbine_calc_MV_PTF_wrapper.c GetThdynCombGasZachV1.c -outdir ..
mex InCylinderMV.c InCylinderMV_wrapper.c GetThdynCombGasZachV1.c -outdir ..
%mex InCylinderMV.c InCylinderMV_wrapper.c GetThdynCombGasZachV1.c -outdir ..
cd(currFolder);