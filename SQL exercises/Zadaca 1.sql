ALTER SESSION SET current_schema = erd;

SELECT Nvl(d.naziv, 'Nema drzave') "Država", Nvl(g.naziv, 'Nema grada') Grad,  k.naziv Kontinent, Count(l.lokacija_id) BrojLokacija
FROM kontinent k
LEFT JOIN drzava d ON d.kontinent_id = k.kontinent_id
LEFT JOIN grad g ON g.drzava_id = d.drzava_id
LEFT JOIN lokacija l ON l.grad_id = g.grad_id
GROUP BY d.naziv, g.naziv, k.naziv;

SELECT Sum(Length(Drzava)*3+Length(Grad)*3+Length(Kontinent)*3)*MAX(BrojLokacija) FROM
(SELECT Nvl(d.naziv, 'Nema drzave') Drzava, Nvl(g.naziv, 'Nema grada') Grad,  k.naziv Kontinent, Count(l.lokacija_id) BrojLokacija
FROM kontinent k
LEFT JOIN drzava d ON d.kontinent_id = k.kontinent_id
LEFT JOIN grad g ON g.drzava_id = d.drzava_id
LEFT JOIN lokacija l ON l.grad_id = g.grad_id
GROUP BY d.naziv, g.naziv, k.naziv);

SELECT Sum(Length(Drzava)*7+Length(Grad)*7+Length(Kontinent)*7)*MAX(BrojLokacija) FROM
(SELECT Nvl(d.naziv, 'Nema drzave') Drzava, Nvl(g.naziv, 'Nema grada') Grad,  k.naziv Kontinent, Count(l.lokacija_id) BrojLokacija
FROM kontinent k
LEFT JOIN drzava d ON d.kontinent_id = k.kontinent_id
LEFT JOIN grad g ON g.drzava_id = d.drzava_id
LEFT JOIN lokacija l ON l.grad_id = g.grad_id
GROUP BY d.naziv, g.naziv, k.naziv);


SELECT pl.naziv naziv
from pravno_lice pl, ugovor_za_pravno_lice uzpl
where uzpl.pravno_lice_id=pl.pravno_lice_id
and uzpl.datum_raskidanja is null
and mod(to_number(to_char(uzpl.datum_potpisivanja, 'yyyy')), 2)=0;

SELECT SUM(LENGTH(naziv)*3) FROM
(SELECT pl.naziv naziv
from pravno_lice pl, ugovor_za_pravno_lice uzpl
where uzpl.pravno_lice_id=pl.pravno_lice_id
and uzpl.datum_raskidanja is null
and mod(to_number(to_char(uzpl.datum_potpisivanja, 'yyyy')), 2)=0);

SELECT SUM(LENGTH(naziv)*7) FROM
(SELECT pl.naziv naziv
from pravno_lice pl, ugovor_za_pravno_lice uzpl
where uzpl.pravno_lice_id=pl.pravno_lice_id
and uzpl.datum_raskidanja is null
and mod(to_number(to_char(uzpl.datum_potpisivanja, 'yyyy')), 2)=0);


SELECT d.naziv Drzava, p.naziv Proizvod, k.Kolicina_proizvoda
FROM kolicina k, skladiste s, proizvod p, lokacija l, grad g, drzava d
WHERE s.skladiste_id = k.skladiste_id
AND k.proizvod_id = p.proizvod_id
AND l.lokacija_id = s.lokacija_id
AND g.grad_id = l.grad_id
AND d.drzava_id = g.drzava_id
AND k.kolicina_proizvoda > 50
AND d.naziv NOT LIKE '%ss%';

SELECT SUM(Length(Drzava)*3 + Length(proizvod)*3 + kolicina_proizvoda*3) FROM
(SELECT d.naziv Drzava, p.naziv Proizvod, k.Kolicina_proizvoda
FROM kolicina k, skladiste s, proizvod p, lokacija l, grad g, drzava d
WHERE s.skladiste_id = k.skladiste_id
AND k.proizvod_id = p.proizvod_id
AND l.lokacija_id = s.lokacija_id
AND g.grad_id = l.grad_id
AND d.drzava_id = g.drzava_id
AND k.kolicina_proizvoda > 50
AND d.naziv NOT LIKE '%ss%');

