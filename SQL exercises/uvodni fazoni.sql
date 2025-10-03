SELECT location_id, country_id
FROM locations
WHERE country_id='CA';

SELECT department_id, location_id
FROM departments
WHERE location_id=1800
OR location_id=1900;

SELECT employee_id, first_name, last_name, department_id
FROM employees
WHERE department_id=20;

SELECT first_name || ' ' || last_name AS "ime i prezime", salary , hire_date
FROM employees
WHERE hire_date > To_Date('01/01/2005','dd/mm/yyyy');

SELECT *
FROM employees
order BY salary, last_name;


/*NVL(neka_kolona, vrijednost) je funkcija koja zamjenjuje null vrijednosti neke kolone sa nekom konkretnom vrijednoscu*/

SELECT *
FROM employees
WHERE SubStr(first_name, 1, 1) BETWEEN 'A' AND 'D';