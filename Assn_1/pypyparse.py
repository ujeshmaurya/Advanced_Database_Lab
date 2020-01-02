from lark import Lark
from lark.tree import pydot__tree_to_png as printTree
#import grammar from the grammar File #seriously
from grammarFile import grammar

#Create a LALR Parser
LALRparser = Lark(grammar, start = 'query', ambiguity = 'explicit')

f = open("testcases.txt", "r")
querynum = 1
for x in f:
	#Create a directed Graph (Abstract Syntax Tree) of the 'x'th query given in testCase File
	diGraphOfQuery = LALRparser.parse(x)
	#Print the Di-graph (Abstract Syntax Tree) of the 'x'th query given in testCase File on a image file using PyDot Library
	printTree(diGraphOfQuery, "ParseTreeQuery" + str(querynum) + ".png")
	querynum += 1



