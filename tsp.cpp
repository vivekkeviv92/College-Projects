/* This file contains some code that may help you in programming assignment 2,
   but which you are not required to use. It includes:

   code for randomly generating TSP problem instances
   code for solving TSP using depth-first branch-and-bound 
        with minimal spanning tree heuristic
*/

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_CITIES 5
#define LARGE_VAL 1000000000
#define START_NODE 0

typedef struct city_node *city_node_ptr;

struct city_node{
  int city_id;
  int cities_visited;
  int depth;
  struct city_node* parent;
  int cost;
};

int best_cost = LARGE_VAL;
city_node_ptr leaf_node_of_best_path = NULL;


/* A triangular matrix is sufficient for symmetric problem, 
   but a square matrix is useed for consistency with assymetric problem. */
int problem_matrix[NUM_CITIES][NUM_CITIES];

//using namespace std;

/* A simple symmetric travelling salesman person problem generator.
   Assumes integer distances, bounds specified, fully connected graph. */
void stsp_problem_generator(int num_cities, int min , int max)  //Larger is the differece between max and min, simpler is the problem.
{
  int i,j;
  double random_distance;
	
  for (i=0;i<num_cities;++i)
    for (j=0;j<i;++j)
      {
	random_distance = (float) ( (float)rand()/RAND_MAX );
	problem_matrix[i][j] = problem_matrix[j][i] = (int)(min + (max - min)  * random_distance); 
      }
  for(i=0;i<NUM_CITIES;++i)
    problem_matrix[i][i] = LARGE_VAL; 
}

void print_problem()
{
  int i,j;
  for(i=0;i<NUM_CITIES;++i)
    {
      printf("\n");
      for(j=0;j<NUM_CITIES;++j)
	printf("%d\t",problem_matrix[i][j]);
    }
}

int s[NUM_CITIES][NUM_CITIES];
int min_one_tree_cost=0;

void heuristic_min_one_tree()
{
  int nearest[NUM_CITIES] ; // Index of vertex nearest to current vertex
  int distance[NUM_CITIES] ; // Weight on edge between current vertex and other vertex
  int near_el = 0;			// The index for which distance[i] is the smallest
  int tmp_nearest, min;
  int min1, min2;
  int min1_pos, min2_pos;
  int i,j,z;

  for(i=0;i<NUM_CITIES;i++)
    for(j=0;j<NUM_CITIES;j++)
      s[i][j]=0;

  for ( i = 0; i <NUM_CITIES-1; i++)
    {
      nearest[i] = 0;
      // For all vertices, initialize current vertex 
      // to be the nearest vertex in the set.
      distance[i] = problem_matrix[1][i+1];
      // initialize the distance from the set
      // to be the weight on the edge to current vertex.
    }
  for ( z = 0; z < NUM_CITIES-1; z++)
    {
      min = LARGE_VAL;
      for ( i = 0; i < NUM_CITIES-1; i++) // Check each vertex to see which one is the nearest
	{
	  if ((distance[i] >= 0) && (distance[i] < min))
	    {
	      min = distance[i];
			near_el = i;
		 }
	}
		tmp_nearest = nearest[near_el];
		min_one_tree_cost=min_one_tree_cost-s[near_el+1][tmp_nearest+1];
		s[near_el+1][tmp_nearest+1] = problem_matrix[tmp_nearest+1][near_el+1]; //Add it to the set
		min_one_tree_cost=min_one_tree_cost+s[near_el+1][tmp_nearest+1];
		distance[near_el] = -1;
		for ( i = 1; i < NUM_CITIES-1; i++)
	{
	  if (problem_matrix[near_el+1][i+1] < distance[i])
		 {
			distance[i] = problem_matrix[near_el+1][i+1]; // Update its distance
			nearest[i] = near_el;
		 }
	}
	 }

  min1=LARGE_VAL;
  min2=LARGE_VAL;
  min1_pos=min2_pos=0;
  for (i=0;i<NUM_CITIES;++i)
	 if(problem_matrix[i][0]<min2)
		{
	if(problem_matrix[i][0]<min1)
	  {
		 min2=min1;
		 min2_pos=min1_pos;
		 min1=problem_matrix[i][0];
		 min1_pos=i;
	  }
	else{
	  min2=problem_matrix[i][0];
	  min2_pos=i;
	}
		}
  s[0][min1_pos]=min1;
  s[0][min2_pos]=min2;

  if((min1!=LARGE_VAL) && (min2!=LARGE_VAL))
	 min_one_tree_cost=min_one_tree_cost+min1+min2;

  else if((min2!=LARGE_VAL) && (min1==LARGE_VAL))
	 min_one_tree_cost=min_one_tree_cost +min2;

  for(i=0;i<NUM_CITIES;i++)
	 {
		printf("\n");
		for(j=0;j<NUM_CITIES;j++)
	printf(" %d ",s[i][j]);
	 }

  printf("\nmin_one_tree_cost=%d",min_one_tree_cost);
  cout << endl;

}


