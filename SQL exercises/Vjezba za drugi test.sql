SELECT employee_id, manager_id
FROM employees
START WITH employee_id=105;
CONNECT BY PRIOR manager_id=employee_id;

SELECT * FROM user_constraints;

SELECT department_id, Grouping(department_id) grupirani_odjel, job_id,  Grouping(job_id) grupirani_posao,  Round(Avg(salary), 5) plata
FROM employees
GROUP BY rollup(department_id, job_id);       --previse komplikovano za objasnit u komentarima, ali grouping samo vraca 1 ili 0, u odnosu da li je red bio grupiran od strane rollupa ili cubea

SELECT department_id, job_id,  Grouping_id(job_id, department_id) grupirano,  Round(Avg(salary), 5) plata
FROM employees
GROUP BY rollup(job_id,department_id);      --s druge strane, ovaj grouping_id vraca binarni broj u zavisnosti sta je grupirano. recimo, ako je prva kolona grupirana a druga nije, onda je to broj 10, ako su obje to je 11 (tj decimalni broj 3) itd

SELECT CASE
WHEN Grouping(department_id) = 1 AND Grouping(job_id) = 1 THEN 'Konacna suma'
WHEN Grouping(job_id) = 1 THEN  department_id || ' - podsuma'
ELSE department_id || ' - ' || job_id
END AS Tabela_sumiranja, Sum(salary) AS plata
FROM employees                                                     --ima logike ovo
GROUP BY rollup(department_id, job_id);

SELECT ROWNUM, salary, last_name
FROM (SELECT *
      FROM employees
      ORDER BY salary DESC)           --prvo napises podupit koji poreda sve zaposelne po plati, onda odaberes ROWNUM koji bukvalno vraca broj reda, i odaberes da je on manji od 3 da bi dobio top 3 najplacenija
WHERE ROWNUM <=3;

SELECT (SELECT Min(e1.first_name)
        FROM employees e1
        WHERE e1.first_name>'b')
FROM employees e2;

SELECT Avg(e1.employee_id)
FROM employees e1, employees e2, employees e3
WHERE e1.department_id<>e2.department_id AND e2.department_id<>e3.department_id;

SELECT TO_CHAR(1234, '9,9,99') FROM dual;
SELECT to_number('1234', '9999') FROM dual;

SELECT SYSDATE AS original_date,
       TRUNC(SYSDATE, 'MONTH') AS truncated_date  --trunc i round zavise od parametara, ali defaultno rade na dan
FROM DUAL;

SELECT TO_DATE('50', 'yy') AS year_50,
       TO_DATE('49', 'yy') AS year_49
FROM DUAL;