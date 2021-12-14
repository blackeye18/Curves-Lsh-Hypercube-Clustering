#ifndef __input_menu_starting_func__
#define __input_menu_starting_func__
//gia oti den yparxei sxolio yparxoun sta arxeia tou lsh kai tou hypercube
int argsOK(int argc, char *argv[]);
int handle_conf_file(char configuration_file[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes);
int input_handler(int argc, char *argv[],char (&input_file)[256], char (&configuration_file)[256], char (&output_file)[256],char (&assigment)[256],char (&update)[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes,int*complete_flag,int* silhouette_flag);
vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors);
void print_to_file(vector<vec>* clustersvec,vector<vector<vec*>>* cluster_neighbours,int complete_flag,char output_file[256],char method[256],int no_of_coordinates,int no_of_vectors,vec* nvectors,double time1, vector<long double>* silhouette_vec);

#endif