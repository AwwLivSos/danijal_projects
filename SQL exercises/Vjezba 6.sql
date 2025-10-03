SELECT Count(*)
FROM employees                  /*count inace vraca broj redova, ali pomocu group by mozemo naci broj redova za neku specificnu kolonu, pri cemu je mozemo dodati onda u select*/
GROUP BY department_id;

SELECT Count(manager_id)    /*razlika izmeðu count * i count po koloni je sto * broji i null*/
FROM employees;

SELECT Count(*), Sum(salary), Min(salary), Max(salary), Avg(salary) prosjek    /*neke osnovne agregatne funkcije, mogu se zatim gledati po nekoj koloni, npr dept_id*/
FROM employees
GROUP BY department_id;

SELECT Count(*), Sum(salary), Min(salary), Max(salary), Avg(salary) prosjek
FROM employees
GROUP BY department_id
HAVING Avg(salary)<5000;                  /*having se koristi umjesto where u agregaciji*/

/*Labaratorijska vjezba 6*/

SELECT Sum(commission_pct), Count(commission_pct), Count(*)
FROM employees;

SELECT Count(*) "Broj uposlanih", job_id "Naziv posla", department_id "Naziv odjela"
FROM employees
GROUP BY job_id, department_id;

SELECT Max(salary), Min(salary), Sum(salary), Round(Avg(salary), 6) Prosjecna
FROM employees;

SELECT Max(salary), Min(salary), Sum(salary), Round(Avg(salary), 6) Prosjecna
FROM employees
GROUP BY job_id;

SELECT Count(*), job_id
FROM employees
GROUP BY job_id;

SELECT Count(manager_id)
FROM employees;

/*deveti zadatak*/
SELECT  distinct  (   SELECT Count(*)
                  FROM employees
                  WHERE To_Char(hire_date, 'yyyy') = '2005'
                  ) ,

                  (   SELECT Count(*)
                  FROM employees
                  WHERE To_Char(hire_date, 'yyyy') = '2006'
                  ) ,

                  (   SELECT Count(*)
                  FROM employees
                  WHERE To_Char(hire_date, 'yyyy') = '2007'
                  )
                   ,
                  (   SELECT Count(*)
                  FROM employees
                  WHERE To_Char(hire_date, 'yyyy') = '2008'
                  ) ,

                  (   SELECT Count(*)
                  FROM employees
                  WHERE To_number(To_Char(hire_date, 'yyyy')) BETWEEN 2005 AND 2008
                  )
FROM employees ;