int min_span_tree_cost= 0;
void heuristic_spanning_tree() // Using Prim's algo
{
  int nearest[NUM_CITIES] ;
  int distance[NUM_CITIES] ;
  int near_el = 0;
  int tmp_nearest, min;
  int i,j,z;

  for(i=0;i<NUM_CITIES;i++)
	 for(j=0;j<NUM_CITIES;j++)
		s[i][j]=0;

  for ( i = 0; i <NUM_CITIES; i++)
	 {
		nearest[i] = 0;
		distance[i] = problem_matrix[0][i];
	 }
  for ( z = 0; z < NUM_CITIES; z++)
	 {
		min = LARGE_VAL;
		for ( i = 1; i < NUM_CITIES; i++) // Check each vertex to see which one is the nearest
	{
	  if ((distance[i] >= 0) && (distance[i] < min))
		 {
			min = distance[i];
			near_el = i;
		 }
	}
		tmp_nearest = nearest[near_el];
		min_span_tree_cost=min_span_tree_cost-s[near_el][tmp_nearest];
		s[near_el][tmp_nearest] = problem_matrix[tmp_nearest][near_el]; //Add it to the set
		min_span_tree_cost=min_span_tree_cost+s[near_el][tmp_nearest];
		distance[near_el] = -1;
		for ( i = 0; i < NUM_CITIES; i++)
	{
	  if (problem_matrix[near_el][i] < distance[i])
		 {
			distance[i] = problem_matrix[near_el][i]; // Update its distance
			nearest[i] = near_el;
		 }
	}
	 }
  printf("\nMin_Span_Tree Cost  %d",min_span_tree_cost);
  for(i=0;i<NUM_CITIES;i++)
	 {
		printf("\n");
		for(j=0;j<NUM_CITIES;j++)
	printf(" %d",s[i][j]);
	 }
}

city_node_ptr two_opt(city_node_ptr temp_node)//two opt functio
{
	int i = 1;
	int j = i+1;

	problem_matrix[i][j] = problem_matrix[i+1][j+1];
	return problem_matrix[i][j];

}



void solve_tsp_dfbnb(city_node_ptr temp_node)  //not using greedy approach,
{
  int city_pos;
  city_node_ptr node;

  if(temp_node->depth == NUM_CITIES - 1)
	 {
		if(temp_node->cost + problem_matrix[temp_node->city_id][0] < best_cost)
	{
	  best_cost = temp_node->cost + problem_matrix[temp_node->city_id][0];
	  leaf_node_of_best_path = temp_node;
	}
		return;

	 }
  for(int i=1;i<NUM_CITIES;++i)
	 {
		city_pos = static_cast<int>(pow(2.0, i));
		if((temp_node->cities_visited & city_pos) != 0)
	continue;

		node = (city_node_ptr) malloc(sizeof(struct city_node));
		node->city_id = i;

		node->cities_visited = temp_node->cities_visited | city_pos;
		node->cost = temp_node->cost + problem_matrix[temp_node->city_id][i];
		node->depth = temp_node->depth +1;
		node->parent = temp_node;

		if(node->cost > best_cost)
	{
	  free(node);
	  continue;
	}
		solve_tsp_dfbnb(node);
	 }
  return;
}

void trace_path(city_node_ptr n)
{
  if(n->depth==0)
	 {
		printf("%d\t",n->city_id);
		return;
	 }

  trace_path(n->parent);
  printf("%d\t",n->city_id);

  return;
}

int main()
{
  city_node_ptr start_node;
  city_node_ptr tmp_node;
  city_node_ptr modified_node;
  srand ((unsigned)time(NULL));
  stsp_problem_generator(NUM_CITIES,12,80);
  print_problem();
  start_node = (city_node_ptr) malloc(sizeof(struct city_node));
  start_node->city_id =0;
  start_node->cities_visited = 1;
  start_node->cost = 0;
  start_node->depth =0;
  start_node->parent =NULL;
  //solve_tsp_dfbnb(start_node);
  do
  {
	  modified_node = two_opt(start_node);
		  if (modified_node->cost < start_node->cost)
		  {
			  tmp_node = start_node;
			  start_node = modified_node;
		  }
  }while(tmp_node->cost < start_node->cost);
	
  printf("\n\n Best Cost= %d",best_cost);
  cout << endl;
  printf("\nPath:   \n");
  trace_path(leaf_node_of_best_path);
  printf("0\t");
  
  heuristic_spanning_tree();
  printf("\n");
  heuristic_min_one_tree();
}



