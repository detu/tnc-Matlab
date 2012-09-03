function grad = computeGradient (x)
  x1 = x(1);
  x2 = x(2);

  grad(1) = 2*x1;
  grad(2) = 3*(abs(x2))^2;

  if x2 < 0; grad(2) = -grad(2);end;
  

  
