%    Copyright: 	NTNU, Trondheim, Norway
%    Licensed under GPL-3.0
%    Created:  	Kevin Koosup Yum (NTNU) on 23 May, 2013

function [ m_dot,H_dot,m_b_dot] = GetIdealNozzleFlowPTF( Cd, A, p_in, p_out, T_in, F_in, fs)
%Calculate the mass flow through ideal nozzle(valve)
%   Input
%       Cd, A, p_in, p_out, T_in, F_in
%   Output
%       m_dot h_dot m_b

    [R,~,~,ht,~,~,~,~,~,~,~,~,K] = GetThdynCombGasZach(p_in,T_in,F_in);
    R_0 = 8314.4621;
    MW = R_0/R;
    pr = p_out./p_in;

    gamma = K;
    gamma1 = (gamma-1)./gamma;
    idx = (pr < (2./(gamma+1)).^(1./gamma1));
    m_dot = zeros(length(p_in),1);
    if sum(idx) == 0
        m_dot = Cd*A*p_in./sqrt(R.*T_in).*pr.^(1./gamma).*sqrt(2./gamma1.*(1-pr.^gamma1));
    else
        m_dot(idx) = sqrt(gamma(idx).*(2./(gamma(idx)+1)).^((gamma(idx)+1)./(gamma(idx)-1)));
        idx = ~idx;
        m_dot(idx) = Cd*A*p_in(idx)./sqrt(R(idx).*T_in(idx)) ...
            .*pr(idx).^(1./gamma(idx)).* ...
            sqrt(2./gamma1(idx).*(1-pr(idx).^gamma1(idx)));
    end;
    H_dot = m_dot .* ht;
    m_b_dot = m_dot.*(F_in*fs)./(1+F_in*fs);
end

