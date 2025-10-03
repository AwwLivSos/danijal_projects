--Medinina filozofija

--prvi zadatak
CREATE TABLE zaptab AS          --ovako mogu individualno selektovat sve sto mi valja iz drugih tabela
SELECT e.employee_id sifra_zaposlenog, e.first_name || ' ' || e.last_name naziv_zaposlenog, d.department_id naziv_odjela, e.job_id sifra_posla, j.job_title naziv_posla, e.salary plata, e.commission_pct dodatak_na_platu
FROM employees e, departments d, jobs j
WHERE e.department_id = d.department_id
AND e.job_id = j.job_id;

SELECT * FROM zaptab;

ALTER TABLE zaptab ADD (id NUMBER);     --cijela sintaksa za dodavanje nove kolone u tabelu, naravno, inicijalno su sve null vrijednosti

CREATE SEQUENCE zapseq
INCREMENT BY 1
START WITH 1
NOMAXVALUE
NOMINVALUE
NOCYCLE
NOCACHE;

SELECT zapseq.CURRVAL FROM dual;       --pri pocetku ona nece raditi jer nije inicirana, prije toga potrebno je odraditi nextval bar jednom
SELECT zapseq.NEXTVAL FROM dual;

UPDATE zaptab                         --ovo je sintaksa za mijenjanje vrijednosti u tabeli, koristili smo sekvecu.nextval da automatski popunimo null kolonu
SET id = zapseq.NEXTVAL;

ALTER TABLE zaptab ADD CONSTRAINT pk_zap_k PRIMARY KEY (id);      --ez postavljanje na primary key

ALTER TABLE zaptab ADD datum_zaposlenja DATE;

UPDATE zaptab z
SET z.datum_zaposlenja = (SELECT e.hire_date
                          FROM employees e
                          WHERE e.employee_id = z.sifra_zaposlenog);  --na ovaj nacin smo postavili datum zaposlenja kopirajuci vrijednosti iz og tabele pomocu podupita, poprilicno intuitivno

CREATE OR REPLACE TRIGGER zaptrig
BEFORE UPDATE OR DELETE ON zaptab --PRIJE nego sto UPDATEAS ili BRISES nesto
REFERENCING OLD AS OLD NEW AS NEW   --wtf??
FOR EACH ROW
BEGIN             --potrebno je to_char, pa onda to_number
 IF (To_Number(to_char(:OLD.datum_zaposlenja, 'yyyy')) = 2008)            --inace je potrebno napisati old i new sa dvotackom ispred
  THEN Raise_Application_Error(-20005, 'Ne moze tako ba');
 END IF;                      --sintaksa za ifove tahtijeva if, pa then, pa end if, sa tackom zarezom na kraju redova, kao u c++
END;

--ubacinaje novog reda u tabelu zahtijeva navoðenje sve i jedne kolone, inace budu null vrijednoasti
--INSERT INTO zaptab VALUES(300, 'David Beckham', 30, 'EA_FC_25', 'FUDBALER', 50000, NULL, zapseq.NEXTVAL, To_Date('02.05.1975', 'dd.mm.yyyy'));   --koristiti sekvencu i paziti na format datuma

UPDATE zaptab
SET plata=plata+10;   --test za trigger, radi za ovako neke specificne komande, ali ako probam za sve onda ce biti error triggera
--inace, cim se aktivira trigger, nece se promjeniti ni jedna vrijednost, bez obzira sto ne spada u argument triggera, mozda je to zato sto je BEFORE trigger

--drugi zadatak
CREATE OR REPLACE TRIGGER zaptrig2
BEFORE UPDATE OR INSERT OR DELETE ON zaptab
REFERENCING OLD AS OLD NEW AS NEW            --when i ovo referencing su optional, sve ostalo je obavezno
FOR EACH ROW
BEGIN
 IF(Lower(To_Char(SYSDATE, 'dy')) IN ('mon', 'tue', 'sun')) AND    --lower je sigurna opcija da uvijek bude sve u laksem formatu za porediti
 To_Date(To_Char(SYSDATE, 'hh24-mi'), 'hh24-mi') BETWEEN
 To_Date('00-01', 'hh24-mi') AND To_Date('23-16', 'hh24-mi')       --belaj je samo zapamtit ove formate sati i minuta jer ide hh24-mi, between radi kako se ocekuje
 THEN Raise_Application_Error(-20005, 'Lose vrijeme izmjene');
 END IF;
END;

--treci zadatak, trigger tabela rokova
CREATE TABLE arhivazap(
sifra_zap NUMBER,
naziv_zap VARCHAR2(100),
naziv_od VARCHAR2(100),
sifra_posla VARCHAR2(100),
plata NUMBER,
dodatak_plata NUMBER,
id NUMBER,
datum_zap DATE,
korisnik VARCHAR2(100),
vrijeme VARCHAR2(100));

SELECT * FROM arhivazap;

CREATE OR REPLACE TRIGGER arhivtrig
AFTER UPDATE ON zaptab                --ovdje se igramo sa fazonima odl i new, ubacujemo izmjene nakon sto se izvrse
REFERENCING OLD AS OLD NEW AS NEW
FOR EACH ROW
BEGIN                                  --ubacujem vrijednosti za tabelu arhive OD ORIGINALNE TABELE ZAPOSLENIH
 INSERT INTO arhivazap VALUES(:old.sifra_zaposlenog, :old.naziv_zaposlenog, :old.naziv_odjela, :old.sifra_posla, :old.plata, :old.dodatak_na_platu, :old.id, :old.datum_zaposlenja, USER, To_Char(SYSDATE, 'hh24:mi'));
