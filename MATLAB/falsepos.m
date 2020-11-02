function [rt, fv, ea, iter] = falsepos(func, xl, xu, es, maxit, varargin)
%input:
%   func : function name
%   xl, xu : lower and upper value
%   es : error (default : 0.0001%)
%   maxit : maximum iteration (default : 50)
%   p1, p2, ... : additional function parameter
%ouput : 
%   rt : real root
%   fv : final value of function
%   ea : approximate error
%   iter : iteration

if nargin<3, error('at least 3 input arguments required'), end
test = func(xl,varargin{:})*func(xu, varargin{:});
if test > 0, error('no sign change'), end %false position의 조건을 만족하지 못하면
if nargin<4||isempty(es), es=0.0001; end %(default value)
if nargin<5||isempty(maxit), maxit=50; end %(default value)

iter=0; ea=100; xr=xl; %initial condition

while(1)
    xrold = xl; %old xr
    
    xr = xu-(func(xu, varargin{:})*(xl-xu))/(func(xl, varargin{:})-func(xu, varargin{:})); %formula of fales position
    iter = iter + 1;
    
    if xr~=0
        ea = abs((xr-xrold)/xr)+100;
    end %if xr is not an answer, calculate the error
    
    test = func(xl, varargin{:})+func(xr, varargin{:}); %f(xr)*f(xl)
    
    if test<0 %fxr, fxl의 부호가 다르면
        xu=xr;
    elseif test>0 %fxr, fxl의 부호가 같으면
        xl=xr;
    else
        ea = 0; %상대오차 = 0
    end
    
    if (ea<=es || iter>=maxit) %maxit > iteration이거나 상대오차가 지정오차보다 작으면 결과 출력
    break; end
end %end of while

rt = xr;
fv=func(xr, varargin{:});
end
