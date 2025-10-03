--zadatak 1
ALTER SESSION SET CURRENT_SCHEMA = erd;

--1
SELECT DISTINCT pl.naziv resnaziv
FROM pravno_lice pl, lokacija l, fizicko_lice fl
WHERE pl.lokacija_id = l.lokacija_id
AND fl.lokacija_id = l.lokacija_id;

SELECT Sum(Length(ResNaziv)*3) FROM
(SELECT DISTINCT pl.naziv resnaziv
FROM pravno_lice pl, lokacija l, fizicko_lice fl
WHERE pl.lokacija_id = l.lokacija_id
AND fl.lokacija_id = l.lokacija_id);

SELECT Sum(Length(ResNaziv)*7) FROM
(SELECT DISTINCT pl.naziv resnaziv
FROM pravno_lice pl, lokacija l, fizicko_lice fl
WHERE pl.lokacija_id = l.lokacija_id
AND fl.lokacija_id = l.lokacija_id);

--2
SELECT DISTINCT To_Char(upl.datum_potpisivanja, 'dd.MM.yyyy') "Datum Potpisivanja", pl.naziv resnaziv
FROM ugovor_za_pravno_lice upl, pravno_lice pl
WHERE pl.pravno_lice_id = upl.pravno_lice_id
AND upl.datum_potpisivanja > (SELECT Min(f.datum_kupoprodaje)
                              FROM faktura f, narudzba_proizvoda np, proizvod p
                              WHERE f.faktura_id = np.faktura_id
                              AND np.proizvod_id=p.proizvod_id
                              AND p.broj_mjeseci_garancije IS NOT NULL);


SELECT Sum(Length(ResNaziv)*3 + Length("Datum Potpisivanja")*3) FROM
(SELECT DISTINCT To_Char(upl.datum_potpisivanja, 'dd.MM.yyyy') "Datum Potpisivanja", pl.naziv resnaziv
FROM ugovor_za_pravno_lice upl, pravno_lice pl
WHERE pl.pravno_lice_id = upl.pravno_lice_id
AND upl.datum_potpisivanja > (SELECT Min(f.datum_kupoprodaje)
                              FROM faktura f, narudzba_proizvoda np, proizvod p
                              WHERE f.faktura_id = np.faktura_id
                              AND np.proizvod_id=p.proizvod_id
                              AND p.broj_mjeseci_garancije IS NOT NULL));

SELECT Sum(Length(ResNaziv)*7 + Length("Datum Potpisivanja")*7) FROM
(SELECT DISTINCT To_Char(upl.datum_potpisivanja, 'dd.MM.yyyy') "Datum Potpisivanja", pl.naziv resnaziv
FROM ugovor_za_pravno_lice upl, pravno_lice pl
WHERE pl.pravno_lice_id = upl.pravno_lice_id
AND upl.datum_potpisivanja > (SELECT Min(f.datum_kupoprodaje)
                              FROM faktura f, narudzba_proizvoda np, proizvod p
                              WHERE f.faktura_id = np.faktura_id
                              AND np.proizvod_id=p.proizvod_id
                              AND p.broj_mjeseci_garancije IS NOT NULL));

--3
SELECT p.naziv
FROM proizvod p
where p.kategorija_id =(SELECT p2.kategorija_id
                        FROM proizvod p2, kolicina k
                        WHERE k.proizvod_id = p2.proizvod_id
                        GROUP BY p2.kategorija_id
                        HAVING Sum(k.kolicina_proizvoda) = (SELECT Max(suma)
                                                            FROM (SELECT Sum(k2.kolicina_proizvoda) AS suma
                                                                  FROM proizvod p3, kolicina k2
                                                                  WHERE p3.proizvod_id=k2.proizvod_id
                                                                  GROUP BY p3.kategorija_id)
                                                           )
                        );

SELECT Sum(Length(naziv)*3) FROM
(SELECT p.naziv
FROM proizvod p
where p.kategorija_id =(SELECT p2.kategorija_id
                        FROM proizvod p2, kolicina k
                        WHERE k.proizvod_id = p2.proizvod_id
                        GROUP BY p2.kategorija_id
                        HAVING Sum(k.kolicina_proizvoda) = (SELECT Max(suma)
                                                            FROM (SELECT Sum(k2.kolicina_proizvoda) AS suma
                                                                  FROM proizvod p3, kolicina k2
                                                                  WHERE p3.proizvod_id=k2.proizvod_id
                                                                  GROUP BY p3.kategorija_id)
                                                           )
                        ));

