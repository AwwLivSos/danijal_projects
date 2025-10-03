CREATE TABLE neka(id           VARCHAR2(25) NOT null,
                  naziv        VARCHAR2(10) NOT null,
                  opis         CHAR(15) NOT null,
                  datum        DATE NOT null,
                  korisnik     VARCHAR2(30) NOT null,
                  napomena     VARCHAR2(10));

CREATE TABLE odjeli AS SELECT * FROM departments;
SELECT * FROM odjeli;

CREATE TABLE zaposleni3 (
    id                      NUMBER(10) NOT NULL,
    sifra_zaposlenog        VARCHAR2(50) NOT NULL,
    naziv_zaposlenog        CHAR(500),
    godina_zaposlenja       NUMBER(10) NOT NULL,
    mjesec_zaposlenja       CHAR(20) NOT NULL,
    sifra_odjela            VARCHAR2(50),
    naziv_odjela            VARCHAR2(150) NOT NULL,
    grad                    CHAR(200) NOT NULL,
    sifra_posla             VARCHAR2(250),
    naziv_posla             CHAR(500),
    iznos_dodatak_na_platu  NUMBER(10),
    plata                   NUMBER(10),
    kontinent               VARCHAR2(200),
    datum_unosa             DATE NOT NULL,
    korisnik_unio           CHAR(200) NOT NULL);

INSERT INTO zaposleni3(id,
                      sifra_zaposlenog,
                      naziv_zaposlenog,
                      godina_zaposlenja,
                      mjesec_zaposlenja,
                      sifra_odjela,
                      naziv_odjela,
                      grad,
                      sifra_posla,
                      naziv_posla,
                      iznos_dodatak_na_platu,
                      plata,
                      kontinent,
                      datum_unosa,
                      korisnik_unio)
            SELECT  e.employee_id,
                    e.employee_id,
                    e.first_name || ' ' || e.last_name,
                    To_Number(To_Char(e.hire_date, 'yyyy')),
                    To_Char(e.hire_date, 'mm'),
                    e.department_id,
                    d.department_name,
                    l.city,
                    e.job_id,
                    j.job_title,
                    e.commission_pct,
                    e.salary,
                    r.region_name,
                    SYSDATE,
                    USER
            FROM employees e, departments d, locations l, countries c, regions r, jobs j
            WHERE e.department_id = d.department_id
            AND e.job_id = j.job_id
            AND d.location_id=l.location_id
            AND l.country_id = c.country_id
            AND c.region_id = r.region_id;

SELECT * FROM zaposleni3;

create TABLE zaposleni4 AS SELECT * FROM zaposleni3;

ALTER TABLE zaposleni4
DROP COLUMN sifra_odjela;
ALTER TABLE zaposleni4
DROP COLUMN sifra_posla;
ALTER TABLE zaposleni4
DROP COLUMN sifra_zaposlenog;

ALTER TABLE zaposleni4 RENAME COLUMN naziv_zaposlenog to zaposleni;
ALTER TABLE zaposleni4 rename COLUMN naziv_posla to posao;
ALTER TABLE zaposleni4 RENAME COLUMN naziv_odjela to odjel;

SELECT * FROM zaposleni4;

RENAME zaposleni4 TO zap_backup;

ALTER TABLE zap_backup SET unused (datum_unosa);
ALTER TABLE zap_backup SET unused (korisnik_unio);

ALTER TABLE zap_backup DROP unused COLUMNS;
