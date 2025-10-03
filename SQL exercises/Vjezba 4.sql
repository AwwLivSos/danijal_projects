SELECT e.*, d.department_name               /*alias se moze i ranije vidjeti ovdje, prikazujemo svce iz emps i samo dept_name iz depts*/
FROM employees e, departments d             /*ovdje smo koristili kao alias za depts i emps*/
WHERE e.department_id = d.department_id;    /*gdje se podudaraju dept_id od emps i depts, da ne dobijemo kartezijanski proizvod*/
/*inace ako biramo n tabela, uslovi prikazivanja trebaju biti n-1, da ne bi dobili visestruki kartezijan*/

SELECT e.first_name, e.employee_id, d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id;

SELECT DISTINCT j.job_title
FROM employees e, jobs j, departments d
WHERE j.job_id = e.job_id
AND e.department_id = d.department_id
AND d.department_id = 30;

SELECT e.first_name, d.department_name, l.city
FROM employees e, departments d, locations l
WHERE e.department_id = d.department_id
AND l.location_id = d.location_id
AND e.commission_pct IS NULL;

SELECT e.first_name, d.department_name
FROM employees e, departments d
WHERE e.department_id = d.department_id
AND Lower(e.first_name) LIKE '%a%';

SELECT e.first_name, j.job_title, d.department_id, d.department_name
FROM employees e, departments d, jobs j, locations l
WHERE j.job_id = e.job_id
AND e.department_id = d.department_id
AND d.location_id = l.location_id
AND l.city = 'Roma';

SELECT DISTINCT city
FROM locations;

SELECT e.first_name AS "Naziv zaposlenog", e.last_name AS "Prezime", sef.first_name AS "Naziv sefa", l.city AS "Grad sefa"
FROM employees e, employees sef, departments d, locations l
WHERE e.manager_id = sef.employee_id
AND sef.department_id = d.department_id
AND d.location_id = l.location_id;

SELECT e.first_name AS "Naziv zaposlenog", e.last_name AS "Prezime", sef.first_name AS "Naziv sefa", l.city AS "Grad sefa"
FROM employees e, employees sef, departments d, locations l
WHERE e.manager_id = sef.employee_id(+)
AND sef.department_id = d.department_id(+)
AND d.location_id = l.location_id(+);

SELECT *
FROM employees;

SELECT e.first_name AS "Uzeti zaposlenik", e.department_id, b.first_name AS "Saradnici"
FROM employees e right OUTER join employees b
on e.department_id = b.department_id;

SELECT e.first_name AS "Uzeti zaposlenik", e.department_id, b.first_name AS "Saradnici"
FROM employees e, employees b
where e.department_id = b.department_id;

SELECT e.first_name, j.job_title, d.department_name, e.salary, e.commission_pct
FROM employees e, departments d, jobs j
WHERE j.job_id = e.job_id
AND e.department_id = d.department_id
AND e.salary * (1+e.commission_pct) > j.max_salary;

SELECT e.first_name, e.hire_date
FROM employees e, employees b
WHERE b.first_name = 'Stephen'
and e.hire_date > b.hire_date
ORDER BY e.hire_date;