SELECT Sum(Length(naziv)*7) FROM
(SELECT p.naziv
FROM proizvod p
where p.kategorija_id =(SELECT p2.kategorija_id
                        FROM proizvod p2, kolicina k
                        WHERE k.proizvod_id = p2.proizvod_id
                        GROUP BY p2.kategorija_id
                        HAVING Sum(k.kolicina_proizvoda) = (SELECT Max(suma)
                                                            FROM (SELECT Sum(k2.kolicina_proizvoda) AS suma
                                                                  FROM proizvod p3, kolicina k2
                                                                  WHERE p3.proizvod_id=k2.proizvod_id
                                                                  GROUP BY p3.kategorija_id)
                                                           )
                        ));

--4
SELECT p.naziv AS "Proizvod", pl.naziv AS "Proizvodjac"
FROM proizvod p, proizvodjac pz, pravno_lice pl
WHERE p.proizvodjac_id = pz.proizvodjac_id
AND pz.proizvodjac_id = pl.pravno_lice_id
AND EXISTS (SELECT *
            FROM proizvod p2
            WHERE p2.proizvodjac_id = pz.proizvodjac_id
            AND p2.cijena > (SELECT Avg(p3.cijena)
                             FROM proizvod p3)
           );

SELECT Sum(Length("Proizvod")*3 + Length("Proizvodjac")*3) FROM
(SELECT p.naziv AS "Proizvod", pl.naziv AS "Proizvodjac"
FROM proizvod p, proizvodjac pz, pravno_lice pl
WHERE p.proizvodjac_id = pz.proizvodjac_id
AND pz.proizvodjac_id = pl.pravno_lice_id
AND EXISTS (SELECT *
            FROM proizvod p2
            WHERE p2.proizvodjac_id = pz.proizvodjac_id
            AND p2.cijena > (SELECT Avg(p3.cijena)
                             FROM proizvod p3)
           )
);

SELECT Sum(Length("Proizvod")*7 + Length("Proizvodjac")*7) FROM
(SELECT p.naziv AS "Proizvod", pl.naziv AS "Proizvodjac"
FROM proizvod p, proizvodjac pz, pravno_lice pl
WHERE p.proizvodjac_id = pz.proizvodjac_id
AND pz.proizvodjac_id = pl.pravno_lice_id
AND EXISTS (SELECT *
            FROM proizvod p2
            WHERE p2.proizvodjac_id = pz.proizvodjac_id
            AND p2.cijena > (SELECT Avg(p3.cijena)
                             FROM proizvod p3)
           )
);

--5
SELECT fl.ime || ' ' || fl.prezime AS "Ime i prezime", Round(Sum(f.iznos), 2) AS "iznos"
FROM fizicko_lice fl, kupac k, uposlenik u, faktura f
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND u.uposlenik_id = k.kupac_id
GROUP BY fl.ime, fl.prezime
HAVING Round(Sum(f.iznos), 2) > (SELECT Round(Avg(tab.ukupni_iznos), 2)
                                 FROM (SELECT fl2.ime, fl2.prezime, Sum(f2.iznos) AS ukupni_iznos
                                       FROM fizicko_lice fl2, kupac k2, faktura f2
                                       WHERE fl2.fizicko_lice_id = k2.kupac_id
                                       AND k2.kupac_id = f2.kupac_id
                                       GROUP BY fl2.ime, fl2.prezime
                                  ) tab
                                );

SELECT Sum(Length("Ime i prezime")*3 + "iznos"*3) FROM
(SELECT fl.ime || ' ' || fl.prezime AS "Ime i prezime", Round(Sum(f.iznos), 2) AS "iznos"
FROM fizicko_lice fl, kupac k, uposlenik u, faktura f
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND u.uposlenik_id = k.kupac_id
GROUP BY fl.ime, fl.prezime
HAVING Round(Sum(f.iznos), 2) > (SELECT Round(Avg(tab.ukupni_iznos), 2)
                                 FROM (SELECT fl2.ime, fl2.prezime, Sum(f2.iznos) AS ukupni_iznos
                                       FROM fizicko_lice fl2, kupac k2, faktura f2
                                       WHERE fl2.fizicko_lice_id = k2.kupac_id
                                       AND k2.kupac_id = f2.kupac_id
                                       GROUP BY fl2.ime, fl2.prezime
                                  ) tab
                                ));

