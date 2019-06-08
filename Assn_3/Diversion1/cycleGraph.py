import sys
from collections import defaultdict 
import pydotplus

# This class will help to create a graph that will be used to detect cycle and
# do topological sorting on acyclic graph
class Graph():
	# Default constructor used to instantiate the graph 
	def __init__(self,vertices): 
	    self.graph = defaultdict(list) 
	    self.V = vertices 

    # This function adds a directed edge between node 'u' and 'v'
    # I have added the edge between 'u - 1' and 'v - 1' because list follows 0-based indexing
	def addEdge(self,u,v): 
	    self.graph[u - 1].append(v - 1) 

	cycleAt = -1

    # This is a utility function for detecting cycle in the graph
	def isCyclicUtil(self, v, visited, recStack, graph): 
	    # Mark current node as visited and  
	    # adds to recursion stack 
	    visited[v] = True
	    recStack[v] = True
	    global cycleAt
	    # Recur for all neighbours 
	    # if any neighbour is visited and in  
	    # recStack then graph is cyclic 
	    for neighbour in self.graph[v]: 
	        if visited[neighbour] == False: 
	            if self.isCyclicUtil(neighbour, visited, recStack, graph) == True: 
	            	if cycleAt != -1:
		            	graph.del_edge(v + 1, neighbour + 1)
		            	graph.add_edge(pydotplus.Edge(v + 1
											, neighbour + 1
											, color = 'red'))
		            	graph.add_node(pydotplus.Node(neighbour + 1
										, label = "Transaction_" + str(neighbour + 1)
										, shape = 'ellipse'
										, color = 'black'
										, style = 'filled'
										, fillcolor = 'magenta'
										, fontname = 'Consolas'
										, fontcolor = 'black'))
	            	if cycleAt == v:
	            		cycleAt = -1
	            	return True
	        elif recStack[neighbour] == True:
	        	cycleAt = neighbour
	        	graph.del_edge(v + 1, neighbour + 1)
	        	graph.add_edge(pydotplus.Edge(v + 1
										, neighbour + 1
										, color = 'red'))
	        	graph.add_node(pydotplus.Node(neighbour + 1
										, label = "Transaction_" + str(neighbour + 1)
										, shape = 'ellipse'
										, color = 'black'
										, style = 'filled'
										, fillcolor = 'magenta'
										, fontname = 'Consolas'
										, fontcolor = 'black'))
	        	return True

	    # The node needs to be poped from  
	    # recursion stack before function ends 
	    recStack[v] = False
	    return False

	# This function returns true if graph is cyclic else false 
	def isCyclic(self, graph):
		global cycleAt
		visited = [False] * self.V 
		recStack = [False] * self.V 
		for node in range(self.V): 
		    if visited[node] == False:
		    	cycleAt = -1
		    	if self.isCyclicUtil(node, visited, recStack, graph) == True: 
		    		return True
		return False

    # This function is a utility function for doing a topological sort on the graph
	def topologicalSortUtil(self,v,visited,stack): 
	    # Mark the current node as visited. 
	    visited[v] = True

	    # Recur for all the vertices adjacent to this vertex 
	    for i in self.graph[v]: 
	        if visited[i] == False: 
	            self.topologicalSortUtil(i,visited,stack) 

	    # Push current vertex to stack which stores result, 0 is the index
	    stack.insert(0,v) 

	# This function do Topological Sort. It uses recursive topologicalSortUtil() 
	def topologicalSort(self, graph): 
	    # Mark all the vertices as not visited 
	    visited = [False]*self.V 
	    stack = [] 

	    # Call the recursive helper function to store Topological 
	    # Sort starting from all vertices one by one 
	    for i in range(self.V): 
	        if visited[i] == False: 
	            self.topologicalSortUtil(i,visited,stack) 

	    # Print contents of the stack 
	    print ("\t\t\tSerializability Order: ", end = " ")
	    counter = 1
	    for trans in stack:
	    	if trans != stack[-1]:
	    		print (str(trans + 1) + " ->", end = " ")
	    		graph.add_node(pydotplus.Node(trans + 1
										, label = "Tran-" + str(trans + 1) + " Step-" + str(counter)
										, shape = 'ellipse'
										, color = 'black'
										, style = 'filled'
										, fillcolor = 'yellow'
										, fontname = 'Consolas'
										, fontcolor = 'black'))
	    		counter += 1
    		else:
    			print (str(trans + 1))
    			graph.add_node(pydotplus.Node(trans + 1
										, label = "Tran-" + str(trans + 1) + " Step-" + str(counter)
										, shape = 'ellipse'
										, color = 'black'
										, style = 'filled'
										, fillcolor = 'yellow'
										, fontname = 'Consolas'
										, fontcolor = 'black'))
    			counter += 1

		# Flush stdout to print the above data quickly
	    sys.stdout.flush()