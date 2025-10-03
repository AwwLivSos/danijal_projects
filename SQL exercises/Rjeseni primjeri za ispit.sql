   /*zadaci iz vjezbe*/

SELECT s.indeks, b.bodovi ,
CASE
WHEN b.bodovi BETWEEN 1 AND 5 THEN 'D'
WHEN b.bodovi BETWEEN 6 AND 10 THEN 'C'           /*umjesto decode ovdje je case, koji je vjv bolji za ove between operacije*/
WHEN b.bodovi BETWEEN 11 AND 15 THEN 'B'
WHEN b.bodovi BETWEEN 16 AND 20 THEN 'A'
END Kategorija
FROM test.studenti s, test.bodovi b
WHERE s.id=b.sid;

CREATE TABLE osobe2 AS SELECT * FROM test.studenti;    /*kopiranje tsbele*/
SELECT *FROM osobe2;
ALTER TABLE osobe2 ADD infro VARCHAR2 (256);        /*dodali smo neku novu kolonu koja je trenutno prazna*/

SELECT Avg(b.bodovi)
FROM test.studenti s, test.bodovi b, test.odsjeci o
WHERE s.id=b.sid AND s.oid=o.id                     /*prosjek odsjeka*/
GROUP BY o.id;

UPDATE osobe2 SET infro = Decode(oid,
                                1, '5.66',          /*updateanje kolone koju smo dodali*/
                                2, '13.33');

SELECT e.first_name || ' ' || e.last_name "Ime i prezime", b.salary
FROM employees e, employees b
WHERE b.employee_id = e.manager_id
AND b.department_id<=40;

SELECT first_name, last_name, hire_date, Round(Months_Between(SYSDATE, hire_date), 0) Mjeseci
FROM employees
ORDER BY 4;
/*dobar fazon, sortiraj po 4. koloni*/


SELECT d.department_name, Count(*)
FROM departments d, employees e
WHERE e.department_id=d.department_id
GROUP BY d.department_name
HAVING Count (*) IN (3,5) OR Count(*)>10        /*having umjesto where za agregatne, takoðer in funkcija je kul*/
ORDER BY 2, 1;
/*HAVING se korsit da obavis operacije nad grupiranim redovima, recimo ovdje je group by dept_name, pa onda NJEGOV COUNT da je 3. 5 ili veci od 10*/


CREATE TABLE zappy AS SELECT * FROM employees;
SELECT * FROM zappy;
UPDATE zappy SET salary = Decode (department_id,           /*update tabele preko decode*/
                                  10, salary +1000,
                                  30, salary +3000,
                                  40, salary +4000,
                                  80, salary +3500,
                                  100, salary+300,
                                  salary+500);

SELECT f.ime || f.prezime AS "Ime i prezime", Sum(r.iznos)
FROM erd.kupac k, erd.fizicko_lice f, erd.faktura r
WHERE k.kupac_id = f.fizicko_lice_id
AND r.kupac_id = k.kupac_id
AND k.broj_telefona LIKE '%9%9%'           /*ono sto nije u agregatnoj ide u gruop by*/
GROUP BY f.ime, f.prezime ;

DELETE
FROM erd.proizvod p
WHERE p.naziv LIKE '%CD%' OR p.naziv LIKE '%CP%'
AND EXISTS (
            SELECT 1
            FROM pravno_lice pr
            JOIN  ugovor_za_pravno_lice upl
            ON pr.pravno_lice_id = upl.pravno_lice_id
            WHERE pr.pravno_lice_id = p.proizvodjac_id
            GROUP BY pr.pravno_lice_id
            HAVING Count (upl.ugovor_id) > (
                                            SELECT Max (Months_Between(SYSDATE, upl.datum_potpisivanja))
                                            FROM ugovor_za_pravno_lice upl
                                            WHERE upl.pravno_lice_id = pr.pravno_lice_id)
            );


SELECT To_Char(t.datum, 'mm.yyyy') Datum, Count(s.id)
FROM test.Studenti s, test.Sale sa, test.Testovi t, test.Bodovi b
WHERE s.id=b.sid AND b.tid=t.id AND t.said=sa.id AND sa.kapacitet>2
GROUP BY To_Char(t.datum, 'mm.yyyy')
ORDER BY 1 DESC;

