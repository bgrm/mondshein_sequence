It is an implementation of an algorithm computing the Mondshein sequence for a given 3-connected graph by Jens Schmidt:
https://arxiv.org/pdf/1311.0750.pdf

The algorithm itself works in the complexity of O(m), but one of the helper-structures (the one in 'paths') works in O(nlog(n)).
The main programm takes the input graph from the standard input and computes its mondshein sequence.
'Test n' script runs main on many random tests.