SELECT SUM(Length(Drzava)*7 + Length(proizvod)*7 + kolicina_proizvoda*7) FROM
(SELECT d.naziv Drzava, p.naziv Proizvod, k.Kolicina_proizvoda
FROM kolicina k, skladiste s, proizvod p, lokacija l, grad g, drzava d
WHERE s.skladiste_id = k.skladiste_id
AND k.proizvod_id = p.proizvod_id
AND l.lokacija_id = s.lokacija_id
AND g.grad_id = l.grad_id
AND d.drzava_id = g.drzava_id
AND k.kolicina_proizvoda > 50
AND d.naziv NOT LIKE '%ss%');


SELECT DISTINCT p.naziv, p.broj_mjeseci_garancije
FROM proizvod p, narudzba_proizvoda np
WHERE np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND Mod(p.broj_mjeseci_garancije, 3)=0;

SELECT Sum(Length(naziv)*3) FROM
(SELECT DISTINCT p.naziv, p.broj_mjeseci_garancije
FROM proizvod p, narudzba_proizvoda np
WHERE np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND Mod(p.broj_mjeseci_garancije, 3)=0);

SELECT Sum(Length(naziv)*7) FROM
(SELECT DISTINCT p.naziv, p.broj_mjeseci_garancije
FROM proizvod p, narudzba_proizvoda np
WHERE np.proizvod_id = p.proizvod_id
AND np.popust_id IS NOT NULL
AND Mod(p.broj_mjeseci_garancije, 3)=0);


SELECT fl.ime  || ' ' || fl.prezime "ime i prezime", o.naziv "Naziv odjela", SubStr(USER, 3) Indeks
FROM fizicko_lice fl, odjel o, uposlenik u, dual
WHERE fl.fizicko_lice_id = u.uposlenik_id
AND u.odjel_id = o.odjel_id
AND u.uposlenik_id != o.sef_id;

SELECT Sum(Length("ime i prezime")*3+Length("Naziv odjela")*3)
FROM
(SELECT fl.ime  || ' ' || fl.prezime "ime i prezime", o.naziv "Naziv odjela", SubStr(USER, 3) Indeks
FROM fizicko_lice fl, odjel o, uposlenik u, dual
WHERE fl.fizicko_lice_id = u.uposlenik_id
AND u.odjel_id = o.odjel_id
AND u.uposlenik_id != o.sef_id);

SELECT Sum(Length("ime i prezime")*7+Length("Naziv odjela")*7)
FROM
(SELECT fl.ime  || ' ' || fl.prezime "ime i prezime", o.naziv "Naziv odjela", SubStr(USER, 3) Indeks
FROM fizicko_lice fl, odjel o, uposlenik u, dual
WHERE fl.fizicko_lice_id = u.uposlenik_id
AND u.odjel_id = o.odjel_id
AND u.uposlenik_id != o.sef_id);


SELECT np.narudzba_id, pr.cijena Cijena, Nvl(po.postotak, 0) Postotak, Nvl(po.postotak, 0)/100 PostotakRealni
FROM narudzba_proizvoda np
left JOIN popust po ON np.popust_id = po.popust_id
INNER JOIN proizvod pr ON pr.proizvod_id = np.proizvod_id
AND ((Nvl(po.postotak, 0)/100))*pr.cijena<200;

SELECT Sum(NARUDZBA_ID*3+cijena*3+postotak*3) FROM
(SELECT np.narudzba_id, pr.cijena Cijena, Nvl(po.postotak, 0) Postotak, Nvl(po.postotak, 0)/100 PostotakRealni
FROM narudzba_proizvoda np
left JOIN popust po ON np.popust_id = po.popust_id
INNER JOIN proizvod pr ON pr.proizvod_id = np.proizvod_id
AND ((Nvl(po.postotak, 0)/100))*pr.cijena<200);

SELECT Sum(NARUDZBA_ID*7+cijena*7+postotak*7) FROM
(SELECT np.narudzba_id, pr.cijena Cijena, Nvl(po.postotak, 0) Postotak, Nvl(po.postotak, 0)/100 PostotakRealni
FROM narudzba_proizvoda np
left JOIN popust po ON np.popust_id = po.popust_id
INNER JOIN proizvod pr ON pr.proizvod_id = np.proizvod_id
AND ((Nvl(po.postotak, 0)/100))*pr.cijena<200);


SELECT naziv Kategorija,          /*ne kontammm ovaj nikako*/
Decode (nadkategorija_id, NULL, 'Nema kategorije', 1, 'Komp oprema', nadkategorija_id) Nadkategorija
FROM kategorija;