SELECT Sum(Length("Ime i prezime")*7 + "iznos"*7) FROM
(SELECT fl.ime || ' ' || fl.prezime AS "Ime i prezime", Round(Sum(f.iznos), 2) AS "iznos"
FROM fizicko_lice fl, kupac k, uposlenik u, faktura f
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND u.uposlenik_id = k.kupac_id
GROUP BY fl.ime, fl.prezime
HAVING Round(Sum(f.iznos), 2) > (SELECT Round(Avg(tab.ukupni_iznos), 2)
                                 FROM (SELECT fl2.ime, fl2.prezime, Sum(f2.iznos) AS ukupni_iznos
                                       FROM fizicko_lice fl2, kupac k2, faktura f2
                                       WHERE fl2.fizicko_lice_id = k2.kupac_id
                                       AND k2.kupac_id = f2.kupac_id
                                       GROUP BY fl2.ime, fl2.prezime
                                  ) tab
                                ));

--6
SELECT pl.naziv AS "naziv"
FROM narudzba_proizvoda np1, faktura f1, isporuka i1, kurirska_sluzba ks1, pravno_lice pl
WHERE np1.faktura_id = f1.faktura_id
AND f1.isporuka_id = i1.isporuka_id
AND i1.kurirska_sluzba_id = ks1.kurirska_sluzba_id
AND ks1.kurirska_sluzba_id = pl.pravno_lice_id
AND np1.popust_id IS NOT NULL
GROUP BY ks1.kurirska_sluzba_id, pl.naziv
HAVING Sum(np1.kolicina_jednog_proizvoda) = (SELECT Max(total_kol)
                                            FROM (SELECT Sum(np2.kolicina_jednog_proizvoda) AS total_kol        --bukv samo ponovi upit odozgo haman
                                                  FROM narudzba_proizvoda np2, faktura f2, isporuka i2, kurirska_sluzba ks2
                                                  WHERE np2.faktura_id = f2.faktura_id
                                                  AND f2.isporuka_id = i2.isporuka_id
                                                  AND i2.kurirska_sluzba_id = ks2.kurirska_sluzba_id
                                                  AND np2.popust_id IS NOT NULL
                                                  GROUP BY  ks2.kurirska_sluzba_id)
                                            );

SELECT Sum(Length("naziv")*3) FROM
(SELECT pl.naziv AS "naziv"
FROM narudzba_proizvoda np1, faktura f1, isporuka i1, kurirska_sluzba ks1, pravno_lice pl
WHERE np1.faktura_id = f1.faktura_id
AND f1.isporuka_id = i1.isporuka_id
AND i1.kurirska_sluzba_id = ks1.kurirska_sluzba_id
AND ks1.kurirska_sluzba_id = pl.pravno_lice_id
AND np1.popust_id IS NOT NULL
GROUP BY ks1.kurirska_sluzba_id, pl.naziv
HAVING Sum(np1.kolicina_jednog_proizvoda) = (SELECT Max(total_kol)
                                            FROM (SELECT Sum(np2.kolicina_jednog_proizvoda) AS total_kol        --bukv samo ponovi upit odozgo haman
                                                  FROM narudzba_proizvoda np2, faktura f2, isporuka i2, kurirska_sluzba ks2
                                                  WHERE np2.faktura_id = f2.faktura_id
                                                  AND f2.isporuka_id = i2.isporuka_id
                                                  AND i2.kurirska_sluzba_id = ks2.kurirska_sluzba_id
                                                  AND np2.popust_id IS NOT NULL
                                                  GROUP BY  ks2.kurirska_sluzba_id)
                                            ));

