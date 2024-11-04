SELECT DISTINCT(name) FROM people, ratings, directors, movies
WHERE people.id = directors.person_id
AND directors.movie_id = movies.id
AND movies.id = ratings.movie_id
AND rating >= 9.0;