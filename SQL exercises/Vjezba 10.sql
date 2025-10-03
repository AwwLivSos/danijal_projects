CREATE TABLE zaposlenji (id NUMBER,
                         ime VARCHAR2 (50)    NOT NULL,
                         prezime VARCHAR2(50) NOT NULL,    --ovo not null za pojedine kolone u tablei su constrainti za kolone, i utjecu samo za unos u kolomnu
                         plata NUMBER         NOT NULL,
                         odjelID NUMBER       NOT NULL,
            CONSTRAINT pk_zap_id PRIMARY KEY (id));       --meðutim, ovo constraint oznacava da se radi o nekom ogranicenju na "globalnom" nivou (nivou cijele tabele), oznacavamo da je id primary key i on dobija naziv pk_zap_id
--moguce je poslije redefinisati ovaj primar kljuc pomocu ovog naziva
--alternativno, moguce je unutar ogranicenja na kolonu staviti da su primary key, iako je to losije, npr id NUMBER PRIMARY KEY

CREATE TABLE odjelji (id NUMBER,
                      menadzer VARCHAR2(50)  NOT NULL,
                      kapacitet NUMBER       NOT NULL,
            CONSTRAINT pk_odj_id PRIMARY KEY (id),
            CONSTRAINT fk_odj_man FOREIGN KEY (menadzer) REFERENCES zaposljenji (id));        --na ovaj nacin pravimo foreign kljuceve, kazem koju tabelu refencira i koju kolonu
--obicno primarni kljuc jedne tabele predstavlja strani kljuc druge tabele

ALTER TABLE zaposlenji ADD CONSTRAINT fk_odj_zap FOREIGN KEY (odjelID) REFERENCES odjelji (id);
--naknadno dodavnja constrainta je moguce kad stavimo alter table, ime tabele, add (ili drop ili disable) constraint, pa onda dalje pisemo kao da je opet na pocetku


CREATE SEQUENCE sekvencinjo  --sintaksa za stvaranje sekvenci, nisam siguran bas za ovo cache i nocache, ali sve ostalo je prilicno intuitivno i logicno
INCREMENT BY 2
START WITH 1
MAXVALUE 200
NOCACHE
NOCYCLE;

SELECT sekvencinjo.nextval FROM dual;   --sekvence ovako mogu samo vratiti current value ili next value i nikada se ne vracaju nazad, tako da su prilicno jednostavne
--upotreba sekvenci u nekom sql upitu je poprilicno nejasna

ALTER SEQUENCE sekvencinjo    --mijenjanje sekvence je moguce odraditi kao i za tabele i sve ostalo, jedini sto se ne moze mijenjati je start with vrijednost
MAXVALUE 10000;


SELECT * FROM employees;

CREATE TABLE MojiZaposleni3 (
    id                      NUMBER(10) PRIMARY KEY,
    sifra_zaposlenog        VARCHAR2(50) NOT NULL,
    ime_zaposlenog          varCHAR2(500),
    prezime_zaposlenog      varCHAR2(500),
    email                   varCHAR2(500),
    broj_telefona           varCHAR2(599),
    datum_zaposlenja        DATE,
    posao_id                varCHAR2(50),
    plata                   NUMBER(10),
    dodatak_na_platu        number(20),
    sef_id                  NUMBER(20),
    odjel_id                number(20) NOT NULL);

SELECT * FROM mojizaposleni3;
DROP TABLE MojiZaposleni2;

CREATE table MojiOdjeli(
    id                  NUMBER(20) PRIMARY KEY,
    datum               DATE,
    odjel_id            NUMBER(20),
    ime_odjela          VARCHAR2(50),
    sef_id              NUMBER(20),
    lokacija_id         NUMBER(29));

ALTER TABLE MojiZaposleni3 ADD CONSTRAINT zap_foreign_1 FOREIGN KEY (odjel_id) REFERENCES MojiOdjeli(id);
ALTER TABLE MojiZaposleni3 ADD CONSTRAINT zap_foreign_2 FOREIGN KEY (sef_id) REFERENCES MojiZaposleni3(id);
ALTER TABLE MojiOdjeli ADD CONSTRAINT odj_foreign_1 FOREIGN KEY (sef_id) REFERENCES MojiZaposleni3(id);

SELECT constraint_name, constraint_type, search_condition
FROM user_constraints
WHERE table_name = 'employees';  --ne valja

show CREATE TABLE employees;  --ne valja

SELECT * FROM user_constraints; --ovo je ok

INSERT INTO MojiZaposleni3(sifra_zaposlenog        ,
                           ime_zaposlenog         ,
                           prezime_zaposlenog    ,
                           email                ,
                           broj_telefona       ,
                           datum_zaposlenja   ,
                           posao_id          ,
                           plata            ,
                           dodatak_na_platu,
                           sef_id      ,
                           odjel_id)
SELECT * FROM employees;

ALTER TABLE MojiZaposleni3
ADD COLUMN plata_dodatak = plata*(1+dodatak_na_platu)
WHERE id = (SELECT employee_id
            FROM employees
            WHERE department_id IN ());

CREATE OR REPLACE VIEW zap_pog (sifra_zaposlenog, naziv_zaposlenog, naziv_odjela)
AS SELECT e.employee_id, e.first_name || ' ' || e.last_name, d.department_name
FROM employees e, departments d
WHERE