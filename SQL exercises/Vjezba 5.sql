SELECT SYSDATE FROM dual;

SELECT first_name, Months_Between(hire_date, SYSDATE)
FROM employees;

SELECT hire_date, Add_Months(hire_date, 6)
FROM employees;

SELECT Mod(6, 4) FROM dual;

SELECT to_char(-0.12789), to_char(sysdate, 'dd.mm.yyyy, hh24:')
FROM dual;

SELECT to_char(sysdate, 'day - month syyyy, hh:mi:ss')
FROM dual;

SELECT lpad(first_name || ' ' || last_name, 20) "LPAD funkcija",
       rpad(first_name || ' ' || last_name, 20) "RPAD funkcija"
FROM employees;

SELECT ltrim('     O B P     ') "LTRIM funkcija",
 rtrim('***** O B P *****', '*') "RTRIM funkcija"
FROM dual;

SELECT Last_day(SYSDATE) FROM dual;


SELECT round(sysdate) "ROUND bez zadatog formata",
 trunc(sysdate) "TRUNC bez zadatog formata",
 round(sysdate, 'mm') "ROUND sa zadatim formatom",
 trunc(sysdate, 'yyyy') "TRUNC sa zadatim formatom"
FROM dual;

/*zadaci sa laba*/
SELECT USER "user", SYSDATE datum
FROM dual;

SELECT employee_id, first_name, last_name, salary, 1.25*salary "Plata uvecana za 25%", Mod(1.25*salary, 100) "ostatak plate"
FROM employees;

SELECT first_name, last_name, hire_date, To_Char(Next_Day(Add_Months(hire_date, 6), 'monday'), 'day - month, yyyy')  "ponedjeljak nakon 6 mjeseci"
FROM employees;

SELECT e.first_name, d.department_name, r.region_name, floor(Months_Between(SYSDATE, hire_date)) "Mjeseci od zaposlenja"
FROM employees e, departments d, locations l, countries c, regions r
WHERE e.department_id = d.department_id
AND d.location_id = l.location_id
AND l.country_id = c.country_id
AND c.region_id = r.region_id ;
                                                            /*ako je commisssion null, onda ga zamjeni sa 0, inace ostaje isto, izgleda da ako ima jedna nula onda je neki belaj*/
SELECT first_name, last_name, salary "Iznos plate", 4.5*salary*(1+Nvl(commission_pct, 0)) "Plata o kojoj mozes sanjati"
FROM employees
WHERE department_id IN (10,30,50);

SELECT LPad(first_name || ' ' || salary, 50, '$')
FROM employees;
                            /*obrnuto inticap prvo slovo je malo a ostala su velika, ako se izostavi 3. parametar u substr onda ide do kraja*/
SELECT Lower(SubStr(first_name, 1, 1)) || Upper(SubStr(first_name, 2)), Length(first_name), InitCap(first_name)
FROM employees
WHERE SubStr(first_name, 1, 1) IN ('A', 'J', 'M', 'S');

SELECT first_name, hire_date, To_Char(hire_date, 'Day'),
Decode (To_Char(hire_date, 'Day'),
        'Monday   ', 1,
        'Tuesday  ', 2,
        'Wednesday', 3,
        'Thursday ', 4,
        'Friday   ', 5,
        'Saturday ', 6,
        'Sunday   ', 7,
        0) poredak
FROM employees
ORDER BY poredak ASC;

      /*decode sluzi kao dodatni uslov za ispisivanje kolona, moze se nakon njega standardno navoditi kolone*/
SELECT first_name, city,
Decode (commission_pct,
        NULL, 'Zaposlenik ne prima dodatak na platu',
        To_Char(commission_pct) ) dodatak
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id
AND d.location_id = l.location_id;

                            /*nadodaj slijeva na blanko string broj zvjezdica koji odgovara broju hiljada*/
SELECT first_name, last_name, salary, LPad(' ', Round(salary/1000) +1, '*') hiljade
FROM employees;