SELECT Sum(Length("Kategorija")*3+Length("Nadkategorija")*3) FROM
(SELECT naziv "Kategorija",
Decode (nadkategorija_id, NULL, 'Nema kategorije', 1, 'Komp oprema', nadkategorija_id) "Nadkategorija"
FROM kategorija);

SELECT Sum(Length("Kategorija")*7+Length("Nadkategorija")*7) FROM
(SELECT naziv "Kategorija",
Decode (nadkategorija_id, NULL, 'Nema kategorije', 1, 'Komp oprema', nadkategorija_id) "Nadkategorija"
FROM kategorija);


SELECT Nvl(datum_raskidanja, Add_Months(datum_potpisivanja, 24)) Raskid
FROM ugovor_za_pravno_lice
WHERE SubStr(ugovor_id, 1, 2)<=50;

SELECT Sum(To_Number(To_Char(RASKID,'YYYY'))) FROM
(SELECT Nvl(datum_raskidanja, Add_Months(datum_potpisivanja, 24)) Raskid
FROM ugovor_za_pravno_lice
WHERE SubStr(ugovor_id, 1, 2)<=50);

SELECT Sum(To_Number(To_Char(RASKID,'YYYY'))) * 2 FROM
(SELECT Nvl(datum_raskidanja, Add_Months(datum_potpisivanja, 24)) Raskid
FROM ugovor_za_pravno_lice
WHERE SubStr(ugovor_id, 1, 2)<=50);


SELECT f.ime, f.prezime,
Decode(o.naziv, 'Management', 'MANAGER', 'Human Resources', 'HUMAN', 'OTHER') odjel, o.odjel_id
FROM uposlenik u, odjel o, fizicko_lice f
WHERE u.odjel_id = o.odjel_id
AND u.uposlenik_id = f.fizicko_lice_id
ORDER BY 1 ASC, 2 DESC;

SELECT SUM(Length(ime)*3+Length(prezime)*3+Length(Odjel)*3) FROM
(SELECT f.ime, f.prezime,
Decode(o.naziv, 'Management', 'MANAGER', 'Human Resources', 'HUMAN', 'OTHER') odjel, o.odjel_id
FROM uposlenik u, odjel o, fizicko_lice f
WHERE u.odjel_id = o.odjel_id
AND u.uposlenik_id = f.fizicko_lice_id
ORDER BY 1 ASC, 2 DESC)
WHERE ROWNUM<2;

SELECT SUM(Length(ime)*7+Length(prezime)*7+Length(Odjel)*7) FROM
(SELECT f.ime, f.prezime,
Decode(o.naziv, 'Management', 'MANAGER', 'Human Resources', 'HUMAN', 'OTHER') odjel, o.odjel_id
FROM uposlenik u, odjel o, fizicko_lice f
WHERE u.odjel_id = o.odjel_id
AND u.uposlenik_id = f.fizicko_lice_id
ORDER BY 1 ASC, 2 DESC)
WHERE ROWNUM<2;


SELECT p.kategorija_id,
       (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT min(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najjeftiniji,
      (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT max(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najskuplji,
       MIN(p.cijena) + MAX(p.cijena) ZCijena
FROM proizvod p
GROUP BY p.kategorija_id
ORDER BY ZCijena;

SELECT SUM(Length(Najjeftiniji)*3+ZCijena*3) FROM
(SELECT p.kategorija_id,
       (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT min(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najjeftiniji,
      (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT max(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najskuplji,
       MIN(p.cijena) + MAX(p.cijena) ZCijena
FROM proizvod p
GROUP BY p.kategorija_id
ORDER BY ZCijena)
WHERE ROWNUM<4;

SELECT SUM(Length(Najjeftiniji)*7+ZCijena*7) FROM
(SELECT p.kategorija_id,
       (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT min(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najjeftiniji,
      (SELECT p2.naziv
       FROM proizvod p2
       WHERE p2.kategorija_id = p.kategorija_id
       AND p2.cijena = (SELECT max(p3.cijena)
					              FROM proizvod p3 WHERE p3.kategorija_id = p.kategorija_id)) Najskuplji,
       MIN(p.cijena) + MAX(p.cijena) ZCijena
FROM proizvod p
GROUP BY p.kategorija_id
ORDER BY ZCijena)
WHERE ROWNUM<4;