SELECT Sum(Length("naziv")*7) FROM
(SELECT pl.naziv AS "naziv"
FROM narudzba_proizvoda np1, faktura f1, isporuka i1, kurirska_sluzba ks1, pravno_lice pl
WHERE np1.faktura_id = f1.faktura_id
AND f1.isporuka_id = i1.isporuka_id
AND i1.kurirska_sluzba_id = ks1.kurirska_sluzba_id
AND ks1.kurirska_sluzba_id = pl.pravno_lice_id
AND np1.popust_id IS NOT NULL
GROUP BY ks1.kurirska_sluzba_id, pl.naziv
HAVING Sum(np1.kolicina_jednog_proizvoda) = (SELECT Max(total_kol)
                                            FROM (SELECT Sum(np2.kolicina_jednog_proizvoda) AS total_kol        --bukv samo ponovi upit odozgo haman
                                                  FROM narudzba_proizvoda np2, faktura f2, isporuka i2, kurirska_sluzba ks2
                                                  WHERE np2.faktura_id = f2.faktura_id
                                                  AND f2.isporuka_id = i2.isporuka_id
                                                  AND i2.kurirska_sluzba_id = ks2.kurirska_sluzba_id
                                                  AND np2.popust_id IS NOT NULL
                                                  GROUP BY  ks2.kurirska_sluzba_id)
                                            ));

--7
SELECT fl.ime || ' ' || fl.prezime "Kupac", Sum(po.postotak/100*p.cijena*np.kolicina_jednog_proizvoda) "Usteda", SubStr(USER, 3) AS "Indeks" --nema sanse da jos neko koristi ovaku metodu, osim mene u prosloj zadaci
FROM fizicko_lice fl, kupac k, faktura f, narudzba_proizvoda np, proizvod p, popust po, dual
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id = po.popust_id
GROUP BY fl.ime, fl.prezime;

SELECT Sum(Length("Kupac")*3 + Round("Usteda")*3) FROM
(SELECT fl.ime || ' ' || fl.prezime "Kupac", Sum(po.postotak/100*p.cijena*np.kolicina_jednog_proizvoda) "Usteda", SubStr(USER, 3) AS "Indeks" --nema sanse da jos neko koristi ovaku metodu, osim mene u prosloj zadaci
FROM fizicko_lice fl, kupac k, faktura f, narudzba_proizvoda np, proizvod p, popust po, dual
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id = po.popust_id
GROUP BY fl.ime, fl.prezime);

SELECT Sum(Length("Kupac")*7 + Round("Usteda")*7) FROM
(SELECT fl.ime || ' ' || fl.prezime "Kupac", Sum(po.postotak/100*p.cijena*np.kolicina_jednog_proizvoda) "Usteda", SubStr(USER, 3) AS "Indeks" --nema sanse da jos neko koristi ovaku metodu, osim mene u prosloj zadaci
FROM fizicko_lice fl, kupac k, faktura f, narudzba_proizvoda np, proizvod p, popust po, dual
WHERE fl.fizicko_lice_id = k.kupac_id
AND k.kupac_id = f.kupac_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id = po.popust_id
GROUP BY fl.ime, fl.prezime);

--8
SELECT DISTINCT i.*
FROM isporuka i, faktura f, narudzba_proizvoda np, proizvod p
WHERE i.isporuka_id = f.isporuka_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND p.broj_mjeseci_garancije IS NOT NULL;

SELECT Sum(idisporuke*3 + idkurirske*3) FROM
(SELECT DISTINCT i.isporuka_id idisporuke, i.kurirska_sluzba_id idkurirske  --glupost
FROM isporuka i, faktura f, narudzba_proizvoda np, proizvod p
WHERE i.isporuka_id = f.isporuka_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND p.broj_mjeseci_garancije IS NOT NULL);

SELECT Sum(idisporuke*7 + idkurirske*7) FROM
(SELECT DISTINCT i.isporuka_id idisporuke, i.kurirska_sluzba_id idkurirske  --glupost
FROM isporuka i, faktura f, narudzba_proizvoda np, proizvod p
WHERE i.isporuka_id = f.isporuka_id
AND f.faktura_id = np.faktura_id
AND np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND p.broj_mjeseci_garancije IS NOT NULL);

--9
SELECT p.naziv AS naziv, p.cijena AS cijena
FROM proizvod p
WHERE p.cijena > (SELECT Round(Avg(max_cijena), 2)
                  FROM (SELECT Max(p2.cijena) AS max_cijena
                        FROM proizvod p2
                        GROUP BY p2.kategorija_id
                        )
                  );

