# Import necessary modules from other files
from preprocessQuery import modifyQuery

# Read the input file for reading queries one by one
f = open("input.txt", "r")

# Iterate through each input query and convert it to Relational Algebra
for xquery in f:
	# Modify the query for convenient and smooth processing
	query = modifyQuery(xquery)
	# Split the query into a list for further operations
	query_list = query.split(" ")
	print(query_list)
	# Make empty list for various operations
	proj_list, cond_list, order_list, groupcond_list, groupproj_list = "", "", "", "", ""
	tablename_list = []
	thetajoin_list = []
	join_flag = False
	thetajoin_flag = False
	fromInd, whereInd, joinInd, orderby, having, groupby, onInd = 1000, 1000, 1000, 1000, 1000, 1000, 1000
	N = len(query_list)
	fromInd = query_list.index('from')
	if ('where' in query_list):
		whereInd = query_list.index('where')
	if ('join' in query_list):
		joinInd = query_list.index('join')
	if ('on' in query_list):
		onInd = query_list.index('on')
	if ('orderby' in query_list):
		orderInd = query_list.index('orderby')
	if ('having' in query_list):
		havingInd = query_list.index('having')
	if ('groupby' in query_list):
		groupInd = query_list.index('groupby')

	for i in range (fromInd + 1, N):
		if (query_list[i] in ['join', 'on', 'where', 'orderby', 'groupby', 'having']):
			break
		tablename_list.append(query_list[i])
	for i in range(0, len(tablename_list) - 1):
		thetajoin_list.append("")

	if ('*' not in query_list):
		for i in range (1, fromInd):
			proj_list = proj_list + query_list[i]

	if ( (len(tablename_list) > 1) or ('join' in query_list) ):
		join_flag = True
		if ('join' in query_list):
			joinSubquery = []
			j = joinInd + 1
			while (j < N):
				if (query_list[j] in ["where", "orderby", "groupby", "having"]):
					break
				j = j + 1
			joinSubquery = query_list[joinInd + 1: j]
			thetacond = ""
			condFlag = False
			for i in range(0, len(joinSubquery)):
				if (joinSubquery[i] == 'join'):
					thetajoin_list.append(thetacond)
					thetacond = ""
					condFlag = False
				elif (joinSubquery[i] == 'on'):
					condFlag = True
				else:
					if condFlag:
						if (joinSubquery[i] == "and"):
							thetacond = thetacond + "/\\"
						elif (joinSubquery[i] == "or"):
							thetacond = thetacond + "V"
						else:
							thetacond = thetacond + joinSubquery[i]
					else:
						tablename_list.append(joinSubquery[i])
			if condFlag:
				thetajoin_list.append(thetacond)
			
	if ('where' in query_list):
		for i in range (whereInd + 1, N):
			if (query_list[i] in ["orderby", "groupby", "having"]):
				break
			if (query_list[i] == "and"):
				cond_list = cond_list + "/\\"
			elif (query_list[i] == "or"):
				cond_list = cond_list + "V"
			else:
				cond_list = cond_list + query_list[i]

	if ('orderby' in query_list):
		for i in range (orderInd + 1, N):
			if (query_list[i] in ["groupby", "having"]):
				break
			order_list = order_list + query_list[i] + " "
			
	if ('groupby' in query_list):
		for i in range (groupInd + 1, N):
			if (query_list[i] in ["orderby", "having"]):
				break
			groupcond_list = groupcond_list + query_list[i]
		for i in range (1, fromInd):
			if (query_list[i].startswith("max(") or query_list[i].startswith("min(") or query_list[i].startswith("sum(") or query_list[i].startswith("avg(") or query_list[i].startswith("count(")):
				groupproj_list = groupproj_list + query_list[i]
		groupproj_list = groupproj_list.rstrip(',')

	if (proj_list != ""):
		print("⺇ {" + proj_list + "}", end = " ")
		print("(", end = " ")
		
	if (order_list != ""):
		print ("γ {" + order_list + "}", end = " ")
		print("(", end = " ")
		
	if (groupcond_list != ""):
		print("{" + groupcond_list + "}", end = " ")
		print("ℑ {" + groupproj_list + "}", end = " ")
		print("(", end = " ")
		
	if (cond_list != ""):
		print("σ {" + cond_list + "}", end = " ")
		print("(", end = " ")
		
	if (join_flag):
		for i in range(0, len(tablename_list)):
			print(tablename_list[i].rstrip(','), end = " ")
			if (i != len(tablename_list) - 1):
				print("⋈", end = " ")
				if (thetajoin_list[i] != ""):
					print("{" + thetajoin_list[i] + "}", end = " ")
	else:
		print(tablename_list[0], end = " ")

	if (cond_list != ""):
		print(")", end = " ")
	if (groupcond_list != ""):
		print(")", end = " ")
	if (order_list != ""):
		print(")", end = " ")
	if (proj_list != ""):
		print(")", end = " ")
