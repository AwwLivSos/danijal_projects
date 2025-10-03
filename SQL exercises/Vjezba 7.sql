SELECT e1.first_name, d1.department_id, j1.job_title
FROM employees e1, departments d1, jobs j1
WHERE e1.department_id = d1.department_id
AND e1.job_id = j1.job_id
AND e1.department_id = (SELECT e2.department_id
                        FROM employees e2
                        WHERE e2.first_name LIKE 'Susan')
AND e1.first_name != 'Susan';


SELECT e1.first_name, e1.last_name, e1.salary
FROM employees e1
WHERE e1.salary > ALL (SELECT Avg(e2.salary)
                   FROM employees e2
                   WHERE e2.department_id IN (30,90));


SELECT e1.*
FROM employees e1
WHERE e1.department_id in (SELECT e2.department_id
                          FROM employees e2
                          WHERE e2.first_name LIKE '%C%');


/*cetvrti ne treba podupit*/

SELECT *
FROM employees e1
WHERE e1.manager_id in (SELECT e2.employee_id
                       FROM employees e2
                       WHERE e2.last_name = 'King');

SELECT e1.*, (SELECT Round(Avg(e3.salary), 2)
             FROM employees e3
             WHERE e3.department_id=e1.department_id) "Prosjecna plata odjela"
FROM employees e1
WHERE e1.salary > (SELECT Avg(e2.salary)
                          FROM employees e2
                          WHERE e2.department_id=e1.department_id);

