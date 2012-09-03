function f = computeObjective (x)
  x1 = x(1);
  x2 = x(2);
  
  f = x1^2 + (abs(x2))^3;

