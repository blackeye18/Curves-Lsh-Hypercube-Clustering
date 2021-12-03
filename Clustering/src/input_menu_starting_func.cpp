#include <iostream>
#include <cstring>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <map>
#include <cassert>
#include <queue>
#include <bitset>
#include <algorithm>


using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "lsh_basic_functions.hpp"
#include "cube_basic_functions.hpp"
#include "cluster_main_functions_silhouette.hpp"
//#include "knn_ranges_brutes.hpp"




//elegxoume an to # twn arguments einai swsto
int argsOK(int argc, char *argv[])
{
    if (argc == 13 || argc== 12 || argc ==11)
    {
        //printf("Error in # of args\n");
        return 1;
    }
    return 0;
}
//synarthsh pou diavazei to conf file 
int handle_conf_file(char configuration_file[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes){
	ifstream conf;
    conf.open(configuration_file);//ama den yparxei
    if(!conf){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return 1;
    }

    string sline;
    string tok;
//default values:
    (*L)=3;//vazoume default times 
    (*k_lsh)=4;
    (*M)=10;
    (*k_hypercube)=3;
    (*probes)=2;
    int count=0;
     while (getline(conf,sline))//diavazoume kathe grammh
    {	count++;
    	stringstream line(sline);
    	string temp1,temp2;
    	line>> tok;//thn spame analoga me ta kena
    	temp1=tok;
    	line>> tok;
    	temp2=tok;

    	if(count>6){
    		cout<<"Something went wrong with configuration_file... Exiting.."<<endl;//ama einai parapanw apo tis megistes grammes, kati paei lathos
    		return 1;
    	}
    	//cout<<"temp1: "<<temp1;
    	if(temp1=="number_of_clusters:")//kai kanoyme tis analoges allages
    		(*K_medians)=atoi(temp2.c_str());
    	else if(temp1=="number_of_vector_hash_tables:")
    		(*L)=atoi(temp2.c_str());
    	else if(temp1=="number_of_vector_hash_functions:")
    		(*k_lsh)=atoi(temp2.c_str());
    	else if(temp1=="max_number_M_hypercube:")
    		(*M)=atoi(temp2.c_str());
    	else if(temp1=="number_of_hypercube_dimensions:")
    		(*k_hypercube)=atoi(temp2.c_str());
    	else if(temp1=="number_of_probes:")
    		(*probes)=atoi(temp2.c_str());
    	else {
    		cout<<"Something went wrong with configuration_file... Exiting.."<<endl;
    		return 1;
    	}

    }

return 0;
}

int input_handler(int argc, char *argv[],char (&input_file)[256], char (&configuration_file)[256], char (&output_file)[256],char (&assigment)[256],char (&update)[256],int* K_medians,int*L,int*k_lsh,int*M,int*k_hypercube,int*probes,int*complete_flag,int* silhouette_flag){

	if(!argsOK(argc,argv)){
		cout<<"You entered something wrong.. Exiting"<<endl;
		return 1;
	}
	if(argc==13){//ama exoun dothei ola ta orismata apo grammh edolwn
	
		strcpy(input_file,argv[2]);
		strcpy(configuration_file,argv[4]);
		strcpy(output_file,argv[6]);
		if(strcmp(argv[11],"-complete")==0){
			cout<<"Complete parameter given!"<<endl;
			(*complete_flag)=1;
		}else{
			cout<<"Something is wrong with input parameters... Exiting.."<<endl;
			return 1;
		}
        if(strcmp(argv[12],"-silhouette")==0){
            cout<<"Silhouette parameter given!"<<endl;
            (*silhouette_flag)=1;
        }else{
            cout<<"Something is wrong with input parameters... Exiting.."<<endl;
            return 1;
        }
        strcpy(update,argv[8]);
		strcpy(assigment,argv[10]);
        if(handle_conf_file(configuration_file,K_medians,L,k_lsh,M,k_hypercube,probes))
            return 1;
    }else if(argc==12){
        (*complete_flag)=0;
        (*silhouette_flag)=0;
        strcpy(input_file,argv[2]);
        strcpy(configuration_file,argv[4]);
        strcpy(output_file,argv[6]);
        if(strcmp(argv[11],"-complete")==0){
            cout<<"Complete parameter given!"<<endl;
            (*complete_flag)=1;
        }
        if(strcmp(argv[11],"-silhouette")==0){
            cout<<"Silhouette parameter given!"<<endl;
            (*silhouette_flag)=1;
        }
        strcpy(update,argv[8]);
        strcpy(assigment,argv[10]);
        if(handle_conf_file(configuration_file,K_medians,L,k_lsh,M,k_hypercube,probes))
            return 1;
    }else if(argc==11){
        (*complete_flag)=0;
        (*silhouette_flag)=0;
        strcpy(input_file,argv[2]);
        strcpy(configuration_file,argv[4]);
        strcpy(output_file,argv[6]);
        strcpy(update,argv[8]);
        strcpy(assigment,argv[10]);
        if(handle_conf_file(configuration_file,K_medians,L,k_lsh,M,k_hypercube,probes))
            return 1;
    
	}else{//ama exoun dothei mono ta vasika orismata apo grammh edolwn
		cout<<"Something went wrong. Exiting..."<<endl;
        return 1;

	}

return 0;
}

vec* open_and_create_vectors(char input_file[256],int* no_of_coordinates,int *no_of_vectors){
    //cout<<"aaa1";
    int counter=0;
    int flag=0;
    ifstream input;
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }

    string sline;
    string tok;


    (*no_of_vectors)=0;
    (*no_of_coordinates)=-1;//-1 giati metrame kai to arxiko id pou den einai coord

    while (getline(input,sline))
    {
        (*no_of_vectors)++;
        
        if((*no_of_vectors)==1){
            stringstream line(sline);

            while(line>> tok)
                (*no_of_coordinates)++;
        }
    }
    input.close();
    input.clear();
    input.open(input_file);
    if(!input){
        cout<<"Something Terrible Happened! Maybe the input_file doesn't exist?! Exiting.."<<endl;
        return NULL;
    }
    vec* nvectors;
    nvectors = new vec[(*no_of_vectors)];
    while (getline(input,sline))
    {       flag=0;
            nvectors[counter].clustered_flag=-1;
            stringstream line(sline);
            while(line>> tok) {
                if(flag==0){
                    //cout<<"TOK:"<<tok<<endl;
                    nvectors[counter].name=tok;
                    flag=1;
                }
                else
                    nvectors[counter].coord.push_back(stof(tok));
                    
            }
        counter++;
    }
    //cout<<"NEW FUNCTION: no_of_vectors:"<<(*no_of_vectors)<<"no_of_coordinates:"<<(*no_of_coordinates)<<endl;
    return nvectors;
            
}
//print to file 
void print_to_file(vector<vec>* clustersvec,vector<vector<vec*>>* cluster_neighbours,int complete_flag,char output_file[256],char method[256],int no_of_coordinates,int no_of_vectors,vec* nvectors,double time1, vector<long double>* silhouette_vec){

    ofstream outfile;
    outfile.open(output_file);
    outfile<<"Algorithm: ";
    if(strcmp(method,"Classic")==0)
        outfile<<"Lloyds"<<endl;
    else if(strcmp(method,"LSH")==0)
        outfile<<"Range Search LSH"<<endl;
    else if(strcmp(method,"Hypercube")==0)
        outfile<<"Range Search Hypercube"<<endl;
    for(int i=0;i<clustersvec->size();i++){
        outfile<<"CLUSTER-"<<i+1<<" {size: "<<(*cluster_neighbours)[i].size()<<", centroid: ";
        for(int j=0;j<no_of_coordinates;j++){
            outfile<<clustersvec->at(i).coord[j]<<" ";
        }
        outfile<<"}"<<endl;
    }
    outfile<<"clustering_time: "<<time1<<endl;
    outfile<<"Silhouette: [";
    double sum=0;
    if(silhouette_vec!=NULL){
        
        for(int i=0;i<silhouette_vec->size();i++){
            sum+=(*silhouette_vec)[i];
        }
        sum=sum/silhouette_vec->size();
        for(int i=0;i<silhouette_vec->size();i++){
            outfile<<(*silhouette_vec)[i]<<",";
        }
        outfile<<sum<<"]";
    }
    if(complete_flag==1){
        outfile<<endl;
        for(int i=0;i<clustersvec->size();i++){
        outfile<<"CLUSTER-"<<i+1<<" {size: "<<(*cluster_neighbours)[i].size()<<", centroid: ";
        for(int j=0;j<no_of_coordinates;j++){
            outfile<<clustersvec->at(i).coord[j]<<" ";
        }
        //outfile<<",";
       for(int k=0;k<(*cluster_neighbours)[i].size();k++){
        outfile<<", "<<((*cluster_neighbours)[i])[k]->name;
        }
       outfile<<"}"<<endl;
        } 
    }   
    return;
}