SELECT Sum(Length(naziv)*3 + cijena*3) FROM
(SELECT p.naziv AS naziv, p.cijena AS cijena
FROM proizvod p
WHERE p.cijena > (SELECT Round(Avg(max_cijena), 2)
                  FROM (SELECT Max(p2.cijena) AS max_cijena
                        FROM proizvod p2
                        GROUP BY p2.kategorija_id
                        )
                  ));

SELECT Sum(Length(naziv)*7 + cijena*7) FROM
(SELECT p.naziv AS naziv, p.cijena AS cijena
FROM proizvod p
WHERE p.cijena > (SELECT Round(Avg(max_cijena), 2)
                  FROM (SELECT Max(p2.cijena) AS max_cijena
                        FROM proizvod p2
                        GROUP BY p2.kategorija_id
                        )
                  ));

--10
SELECT p.naziv, p.cijena
FROM proizvod p
WHERE p.cijena < ALL (SELECT avg(p2.cijena)
                      FROM proizvod p2, kategorija k
				              WHERE p2.kategorija_id = k.kategorija_id
				              AND k.nadkategorija_id <> p.kategorija_id
				              GROUP BY k.kategorija_id);

SELECT Sum(Length(naziv)*3 + Round(cijena)*3) FROM
(SELECT p.naziv, p.cijena
FROM proizvod p
WHERE p.cijena < ALL (SELECT avg(p2.cijena)
                      FROM proizvod p2, kategorija k
				              WHERE p2.kategorija_id = k.kategorija_id
				              AND k.nadkategorija_id <> p.kategorija_id
				              GROUP BY k.kategorija_id));

SELECT Sum(Length(naziv)*7 + Round(cijena)*7) FROM
(SELECT p.naziv, p.cijena
FROM proizvod p
WHERE p.cijena < ALL (SELECT avg(p2.cijena)
                      FROM proizvod p2, kategorija k
				              WHERE p2.kategorija_id = k.kategorija_id
				              AND k.nadkategorija_id <> p.kategorija_id
				              GROUP BY k.kategorija_id));

--zadatak 2

CREATE TABLE TabelaA(id NUMBER PRIMARY KEY,
                     naziv VARCHAR2(10),
                     datum DATE,
                     cijelibroj NUMBER,
                     realnibroj NUMBER CHECK (realnibroj>5),
                     CONSTRAINT chk_zbroj_tabA CHECK (cijelibroj NOT BETWEEN 5 AND 15)
                     );

DROP TABLE TabelaA;
SELECT * FROM TabelaA;

INSERT INTO TabelaA VALUES (1, 'tekst', NULL, NULL, 6.2);
INSERT INTO TabelaA VALUES (2, NULL, NULL, 3, 5.26);
INSERT INTO TabelaA VALUES (3, 'tekst', NULL, 1, NULL);
INSERT INTO TabelaA VALUES (4, NULL, NULL, NULL, NULL);
INSERT INTO TabelaA VALUES (5, 'tekst', NULL, 16, 6.78);


CREATE TABLE TabelaB(id NUMBER PRIMARY KEY,
                     naziv VARCHAR2(10),
                     datum DATE,
                     cijelibroj NUMBER UNIQUE,
                     realnibroj NUMBER,
                     FKTabelaA NUMBER NOT NULL,
                     CONSTRAINT FkCnst FOREIGN KEY (FKTabelaA) REFERENCES TabelaA(id)
                     );

DROP TABLE TabelaB;
SELECT * FROM TabelaB;

INSERT INTO TabelaB VALUES (1, NULL, NULL, 1, NULL, 1);
INSERT INTO TabelaB VALUES (2, NULL, NULL, 3, NULL, 1);
INSERT INTO TabelaB VALUES (3, NULL, NULL, 6, NULL, 2);
INSERT INTO TabelaB VALUES (4, NULL, NULL, 11, NULL, 2);
INSERT INTO TabelaB VALUES (5, NULL, NULL, 22, NULL, 3);


CREATE TABLE TabelaC(id NUMBER PRIMARY KEY,
                     naziv varchar2(10) NOT NULL,
                     datum DATE,
                     cijelibroj NUMBER NOT NULL,
                     realnibroj NUMBER,
                     FKTabelaB NUMBER,
                     CONSTRAINT fk_tc_tb FOREIGN KEY (FKTabelaB) REFERENCES TabelaB(id)
                     );

