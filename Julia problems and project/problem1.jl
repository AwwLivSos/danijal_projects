#Zadatak 1
#=

        Uvod u Juliu

=#

                                                #mozemo samo napisati neki brojcani izraz i ispisace se na ekran

3*456/23+31.54+2^6                              #sabiranje, oduzimanje, mnozenje i dijeljenje je na isti fazon, zarez također, samo stepenovanje se sada radi sa ^
sin(pi/7)*exp(0.3)*2(2+0.9*im)                  #sinus je jasan, konstanta "pi" se pise upravo tako, stepenovanje konstante e se radi na principu "exp(broj)", a imaginarna jedinica je "im"
sqrt(2)*log(10)                                 #korijen je jasan, log je ovdje prirodni logaritam, kao u c++
(5+3*im)/(1.2+4.5im)

#Zadatak 2                                      #ovdje dodjeljujemo izraze varijablama koje nemaju tipove

a=(atan(5)+exp(5.6))/3                          #arcus tangens i stepenovanje sa e                 
b=sin(pi/3)^(1/15)                              #petnaesti korijen, odrađen na fazon ^(1/n)
c=(log(15)+1)/23                                
d=sin(pi/2)+cos(pi)

(a+b)*c                                         #zatim se ovdje mogu odraditi jos druge razne operacije sa varijablama
acos(b)*asin(c/11)
(a-b)^4/d
c^(1/a)+b*im/(3+2*im)



#Zadatak 3
using LinearAlgebra                             #za matrice

A=[1 -4*im sqrt(2); log(Complex(-1)) sin(pi/2) cos(pi/3); asin(0.5) acos(0.8) exp(0.8)]                 #elementi istog reda se odvajaju RAZMAKOM, a kada prelazimo u novi red matrice onda se stavi ;
A'
transpose(A)                                    #obje ove komande su za transponovanje
A+transpose(A)
A*transpose(A)
transpose(A)*A
det(A)                                          #determinanta matrice
inv(A)                                          #inverzna matrica
pinv(A)                                         #također komanda za inverznu, nisam siguran koja je razlika za sada

#Zadatak 4                                      #automatsko generiranje matrica

zeros(8,9)                                      #matrica sa svim nulama
ones(7,5)                                       #matrica sa svim jedinicama
I(5)                                            #jedinicna matrica
I(5)+zeros(5,5)                                 #sabiranje matrica
randn(4,9)                                      #random matrica

#Zadatak 5                                      
a=[2 7 6; 9 5 1; 4 3 8]
sum(a)                                          #zbir svih elemenata matrice                        
sum(a,dims=1)                                   #zbir svih elemenata svake kolone
sum(a,dims=2)                                   #zbir svih elemenata svakog reda
tr(a)
tr(rotl90(a))
minimum(a,dims=1)                               #minimalni elementi kolona
minimum(a,dims=2)                               #minimalni elementi redova
minimum(diag(a))                                #minimalni element na dijagonali
minimum(diag(rotl90(a)))                        #minimalni element na drugoj dijagonali  
maximum(a,dims=1)
maximum(a,dims=2)
maximum(diag(a))
maximum(diag(rotl90(a)))

#Zadatak 6
a=[1 2 3; 4 5 6; 7 8 9]
b=[1 1 1; 2 2 2; 3 3 3]
c=sin.(a)                                       #matrica c je sastavljena od sinusa svih elemenata matrice a
c=sin.(a).*cos.(b)                              #matrica c je sastavljena od sinusa svih elemenata matrice a pomnozenih sa kosinusom svih elemenata matrice b
c=(a^3)^(1/3)                                   #matrica c je treci korijen od matrice a (???)
c=(a.^3).^(1/3)                                 #matrica c je sastavljena od treceg korijena svih elemenata matrice a


#Zadatak 7                                      #vektori
0:99                                            #vektor red ciji su elementi od 0 do 99, ima naravno 100 elemenata
[0:99;]'                                        #ovo radi isto ali u formatu matrice                              
[0:0.01:0.99;]'                                 #vektor red ciji su elementi 0, pa od 0.01 do 0.99
[39:-2:1;]                                      #vektor KOLONU koja ide od 39 do 1, sa korakom od 2 (tj -2)
#ovo su zapravo SVE VEKTORI KOLONE kada se nalaze u srednjim zagradama [], ali kada smo stavili ' iza zagrade, to znaci da je vektor transponovan, pa postaje vektor red

#Zadatak 8                                      #dodavanje matrica u druge matrice
a=[7*ones(4,4) zeros(4,4); 3*ones(4,8)]         #matrici a je dodijeljeno da je sastavljena od 4x4 matrice sedmica, pa je pored nje dodana 4x4 matrica nulama
                                                #ovo cini 4x8 matricu gdje je lijeva strana samo sedmice, a desna samo nule
                                                #zatim, kada smo dodali ";" prelazimo u novi red, te matrici a dodajemo 4x8 matricu trica, sto cini 8x8 matricu sa gornjom stranom kao sto je vec opisana, a donja strana su sve trice
b=I(8)+a                                        
c=b[1:2:8,:]                                    #matrici c smo dodijelili matricu b, pri cemu smo izostavili svaki drugi red
d=b[:,1:2:8]                                    #matrici d smo dodijelili matricu b, pri cemu smo izostavili svaku drugu kolonu, zarez je kljucan u oba zapisa
e=b[1:2:8,1:2:8]                                #matrici e smo dodijelili matricu b, pri cemu smo izostavili svaki drugi red i svaku drugu kolonu
c[2,:] = c[2,:] *2                              #druga kolona = druga kolona pomnozena s 2