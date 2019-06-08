import nltk
from nltk.tag import pos_tag
from nltk.tokenize import word_tokenize
from nltk.tree import *
from nltk.draw import tree
from nltk import Nonterminal, nonterminals, Production, CFG
from nltk.parse import RecursiveDescentParser
text = "what is your name?"
words = text.split()
sentence = pos_tag(words)

grammar1 = nltk.CFG.fromstring("""
    S -> NP VP
    S -> VP
    VP -> V NP | V NP PP
    NP ->  Det N | Det N PP
    PP -> P NP
    V -> "saw" | "ate" | "walked" | "book" | "prefer" | "sleeps"
    Det -> "a" | "an" | "the" | "my" | "that"
    N -> "man" | "dog" | "cat" | "telescope" | "park" | "flight" | "apple"
    P -> "in" | "on" | "by" | "with"
     """)

rd = nltk.RecursiveDescentParser(grammar1, "Input")
result = rd.parse(sentence)
result.draw()
