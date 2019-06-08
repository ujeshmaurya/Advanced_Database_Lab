from inputUtility import get_transaction_id, get_operation_type, get_data_item
import pydotplus

# This function will make 2 graphs:
# 1st graph will be for pydotplus (to generate the visual image of precedence graph)
# 2nd graph will be for finding cycle and doing topological sorting
def make_graph(graph, cgraph, schedule):
	node_cnt = 0
	node_list = []
	schedule_list = schedule.split()
	for token in schedule_list:
		# Get the various parameters of this token using inputUtility functions
		transaction_id = get_transaction_id(token)
		operation_type = get_operation_type(token)
		data_item = get_data_item(token)

		# If the transaction is not added in graph, then add it
		if transaction_id not in node_list:
			node_list.append(transaction_id)
			graph.add_node(pydotplus.Node(transaction_id
										, label = "Transaction_" + str(transaction_id)
										, shape = 'ellipse'
										, color = 'blue'
										, style = 'filled'
										, fillcolor = 'cyan'
										, fontname = 'Consolas'
										, fontcolor = 'black'))

		# Add edges from this transaction to others if there is a conflicted operation
		for i in range(schedule_list.index(token) + 1, len(schedule_list)):
			iterator = schedule_list[i]

			# If both operations are in same transaction, then NO conflict
			it_transaction_id = get_transaction_id(iterator)
			if it_transaction_id == transaction_id:
				continue

			# If both operations on different data item, then also NO conflict
			it_data_item = get_data_item(iterator)
			if it_data_item != data_item:
				continue

			# If both operations are READ operation, then also NO conflict
			it_operation_type = get_operation_type(iterator)
			if it_operation_type == "read" and operation_type == "read":
				continue

			# Other than above 3 cases, there will be a conflict, hence add an edge
			# However, first check if there is an edge already present or not
			if len(graph.get_edge(transaction_id, it_transaction_id)) == 0:
				# Edge is not present, hence add edge between the two transactions
				graph.add_edge(pydotplus.Edge(transaction_id
										, it_transaction_id
										, color = 'green'))
				cgraph.addEdge(transaction_id, it_transaction_id)