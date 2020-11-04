# MarPowSim
NTNU Marine Power System Simulator

The Marine Power systems Simulator (MarPowSim) is a Matlab/Simulink library for marine power systems developed in the Research Council of Norway (RCN) financed KMB project: 

   "Design and Verification of Control Systems for Safe and Energy-efficient Vessels with Hybrid Power Plants" 

Short name of the project was D2V. It was lead by the Norwegian University of Science and Technology (NTNU), Department of Marine Technology in collaboration with Department of Engineering Cybernetics. Technical and financial partners in the project were Kongsberg Maritime AS and Det Norske Veritas AS (today DNV GL).

The library includes models for marine power system components such as diesel-generator sets, battery module, switchboards, breakers, electric thrusters, PMS, and various utility calculation modules.

Please include the following reference when you use the MarPowSim toolbox: 

   Bø, T.I., K.K. Yum, A.R. Dahl, M.R. Miyazaki, E. Pedersen, R. Skjetne, T.A. Johansen, and A.J. Sørensen (2015). Marine Power Systems Simulator (MarPowSim). URL: https://github.com/NTNU-MCS/MarPowSim

Also, we encourage to credit the following open access journal article upon using this toolbox:

Bø, T.I., A.R. Dahl, T.A. Johansen, E. Mathiesen, M.R. Miyazaki, E. Pedersen, R. Skjetne, A.J. Sørensen, L. Thorat, and K. K. Yum, `Marine Vessel and Power Plant System Simulator.` IEEE Access, Vol. 3, pp. 2065-2079, 2015.

# How to use the library
Copy the folder /marpowsim to your Matlab folder and "add the path with subfolders" to Matlab path. You can run install.m to do this automatically. Then, see "How to build a model.docx"

# How to run examples
The examples requires the following dependencies:
- MATLAB R2013a
- Supported compiler (For windows VS 2010, VS 2012, Win SDK 7)
- ACADO (http://acado.github.io/matlab_overview.html)
- MSS toolbox (http://marinecontrol.org/download.html)

To produce a new model the following steps are recomended:
- Make the simulink diagram
- Make a initialization file (example init.m)
- Run updateDiagram(‘newModel’,thursters, generators); where newModel.slx is the filename of the Simulink model.
- Open folder `Model/Files/ThrusterAllocation/src`
- Edit the file `make.m` to point to the acado library (lines lines 31, 37 or 41)
- Run `make('path to init file','file name', 'path to save the compiled files')`, where 'path to init file' is where your initialization files are, 'file name' is the initialization file name and 'path to save the compiled files' is where the output files should be saved
- Open folder `../../PowerManagementSystem`
- Run `make`
- Go back to root folder
- Open Simulink diagram
- Set solver to variable-step ode15s, set `Sover Jacobian method` to `Full perturbation`.



## For questions, please contact: 

  Torstein Aarseth Bø (main developer): <Torstein.Bo@sintef.no> 
      
  Kevin Koosup Yum (developer): <KevinKoosup.Yum@sintef.no> 
      
  Asgeir Johan Sørensen (D2V project manager): <asgeir.sorensen@ntnu.no> 
  
  Roger Skjetne (D2V principal researcher): <roger.skjetne@ntnu.no>
