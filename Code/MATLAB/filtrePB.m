function out = filtrePB(x, N)

  x4 = 0;
  x3 = 0;
  x2 = 0;
  x1 = 0;

  y4 = 0;
  y3 = 0;
  y2 = 0;
  y1 = 0;
  b0 = 752;
  b2 = -1504;
  b4 = 752;
    

  a0 = 10000;
  a1 = -28343;
  a2 = 30477;
  a3 = -15080;
  a4 = 3007;
    
    for n=1:N 
        out(n) = (b0*x(n) + b2*x2 + b4*x4 -a1*y1 -a2*y2 -a3*y3 -a4*y4)/a0;

        x4 = x3;
        x3 = x2;
        x2 = x1;
        x1 = x(n);


        y4 = y3;
        y3 = y2;
        y2 = y1;
        y1 = out(n);

    end


end
