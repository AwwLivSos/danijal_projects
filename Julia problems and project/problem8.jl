using Pkg;
Pkg.add("DifferentialEquations")
using DifferentialEquations
function opruga(du,u,p,t)
    du[1] = u[2];
    du[2] = -p[1]*u[1]/p[2]
end

p = [100.0;10.0]
u0 = [0.0;1.0]
tspan = (0.0,10.0)
prob = ODEProblem(opruga,u0,tspan,p)
sol = solve(prob)

plot(sol,vars=(1))
plot(sol,vars=(2))

function dva_tijela(du,u,p,t)
du[1] = u[2];
du[2] = (1/p[1])*(-p[3]*u[1]-p[4]*(u[2]-u[4]));
du[3] = u[4];
du[4]= (p[4]*(u[2]-u[4]))*(1/p[2])
end

p = [10.0;20.0;100.0;30.0]
u0 = [0.0;0.0;0.0;0.0]
tspan = (0.0,10.0)
prob = ODEProblem(dva_tijela,u0,tspan,p)
sol = solve(prob)
plot(sol,vars=(1))
plot!(sol,vars=(3))
plot(sol,vars=(2))
plot!(sol,vars=(4))

p = [10.0;20.0;100.0;30.0]
u0 = [0.0;0.0;1.0;1.0]
tspan = (0.0,10.0)
prob = ODEProblem(dva_tijela,u0,tspan,p)
sol = solve(prob)
plot(sol,vars=(1))
plot!(sol,vars=(3))
plot(sol,vars=(2))
plot!(sol,vars=(4))