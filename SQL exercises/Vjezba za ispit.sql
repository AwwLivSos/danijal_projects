CREATE TABLE movies (id INTEGER PRIMARY KEY, title VARCHAR(30 ),c1 INTEGER, c2 INTEGER);

INSERT INTO movies(id,title,c1,c2)
VALUES(1, 'GHOST', NULL,2);

INSERT INTO movies(id,title,c1,c2)
VALUES(2, 'HOUSE', 1,4);

INSERT INTO movies(id,title,c1,c2)
VALUES(3, 'tick',2,3);

INSERT INTO movies(id,title,c1,c2)
VALUES(4, 'red', 3,3);

INSERT INTO movies(id,title,c1,c2)
VALUES(5, 'spider', 2,2);

SELECT * FROM movies;

SELECT Upper(SubStr(a.title,1,4)) kolA
FROM movies a, movies b, movies c
WHERE a.c1=b.id AND b.id+b.c1=c.c2
ORDER BY kolA;

SELECT * FROM employees e, departments d WHERE e.department_id = d.department_id;