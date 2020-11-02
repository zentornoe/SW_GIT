function [zint] = interp(x, y, z, xx, yy, method)
%input
%   x= independent variable
%   y= independent variable
%   z = dependent variable= dependent variable
%   xx = value of xx at which the interpolation is calculatedat which the interpolation is calculated
%   yy = value of y at which the interpolation is calculated= value of y at which the interpolation is calculated
%   ¡®method¡¯ = argument to select ¡®Newton¡¯ or Lagrange¡¯
%
%output
%   zint = interpolated value of dependent variable




if (nargin <6)
    method = "Lagrange"; % default : lagrange method
end

method = lower(method); %remove case sensitivity


if(method == "lagrange")
% Lagrange Method
n=length(x);

if length(y)~=n, error('x and y, z must be same length');end
if length(z)~=n, error('x and y, z must be same length');end
%error message

s=0; % initialize s 
    yint = zeros(1,n); % for x interpolation
 for k = 1:n   
    for i = 1:n 
        product = z(k,i);
        for j = 1:n 
           if i~=j % if i=j -> error
               product = ((xx-x(j))/(x(i)-x(j)))*product;
           end
        end
         s=s+product; %lagrange
    end
    yint(1,k) = s;%save f(xx, yy)
    s=0;
 end
 
 s=0;% initialize
for i = 1:n
        product = yint(1,i);
        for j = 1:n
           if i~=j
               product = ((yy-y(j))/(y(i)-y(j)))*product;
           end
        end
         s=s+product; %Lagrange method
end
    zint = s; %% complete : zint = f(xx,yy) 
end



if(method=="newton")
    %Newtion Method
 
n = length(x);
if length(y)~=n, error('x and y must be same length'); end
if length(z)~=n, error('x and y must be same length'); end
%error method
 A= zeros(n,n,n); %3-dimentional matrix
for k = 1:n
   
b = zeros(n,n);
b(:,1) = z(k,:);
    for j=2:n
      for i= 1:n-j+1
         b(i,j) = (b(i+1,j-1)-b(i,j-1))/(x(i+j-1)-x(i));
      end
    end
    A(:,:,k) = b;
end
yint = zeros(1:n);

for k=1:n
xt=1;
yint(1,k) = A(1,1,k);
    for j = 1:n-1
     xt=xt*(xx-x(j));
     yint(1,k)=yint(1,k)+A(1,j+1,k)*xt;
    end
end


b = zeros(n,n);
b(:,1) = yint(1,:);

for j=2:n 
    for i= 1:n-j+1
        b(i,j) = (b(i+1,j-1)-b(i,j-1))/(y(i+j-1)-y(i));
    end
end

yt=1; 
zint = b(1,1);
for j = 1:n-1
    yt=yt*(yy-y(j));
    zint=zint+b(1,j+1)*yt;
end   
    
end

    
    

    
    
    
    
    
    
    
    
    
    
    
    

