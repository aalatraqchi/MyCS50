SELECT name FROM people, stars, movies
WHERE people.id = stars.person_id
AND stars.movie_id = movies.id
AND title IN (SELECT title FROM movies, stars, people
WHERE movies.id = stars.movie_id
AND stars.person_id = people.id
AND name = "Kevin Bacon" AND birth = 1958)
AND name != "Kevin Bacon";