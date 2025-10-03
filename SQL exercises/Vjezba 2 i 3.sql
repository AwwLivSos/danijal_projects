SELECT first_name, salary
FROM employees
WHERE salary>2456;

SELECT * FROM employees;

SELECT  first_name, department_id
FROM employees
WHERE employee_id=102;

SELECT *
FROM employees
WHERE salary>2345
OR salary<1000;

SELECT first_name AS Zaposleni, job_id, hire_date
FROM employees
WHERE hire_date BETWEEN To_Date('11.01.2006','dd.mm.yyyy') AND To_Date('22.02.2007','dd.mm.yyyy');

SELECT first_name, department_id
FROM employees
WHERE department_id =10 OR department_id=30
ORDER BY last_name;

SELECT salary AS "Mjesecna plata", first_name, last_name, salary+2000
FROM employees
WHERE salary>1500 AND (department_id=10 OR department_id=30);

SELECT *
FROM employees
WHERE hire_date < To_Date('01.01.2006','dd.mm.yyyy');

SELECT *
FROM employees
WHERE manager_id IS NULL;

SELECT first_name, last_name, salary, commission_pct
FROM employees
WHERE commission_pct IS NOT NULL
ORDER BY salary DESC, commission_pct DESC;

SELECT first_name||last_name AS Zaposleni
FROM employees
WHERE (first_name||last_name) LIKE '%i%i%';

SELECT first_name, last_name, salary, commission_pct
FROM employees
WHERE commission_pct > (salary*0.8);