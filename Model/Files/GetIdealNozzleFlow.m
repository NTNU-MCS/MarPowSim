%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ m_dot h_dot N_dot] = GetIdealNozzleFlow( Cd, A, p_in, p_out, T_in, X_in)
%Calculate the mass flow through ideal nozzle(valve)
%   Input
%       Cd, A, p_in, p_out, T_in, X_in
%   Output
%       m_dot
    [R, Cp, Cv, ~, ht, ~, ~] = GetThermoDynProp(p_in,T_in,X_in);
    R_0 = 8314.4621;
    MW = R_0/R;
    pr = p_out/p_in;

    gamma = Cp/Cv;
    gamma1 = (gamma-1)/gamma;
    if pr < (2/(gamma+1))^(1/gamma1)
        m_dot = Cd*A*p_in/sqrt(R*T_in)*sqrt(gamma*(2/(gamma+1))^((gamma+1)/(gamma-1)));
    else
        m_dot = Cd*A*p_in/sqrt(R*T_in)*pr^(1/gamma)*sqrt(2/gamma1*(1-pr^gamma1));
    end;
    h_dot = m_dot * ht;
    N_dot = X_in*m_dot/MW;
end

