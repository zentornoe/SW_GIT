function X = modsecant(func, xi, d)
% input : 
%   fuc : function name
%   xi : initial guess
%   d : delta(small value)
% output :
%   X : final guess

maxit=50; % maximum allowable iterations
es = 0.001; %desired relative error
iter = 0;

while(1)
    iter = iter+1;
    xmi = xi;
    xi=xi-d*xi*func(xi)/(func(xi+d*xi)-func(xi)); %formula of modified secant
    if xi~=0
        ea = abs((xi-xmi)/xi)*100; %Ea(%)
    end
    if ea<=es||iter>maxit
        break;
    end
end
X=xi;
end