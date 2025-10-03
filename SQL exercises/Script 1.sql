SELECT location_id, country_id
FROM locations
WHERE country_id='CA';

SELECT department_id, location_id
FROM departments
WHERE location_id=1800 OR location_id=1900;

SELECT employee_id, first_name, last_name
FROM employees
WHERE department_id=20;