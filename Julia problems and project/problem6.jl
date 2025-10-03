using Pkg; Pkg.add("NumericalIntegration"); Pkg.add("Calculus"); Pkg.add("Interpolations"); Pkg.add("Polynomials");
using NumericalIntegration, Calculus,Interpolations,Polynomials;
Pkg.add("Plots"); using Plots;

p=Polynomial([5,1,3,4])
p1=Polynomials.derivative(p)
p2=Polynomials.derivative(p1)
p3=Polynomials.integrate(p)
p3(2)

Calculus.derivative(sin, 0.0)
Calculus.second_derivative(sin,0.0)

fun = function (x)
    x[1]^3+x[2]^3+x[3]^3
end

fun([2,3,4])
Calculus.gradient(fun, [1,2,3])
Calculus.hessian(fun,[2.0,3.0,4.0])


differentiate("cos(x)*exp(x) + sin(x)", :x)

x = collect(0 : 0.5 : π)
y = sin.(x)
NumericalIntegration.integrate(x,y)

fun1 = function (x)
    x*sin(30x)/(sqrt(Complex(1-(x/2*pi)^2,0)))
end

fun1(2)
x=collect(0:pi/1000:2pi)
y=fun1.(x)
NumericalIntegration.integrate(x, y, SimpsonEven())
x=collect(0:pi/1000:2pi)
y=fun1.(x)
NumericalIntegration.integrate(x, y, TrapezoidalEven())