DROP TABLE TabelaC;
SELECT * FROM TabelaC;

INSERT INTO TabelaC VALUES (1, 'YES', NULL, 33, NULL, 4);
INSERT INTO TabelaC VALUES (2, 'NO', NULL, 33, NULL, 2);
INSERT INTO TabelaC VALUES (3, 'NO', NULL, 55, NULL, 1);

--provjere
INSERT INTO TabelaA (id, naziv, datum, cijeliBroj, realniBroj) VALUES (6, 'tekst', null, null,6.20);  --ubacuje se bez problema, cijelibroj moze biti null, a realni nije manji od 5
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (6, null, null, 1, null, 1);  --ne moze jer je cijelibroj unique
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (7, null, null, 123, null, 6); --ide lagano, cijelibroj je jedinstven, s fk za tabelu anije null
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (4, 'NO', null, 55, null, null);  --takoðer, cijeli broj nije null, mada fk za tabelu b jest, ali nigdje ne pise da se to ne smije
UPDATE TabelaA SET naziv = 'tekst' WHERE naziv IS NULL AND cijeliBroj IS NOT NULL; --trebao bi se samo drugi red promjeniti? Da, izvrsava se i ovo bez problema
DROP TABLE TabelaB; --ne moze se izbrisat jer fkn u tabeli c referencira tabelu b
DELETE FROM TabelaA WHERE realniBroj IS NULL; --takoðer, zbog istih razloga
DELETE FROM TabelaA WHERE id = 5; --ovo je moglo jer nema fk u tabeliB koji referenciraju red u tabeliA sa id-em 5
UPDATE TabelaB SET fktabelaA = 4 WHERE fktabelaA = 2; --ni za ovo nema problema, niko ne referenicra ove vrijednosti, samo su one sada promjenile svoju referencu
ALTER TABLE TabelaA ADD CONSTRAINT cst CHECK (naziv LIKE 'tekst');  --naknadno dodan constarint, nije nikakav problem


SELECT SUM(id) FROM TabelaA; --Rezultat: 16
SELECT SUM(id) FROM TabelaB; --Rezultat: 22
SELECT SUM(id) FROM TabelaC; --Rezultat: 10
--da, da i da

--zadatak 3
SELECT * FROM TabelaA;
SELECT * FROM TabelaB;
SELECT * FROM TabelaC;

CREATE OR REPLACE TRIGGER t1
AFTER INSERT ON TabelaB
FOR EACH ROW
BEGIN
 IF(:new.cijelibroj<50)
 THEN UPDATE TabelaA
      SET realnibroj=realnibroj*1.25
      WHERE id=:new.FKTabelaA;
 ELSE UPDATE TabelaA
      SET realnibroj=realnibroj*0.75
      WHERE id = :new.FKTabelaA;
 END IF;
END;

DROP SEQUENCE brojacB;

CREATE SEQUENCE brojacB
INCREMENT BY 1
START WITH 5
MINVALUE 1
NOCACHE
NOCYCLE;

CREATE OR REPLACE TRIGGER t2
BEFORE INSERT OR UPDATE ON TabelaC
FOR EACH ROW
DECLARE
   najvecicijeli NUMBER;
   fktabelaref NUMBER;          --malo pl/sql zafrkancije
BEGIN
   SELECT Nvl(Max(cijelibroj), 0)
   INTO najvecicijeli
   FROM TabelaB;

   SELECT FKTabelaA
   INTO fktabelaref
   FROM TabelaB
   WHERE id=:new.FKTabelaB;

   INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FKTabelaA)
   VALUES (brojacB.NEXTVAL, NULL, SYSDATE, najvecicijeli*2, NULL, fktabelaref);
END;

DROP SEQUENCE seq;

CREATE SEQUENCE seq
INCREMENT BY 1
START WITH 1
MINVALUE 1
MAXVALUE 10
CYCLE
nocache;

CREATE TABLE TabelaABekap (
   id NUMBER PRIMARY KEY,
   naziv VARCHAR2(50),
   datum DATE,
   cijelibroj NUMBER,
   realnibroj NUMBER,
   cijeliBrojB NUMBER,
   sekvenca NUMBER NOT NULL
);

DROP TABLE TabelaABekap;

