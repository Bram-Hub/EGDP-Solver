# EGDP Solver
## Authors
2014:
Nathan Berkley

## About
Usage:

  ./solver

Expects a graph on standard input, so you can always redirect from a file like 

  ./solver < inputfile.txt

if you don't want to type one out.

The gramar for a graph is 

  G -> (G) G | L G | empty
  L -> [a-zA-Z]

Where parenthesis denote cuts. So (A) would be "not A", A(A(B)) would be
"A and A implies B", etc.


Internal Representation:

Graphs are defined in eg.h and are represented recursively as a 
tree of graphs each of which contains one list of characters 
representing un-negated symbols, and a list of graphs each of 
which represents a cut. So the graph

  AB(A(C))(CB(D))

would be stored as

  G1
  symbols: A,B
  cuts: G2,G3

  G2
  symbols: A
  cuts: G4

  G3
  symbols: C,B
  cuts: G5

  G4
  symbols: C
  cuts: empty

  G5
  symbols: D
  cuts: empty

where G1 is the root of the tree and represents the full graph.

graphs support a number of operations for conveniet manipulation:

  void read(std::string g, int i): reads a graph from g starting at position
    i in the format described above.

  std::string toString(): returns a string representation of the graph in the 
    format described above.

  void print(int depth): prints the graph to standard output in a more human readable
    format, putting each cut and symbol on its own line at indent depth.

  bool open(): returns true iff the graph does not contain the empty cut
    at the top level.

  bool finished(): returns true iff the graph is the empty graph.

  char find_symbol(): Locate a symbol that exists in the graph,
    used to pick the next symbol to call reduce on. Looks for
    symbols at the top level or cuts containing only a single
    symbol first.

  void simplify(): Used to simplify the graph. This helps to reduce
    the run time by dropping subgraphs where possible, possibly
    dropping literals that would otherwise need to be branched on
    and also allows for the open() and finished() calls to be faster
    if we can assume the graph to be simplified.

    simplify does two checks at each subgraph starting from the leaves
    of the tree and working back to the root in a post-order traversal
    of the structure:

      If any child of that graph contains only a single cut and no
      symbols then it is a double cut and we move all of it's symbols
      and cuts up to the current level and delete the child.

      If any child is not open (contains an empty cut) then we can delete
      that child as it is tautologous.

  void reduce(char c, bool t): reduce the graph with respect to symbol c.
    t determines if we are setting c to true or false in the graph.
    If t is true then we simply remove c everywhere it occurs.
    If t is false we remove c everywhere it occurs and add an empty
    cut to that subgraph.

  bool check_consistency(int d): Runs a consistency check based on
    Davis-Putnam printing its work to standard out  at indent d for
    the root. Returns true if the graph is satisfiable.
    At each step it will simplify the graph, check if it is finished
    and open or closed, and if not it will pick another symbol and recurse
    where that symbol is true and where that symbol is false.
