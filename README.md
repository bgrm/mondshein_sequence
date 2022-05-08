It is an implementation of the algorithm computing the Mondshein sequence for a given 3-connected graph by Jens Schmidt:
https://arxiv.org/pdf/1311.0750.pdf

The algorithm itself works in the complexity of O(m), but one of the helper-structures (the one in 'paths') works in O(nlog(n)).
The main program takes the input graph from the standard input and computes its Mondshein sequence.
'Test n' script runs main on many random tests.
