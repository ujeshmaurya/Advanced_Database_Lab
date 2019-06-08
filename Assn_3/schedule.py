import pydotplus
import cv2


from collections import defaultdict 
  
class Graph(): 
    def __init__(self,vertices): 
        self.graph = defaultdict(list) 
        self.V = vertices 
  
    def addEdge(self,u,v): 
        self.graph[u - 1].append(v - 1) 
  
    def isCyclicUtil(self, v, visited, recStack): 
        visited[v] = True
        recStack[v] = True
        for neighbour in self.graph[v]: 
            if visited[neighbour] == False: 
                if self.isCyclicUtil(neighbour, visited, recStack) == True: 
                    return True
            elif recStack[neighbour] == True: 
                return True
        recStack[v] = False
        return False

    def isCyclic(self): 
        visited = [False] * self.V 
        recStack = [False] * self.V 
        for node in range(self.V): 
            if visited[node] == False: 
                if self.isCyclicUtil(node,visited,recStack) == True: 
                    return True
        return False

INPUT_PATH = "input.txt"
def get_transaction_id(token):
	ind = token.find('(')
	return int(token[2 : ind])

def get_operation_type(token):
	if token[0] == 'r' or token[0] == 'R':
		return "read"
	return "write"

def get_data_item(token):
	ind1 = token.find('(')
	ind2 = token.find(')')
	return token[ind1 + 1 : ind2]

def get_num_of_transactions(schedule):
	num = 0
	for ch in schedule:
		if ch.isdigit():
			num = max(num, int(ch))
	return num


with open(INPUT_PATH, "r") as infile:
	for schedule in infile.readlines():
		num_of_transactions = get_num_of_transactions(schedule)
		cgraph = Graph(num_of_transactions)
		node_cnt, edge_cnt = 0, 0
		node_list = []
		graph = pydotplus.Dot(graph_type='digraph', ordering='out')
		schedule_list = schedule.split()
		for token in schedule_list:
			transaction_id = get_transaction_id(token)
			operation_type = get_operation_type(token)
			data_item = get_data_item(token)
			if transaction_id not in node_list:
				node_list.append(transaction_id)
				graph.add_node(pydotplus.Node(transaction_id
											, label = "Transaction_" + str(transaction_id)
											, shape = 'box'
											, color = 'brown'
											, style = 'filled'
											, fillcolor = 'grey'
											, fontcolor = 'brown'))
			for i in range(schedule_list.index(token) + 1, len(schedule_list)):
				iterator = schedule_list[i]
				it_transaction_id = get_transaction_id(iterator)
				if it_transaction_id == transaction_id:
					continue
				it_data_item = get_data_item(iterator)
				if it_data_item != data_item:
					continue
				it_operation_type = get_operation_type(iterator)
				if it_operation_type == "read" and operation_type == "read":
					continue
				if len(graph.get_edge(transaction_id, it_transaction_id)) == 0:
					graph.add_edge(pydotplus.Edge(transaction_id
											, it_transaction_id
											, label = str(edge_cnt)
											, color='purple'
											, fontcolor='red'))
					cgraph.addEdge(transaction_id, it_transaction_id)
					edge_cnt += 1
		if cgraph.isCyclic() == 1: 
		    print ("Graph has a cycle")
		else: 
		    print ("Graph has no cycle")
		graph.write_png('precedence.png')
		img = cv2.imread('precedence.png')
		cv2.imshow("Precedence Graph", img)
		cv2.waitKey(0)

				


