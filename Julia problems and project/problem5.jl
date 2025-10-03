using Pkg; 
Pkg.add("Polynomials"); 
using Plots, Polynomials;

p1=Polynomial([1:2:5;])
p2=fromroots([1:3:10;])
roots(p1)
roots(p2)
coeffs(p1)
coeffs(p2)

#dodacemo komentare sa objasnjenjima kasnije al ovo se cini simplex simplicis

p1+p2
p1-p2
p1*p2
p2 ÷ p1

p=Polynomial([1,2,3],:y)
p(5)
p(5+2*im)
p.([1:5;])

x=variable()
x^2

cheb=ChebyshevT([1, 0, 3, 4])
p = convert(Polynomial, cheb)
Polynomials.domain(p)
x = -10:10
extrema(mapdomain(ChebyshevT, x))

xs = range(0, 10, length=10)
ys = @. exp(-xs)
f = fit(xs, ys) # degree = length(xs) - 1
f2 = fit(xs, ys, 2) # degree = 2

scatter(xs, ys, markerstrokewidth=0, label="Data")
plot!(f, extrema(xs)..., label="Fit")
plot!(f2, extrema(xs)..., label="Quadratic Fit")
