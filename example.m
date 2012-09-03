% Test the "tn" Matlab interface on the Hock & Schittkowski test problem
% #38. See: Willi Hock and Klaus Schittkowski. (1981) Test Examples for
% Nonlinear Programming Codes. Lecture Notes in Economics and Mathematical
% Systems Vol. 187, Springer-Verlag.

% The starting point.
x0  = [1 1];     % The starting point.
lb  = [-1e6 -1e6];  % Lower bound on the variables.
ub  = [+1e6 +1e6];  % Upper bound on the variables.

% ganti cara call-nya !
x = tn(x0,lb,ub,'computeObjective','computeGradient',...
           [],'maxCGit',7,'maxnfeval',200000);
