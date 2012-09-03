% Test the "tn" Matlab interface on the Hock & Schittkowski test problem
% #38. See: Willi Hock and Klaus Schittkowski. (1981) Test Examples for
% Nonlinear Programming Codes. Lecture Notes in Economics and Mathematical
% Systems Vol. 187, Springer-Verlag.

% The starting point.
x0  = [-3  -1  -3  -1];   % The starting point.
lb  = [-10 -10 -10 -10];  % Lower bound on the variables.
ub  = [+10 +10 +10 +10];  % Upper bound on the variables.

% ganti cara call-nya !
x = tn(x0,lb,ub,'computeObjectiveHS038','computeGradientHS038',...
           [],'maxCGit',7,'maxnfeval',200);
