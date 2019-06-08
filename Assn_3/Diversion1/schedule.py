# Import libraries for using pydotplus and handling png images
import pydotplus
import cv2
from cycleGraph import Graph
from inputUtility import get_num_of_transactions
from makeGraph import make_graph
# Set path for files used
INPUT_PATH = "input.txt"
OUTPUT_PATH = "output.txt"
#________________________________________________________________________________________#


# Parsing the input into a directed graph and finding cycle to check conflict serializability
#outfile = open(OUTPUT_PATH, "w")
with open(INPUT_PATH, "r") as infile:
	file_counter = 1
	for schedule in infile.readlines():
		# Print the id of schedule
		print ("Schedule " + str(file_counter) + ":", end = " ")

		# Find the number of transaction in the schedule
		num_of_transactions = get_num_of_transactions(schedule)

		# Make a graph to detect cycle and topological sorting
		cgraph = Graph(num_of_transactions)

		# Create a pydotplus graph
		graph = pydotplus.Dot(graph_type='digraph', ordering='out')

		#outfile.write(schedule)
		make_graph(graph, cgraph, schedule)

		# Check for cycle in the graph and find serializability order using topological sorting
		if cgraph.isCyclic(graph) == 1: 
		    print ("Precedence Graph is cyclic. Hence it is not conflict serializable.")
		else: 
		    print ("Precedence Graph is not cyclic. Hence it is conflict serializable.")
		    cgraph.topologicalSort(graph)

	    # Print the graph on a png image using pydotplus graph
		graph.write_png('precedence' + str(file_counter) + '.png')

		# Show that image on the screen
		img = cv2.imread('precedence' + str(file_counter) + '.png')
		cv2.imshow("Precedence Graph " + str(file_counter), img)
		cv2.waitKey(0)

		# Increment the file counter for next handling schedule
		file_counter += 1