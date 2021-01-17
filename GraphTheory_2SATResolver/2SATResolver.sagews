## Exercice 3

def check_formula(F, init):
	### Add Falsy values in the init states dict
	fill = {}
	for key, value in init.items():
		fill[key] = value
		fill['!' + key] = not value
    
	### Check if a false clause is found, if not return true, else false
	for C in F:
		if (not fill[C[0]] and not fill[C[1]]):
			return False
	return True

F1 = [['x1', 'x2'], ['!x1', 'x2'], ['!x1', '!x2']]
i1 = {'x1': False, 'x2': True}
i2 = {'x1': False, 'x2': False}
i3 = {'x1': True, 'x2': False}
i4 = {'x1': True, 'x2': True}

print("Pour init={x2}, la formule donnée est : ", check_formula(F1, i1))
print("Pour init={}, la formule donnée est : ", check_formula(F1, i2))
print("Pour init={x1}, la formule donnée est : ", check_formula(F1, i3))
print("Pour init={x1, x2}, la formule donnée est : ", check_formula(F1, i4))

## Exercice 4

### Get the opposite variable
def get_opposite(x):
	if (x[0] == "!"):
		return (x[1:])
	return ("¬" + x)

### Used to replace "!" by the non logical operator character for lisibility purposes
def get_node(x):
	if (x[0] == "!"):
		return ("¬" + x[1:])
	return (x)

def greek_god_graph(F):
	g = DiGraph()
	for C in F:
		### Add the necessary nodes for each clause
		g.add_vertex(get_node(C[0]))
		g.add_vertex(get_opposite(C[0]))
		g.add_vertex(get_node(C[1]))
		g.add_vertex(get_opposite(C[1]))
		### Add mutual implications to build the implication graph
		g.add_edge(get_opposite(C[0]), get_node(C[1]))
		g.add_edge(get_opposite(C[1]), get_node(C[0]))
	return g

### Formula found on wikipedia : https://fr.wikipedia.org/wiki/Probl%C3%A8me_2-SAT
F2 = [['x0', 'x2'], ['x0', '!x3'], ['x1', '!x3'], ['x1', '!x4'], ['x2', '!x4'], ['x0', '!x5'], ['x1', '!x5'], ['x2', '!x5'], ['x3', 'x6'], ['x4', 'x6'], ['x5', 'x6']]

## Exercice 5

## Non-satisfiable formula from the Exercice 2
F3 = [['x1', 'x2'], ['!x1', 'x2'], ['!x1', '!x2'], ['x1', '!x2']]

### Tarjan algorithm python implementation (http://www.logarithmic.net/pfh/blog/01208083168)
### Based on wikipedia pseudo code (https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm)
### Linear complexity (In O(n) time)
def tarjan(graph):
	index_counter = [0]
	stack = []
	lowlinks = {}
	index = {}
	result = []
	def strongconnect(node):
		index[node] = index_counter[0]
		lowlinks[node] = index_counter[0]
		index_counter[0] += 1
		stack.append(node)
		try:
			successors = graph[node]
		except:
			successors = []
		for successor in successors:
			if successor not in lowlinks:
				strongconnect(successor)
				lowlinks[node] = min(lowlinks[node],lowlinks[successor])
			elif successor in stack:
				lowlinks[node] = min(lowlinks[node],index[successor])
		if lowlinks[node] == index[node]:
			connected_component = []
			while True:
				successor = stack.pop()
				connected_component.append(successor)
				if successor == node: break
			result.append(connected_component)
	for node in graph:
		if node not in lowlinks:
			strongconnect(node)
	return result

### Remove non-values to process the output
def pop_opposites(solutions):
	output = {}
	for key, value in solutions.items():
		if (not (key[0] == "¬")):
			output[key] = value
	return (output)
			

def is_satisfiable(g):
	### Converting the graph to adjacency list by myself because sagemath doesn't do it
	list = {}
	for v in g.vertices():
		list[v] = g.neighbors_out(v)
	### Get the strongly connected component using tarjan algorithm
	ssc = tarjan(list)
	### Check if xi and !xi in the same component
	for component in ssc:
		for v in component:
			if get_opposite(v) in component:
				return False
	### Get valid solution if satisfiable, Tarjan algorithm gives a topologic order, and each strongly connected component also has his opposite in the graph
	### So, as long as we still have undefined values, we set the ones from the beginning as true, and from the ending as false 
	solution = {}
	ssc_flat = [item for sublist in ssc for item in sublist]
	for x in range(len(ssc_flat)):
		if (not (ssc_flat[x] in solution)):
			solution[ssc_flat[x]] = True
		if (not (ssc_flat[len(ssc_flat) - (x + 1)] in solution)):
			solution[ssc_flat[len(ssc_flat) - (x + 1)]] = False
	return pop_opposites(solution)
	

### Display the graph for a given formula and get a solution (if satisfiable) or false
g = greek_god_graph(F1)
g.graphplot(vertex_size=500).show()
sol = is_satisfiable(g)

### Verify the given answer using the Exercice 3 sanitizer
print("Pour la formule entrée, la solution proposée via le graphe est : ", check_formula(F1, sol))
