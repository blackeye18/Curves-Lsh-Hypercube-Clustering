/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
#ifndef __input_menu_starting_func__
#define __input_menu_starting_func__

int argsOK(int argc, char *argv[]);//elegxei an o arithmos twn arguments einai swstos
int input_handler(int argc, char *argv[],int* k, int* L,int* probes,double* delta,int* M,char (&metric)[256],char (&input_file)[256], char (&query_file)[256], char (&output_file)[256],char (&algorithm)[256]);//synarthsh pou elegxei an prepei na diavastei kati mesa apo thn efarmogh
vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors);//diavazei ola ta vector apo to inputfile(eite afto einai input eite query)
void print_vectors(vec *nvectors,int no_of_vectors,int no_of_coordinates);//emfanizei ola ta vectors, voithikh synarthsh
int print_to_file(char output_file[256],string lsh_or_hypercube,vector<vector<dist_vec>*>* dsvec2,int queries_no_of_vectors,vec* qvectors,double* time_per_query_lsh,double *time_per_query_brute,vector<vector<dist_vec>*>* dsvec3);//ektypwnei to output file opws afto zhteite
int repeat_handler(vec* nvectors, vec* qvectors,char* input_file,char*query_file,char* output_file,Lhashtables *lht);//synarthsh pou einai ypefthinh gia to menu pou emfanizetai sto telos tou kathe treksimatos
#endif