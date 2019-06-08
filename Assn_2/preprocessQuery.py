def modifyQuery(query):
	query = query.lower()
	query = query.replace("order by", "orderby")
	query = query.replace("group by", "groupby")
	query = query.replace("inner join", "join")
	query = query.replace("outer join", "join")
	query = query.replace("left join", "join")
	query = query.replace("right join", "join")
	query = query.replace("natural join", "join")
	return query