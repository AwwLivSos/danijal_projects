
#=

       Vjezba 2

=#

#Zadatak 1
import Pkg; 
Pkg.add("Plots")
using Plots
x=[-pi:pi/50:pi;];
y=sin.(x)
plot(x,y,title = "Sinus", label = "sin(x)")
xlabel!("x")
ylabel!("y")

x=range(-pi,stop=pi,length=101);
y=cos.(x);
plot(x,y,title = "Kosinus", label = "cos(x)")
xlabel!("x")
ylabel!("y")

x=range(-pi, π, length = 101)
y=[sin.(x) cos.(x)]
plot(x,y,shape = [:circle :star5], label = ["sin(x)" "cos(x)"])
title!("Sinus i kosinus")

x=range(-pi, π, length = 101)
y1=sin.(x);
y2=cos.(x);
p1=plot(x,y1,title="Sinus",shape=:circle,label="sin(x)");
p2=plot(x,y2,title="Kosinus",color=:red,shape=:star5,label="cos(x)");
plot(p1,p2,layout=(1,2))

#Zadatak 2
x, y = -8:0.5:8, -8:0.5:8
z(x,y) = sin.(sqrt.(x.^2+y.^2))
surface(x,y,z, st=:surface)

#=

        Julia funkcije

=#

#Zadatak 3a
function zbir_razlika(x=0,y=0)
    if (size(x)===size(y))
     x + y, x-y
    else return (0,0)
    end
end

zbir_razlika([2;2])
zbir_razlika([1 2 3; 4 5 6],[1 2 3])
a,b=zbir_razlika(5,8)
zbir_razlika([1 2 3; 1 2 3], [4 5 6; 4 5 6])
zbir_razlika()

#Zadatak 3b
function sume(mat)
    suma_matrice=0
    suma_redova=zeros(size(mat,1))
    suma_kolona=zeros(size(mat,2))
    suma_diagonale=0
    suma_sporedne=0
    for i = 1:size(mat,1)
        for j = 1:size(mat,2)
           suma_matrice+=mat[i,j]
           suma_redova[i]+=mat[i,j]
           suma_kolona[j]+=mat[i,j]
           if size(mat,1) == size(mat,2)
              if i==j
                  suma_diagonale+=mat[i,j]
              end
              if (i + j) == (size(mat,1)+1)
                    suma_sporedne += mat[i,j]; 
              end
           end
        end
    end
    return suma_matrice,suma_redova,suma_kolona, suma_diagonale, suma_sporedne
end

sume([1 2 3; 4 5 6; 7 8 9])
a=[2 7 6; 9 5 1; 4 3 8];
sume(a)
sume(a[2,:])
sume(a[1:2,1:2])