END;                               --old ipak referenicra tabelu nad kojom se vrse izmjene

--sad moramo uklonit stare triggere jer oni trenutno onemogucavaju izmjene nad zaptab
DROP TRIGGER zaptrig;
DROP TRIGGER zaptrig2;

UPDATE zaptab
SET plata=plata+10;

SELECT * FROM arhivazap;  --sada se ovdje nalaze nove updatanje vrijednosti plus korisnika koji je izmjenio tabelu (ja) i datum (vrijeme) izmjene

--cetvrti zadatak
CREATE TABLE odtab AS SELECT * FROM departments;
CREATE TABLE jobtab AS SELECT * FROM  jobs;

ALTER TABLE odtab
ADD (id NUMBER, korisnik VARCHAR2(50), datum DATE);

ALTER TABLE jobtab
ADD (id NUMBER, korisnik VARCHAR2(50), datum DATE);

ALTER TABLE odtab
ADD CONSTRAINT id_pk_jb PRIMARY KEY (id);   --nododana kolona u novostvorenoj tabeli ima sve null vrijednosti pa nije moguce odmah ih staviti kao pk
--ideja je da je prvo popunimo sekvencom

UPDATE odtab SET id=zapseq.NEXTVAL;
UPDATE jobtab SET id=zapseq.NEXTVAL;

ALTER TABLE jobtab
ADD CONSTRAINT id_pk_od PRIMARY KEY (id);

ALTER TABLE zaptab ADD (id_odjela NUMBER, id_posla NUMBER);

SELECT * FROM jobtab;

DROP TRIGGER zaptrig;

UPDATE zaptab z
SET z.id_odjela=(SELECT o.id
                 FROM odtab o                          --ovime smo postavili da su id-evi na odjel zaposlenog poklapajuci sa njihovim stvarnim ide-evima
                 WHERE o.department_id=z.naziv_odjela);

UPDATE zaptab z
SET z.id_posla=(SELECT o.id
                 FROM jobtab o                          --ovime smo postavili da su id-evi na posao zaposlenog poklapajuci sa njihovim stvarnim ide-evima
                 WHERE o.job_id=z.sifra_posla);

ALTER TABLE zaptab
ADD CONSTRAINT job_fk_z FOREIGN KEY (id_posla) REFERENCES jobtab (id);

ALTER TABLE zaptab
ADD CONSTRAINT od_fk_z FOREIGN KEY (id_odjela) REFERENCES odtab (id);

CREATE SEQUENCE odseq
INCREMENT BY 1
START WITH 141    --od pocetka id-eva
NOMAXVALUE
NOMINVALUE
NOCACHE
NOCYCLE;

CREATE SEQUENCE jobseq
INCREMENT BY 1
START WITH 160
NOMAXVALUE
NOMINVALUE
NOCACHE
NOCYCLE;

CREATE TABLE odprob AS SELECT * FROM departments;  --nejasno

CREATE OR REPLACE TRIGGER odtrigg
AFTER INSERT ON odprob
REFERENCING OLD AS OLD NEW AS NEW
FOR EACH ROW
BEGIN
 INSERT INTO odtab VALUES (:new.department_id, :new.department_name, :new.location_id, :new.manager_id, odseq.NEXTVAL, USER, SYSDATE);
END;

CREATE TABLE jobprob AS SELECT * FROM jobs;

CREATE OR REPLACE TRIGGER jobtrigger
AFTER INSERT ON jobprob
REFERENCING OLD AS OLD NEW AS NEW
FOR EACH ROW
BEGIN
 INSERT INTO jobtab VALUES (:new.job_id, :new.job_title, :new.min_salary, :new.max_salary, jobseq.NEXTVAL, USER, SYSDATE);
END;

INSERT INTO odprob VALUES(382,'Admin',258,7582);  --ne radi bas al sta cu mu ja, nije do triggera, samo je belaj sto je tesko unijeti vrijednosti u tabelu koja ima pk i fk

--moj bullshit
SELECT * FROM MojiZaposleni3;

CREATE TABLE ZaposleniMoji (
    employee_id NUMBER PRIMARY KEY,
    employee_name VARCHAR2(100),
    department_name VARCHAR2(100),
    job_id NUMBER,
    job_title VARCHAR2(100),
    salary NUMBER,
    bonus NUMBER,
    hire_date DATE
);

CREATE OR REPLACE TRIGGER prevent_update
BEFORE UPDATE OR DELETE ON ZaposleniMoji
FOR EACH ROW      --ovo new znaci da se radi o vrijednosti tabele/kolone nakon promjene, alternativno koristi se OLD, za vrijednosti prije mijenjanja
WHEN (NEW.hire_date >= To_Date('2008', 'yyyy'))             --ovdje se moze ubaciti neka simpl projvera prije tijela triggera da vidimo da li ce uopste uci u njega
BEGIN
 Raise_Application_Error(-20001, 'Zabranjeno mijenjanje stanja zaposlenih poslije 2008!');
END;

CREATE OR REPLACE TRIGGER radni_dani
BEFORE UPDATE OR DELETE OR INSERT ON ZaposleniMoji
FOR EACH ROW                                      --ovdje necemo vrsiti nikakvu provjeru jer je malo kompleksnija pa cemo je samo ubaciti u tijelo
BEGIN
 NULL;
END;