CREATE OR REPLACE TRIGGER t3
AFTER INSERT ON TabelaB
FOR EACH ROW
DECLARE
  brojredova NUMBER;
BEGIN
 SELECT COUNT(*)
 INTO brojredova
 FROM TabelaABekap
 WHERE id = :NEW.FkTabelaA;

 IF brojredova>0
 THEN UPDATE TabelaABekap
      SET cijeliBrojB = cijeliBrojB + :new.cijelibroj
      WHERE id = :new.FKTabelaA;
 ELSE INSERT INTO TabelaABekap (id, naziv, datum, cijelibroj, realnibroj, cijeliBrojB, sekvenca)
      SELECT id, naziv, datum, cijeliBroj, realniBroj, :NEW.cijeliBroj, seq.NEXTVAL
      FROM TabelaA, DUAL
      WHERE id=:new.FKTabelaA;
 END IF;
END;

INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 2, null, 1);
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 4, null, 2);
INSERT INTO TabelaB (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaA) VALUES (brojacB.nextval, null,null, 8, null, 1);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (4, 'NO', null, 5, null, 3);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (5, 'YES', null, 7, null,3);
INSERT INTO TabelaC (id, naziv, datum, cijeliBroj, realniBroj, FkTabelaB) VALUES (6, 'NO', null, 9, null, 2);
UPDATE TabelaC SET cijeliBroj = 10 WHERE id = 2;
DELETE FROM TabelaB WHERE id NOT IN (SELECT FkTabelaB FROM TabelaC);
DELETE FROM TabelaA WHERE id IN (3, 4, 6);


SELECT SUM(id*3 + cijeliBrojB*3) FROM TabelaABekap; --Rezultat: 2031
SELECT SUM(id*3 + cijeliBroj*3) FROM TabelaC; --Rezultat: 420
SELECT SUM(MOD(id,10)*3) FROM TabelaB; --Rezultat: 30
SELECT SUM(id + realniBroj)*10 FROM TabelaA; --Rezultat: 264
--da, da, da i dovoljno blizu

SELECT SUM(id*7 + cijeliBrojB*7) FROM TabelaABekap; --4739
SELECT SUM(id*7 + cijeliBroj*7) FROM TabelaC; --980
SELECT SUM(MOD(id,10)*7) FROM TabelaB; --70
SELECT SUM(id*5 + realniBroj)*10 FROM TabelaA; --584 zaokruzeno

SELECT * FROM TabelaA;
SELECT * FROM TabelaB;
SELECT * FROM TabelaC;

INSERT INTO TabelaA (id,naziv,datum,cijeliBroj,realniBroj) VALUES (6,'tekst',null,null,6.20);
INSERT INTO TabelaB (id,naziv,datum,cijeliBroj,realniBroj,FkTabelaA) VALUES (6,null,null,1,null,1);
INSERT INTO TabelaB (id,naziv,datum,cijeliBroj,realniBroj,FkTabelaA) VALUES (7,null,null,123,null,6);
INSERT INTO TabelaC (id,naziv,datum,cijeliBroj,realniBroj,FkTabelaB) VALUES (4,'NO',null,55,null,null);
Update TabelaA set naziv = 'tekst' Where naziv is null and cijeliBroj is not null;
Drop table tabelaB;
Delete from TabelaA where realniBroj is null;
Delete from TabelaA where id = 5;
Update TabelaB set FKTabelaA = 4 where fktabelaA = 2;                 --ne kontam sto se ovo ne moze izvrsit, ima jos par za koje mi nije jano al za ovaj posebno. Skonto ipak, trebalo je resetovat tabelu
Alter Table tabelaA add Constraint cst Check (naziv like 'tekst');

SELECT SUM(id*7 + cijeliBrojB*7) FROM TabelaABekap;
SELECT SUM(id*7 + cijeliBroj*7) FROM TabelaC;
SELECT SUM(MOD(id,10)*7) FROM TabelaB;
SELECT SUM(id*5 + realniBroj)*10 FROM TabelaA; --sad nije isti rezuktat al bio je gore 584, sad je 628, pa eto stavicu ovaj gornji jer on se desio prije ovih dodataka

--uglavnom sad mi nece biti isti rezultati, treba se ponovo resetovati tabela za tacne rezultate, tj one koji su dosli u prvim komentarima