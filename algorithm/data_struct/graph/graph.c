/*
graph is an important data structure. 
We can judge the connection between 2 point; 
We can calculate minimum distance between 2 point;
We can find different route based on different requirement.
Sometimes We have to distract minimum tree from graph, then We don't need to judge if we meet loop node constantly.
*/

/*
1. graph based on matrix
matrix is the simplest express method.
If there are 5 point in a graph, we can build a 5x5 matrix.
If there is connecion between points, then fill with 1; If there is no connection, we can use 0.
Of course, the diagonal connecntion has no significant.
*/
static int graph[5][5] = 
{
	{0, 1, 0, 1, 1},
	{1, 0, 1, 0, 1},
	{0, 1, 0, 1, 0},
	{1, 0, 1, 0, 1},
	{1, 1, 0, 1, 0}
};

/*
If the connections between points have direction, then they are not symmetrical.
*/

static int graph[5][5] = 
{
	{0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0},
	{0, 1, 0, 0, 0},
	{1, 0, 1, 0, 0},
	{1, 1, 0, 1, 0}
};

/*
If the connection between points has some kinds of relationship, such as distance, we can use it to replace 1.
*/
static int graph[5][5] = 
{
	{0, 0, 0, 0, 0},
	{3, 0, 0, 0, 0},
	{0, 6, 0, 0, 0},
	{8, 0, 4, 0, 0},
	{9, 2, 0, 7, 0}
};

/*
Matrix struct is visual, but it waste space.
If there are 10000 node in a graph, we need 10000X10000 space, so it sacrifice space very much.
*/

/*
2. graph based on array.
To avoid wasting space, we can build a data space only with lines.
Such as:
*/
typedef struct _LINE
{
	int start;
	int end;
	int weight;
	int isDirection;
}LINE;
/*
start: is the start point
end : is the end point
weight: is the relationship.
isDirection: used to judge if the edge has direction.

How much lines in a graph, we need to define how much this data struct.
If we put all the lines in a array, then we can use this array to express a whole graph.

But this data structure pay more attention on line, and ignored start point.
because the line information has no significant without start point.
*/

/*
3. graph based on list
*/
typedef struct _LINE
{
	int end;
	int weight;
	struct _LINE* next;
}LINE;

typedef struct _VECTEX
{
	int start;
	int number;
	LINE* neighbor;
}VECTEX;
/*
VECTEX means a start point information. 
LINE means node information.
If LINE is a variable in VECTEX, then neighbor means all the start point of the  nodes is (int start).
*/

/*
If the line is a viriable in PATH, then the start point of next, is the node before current line.
*/
typedef struct _PATH
{
	int start;
	int end;
	int lenth;
	LINE* next;
}PATH;

/*
notes:
1. array and list is the base of graph.
2. every data structure has it's own feature, the key is to understand the ideology and methods.
3. the expression of graph is the basic of graph computation.
*/





























