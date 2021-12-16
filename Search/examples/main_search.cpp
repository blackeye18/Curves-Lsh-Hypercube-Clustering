/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
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
#include <chrono>
#include <queue>

using namespace std;
using namespace std::chrono;

#include "classes_and_defines.hpp"
#include "input_menu_starting_func.hpp"
#include "cube_basic_functions.hpp"
#include "lsh_basic_functions.hpp"
#include "knn_ranges_brutes.hpp"
#include "config.hpp"
#include "types.hpp"
#include "point.hpp"
#include "interval.hpp"
#include "curve.hpp"
#include "frechet.hpp"
#include "simplification.hpp"
#include "discrete_cont.hpp"
//string 
string metric = "euclidean_distance";

double delta;





int main(int argc, char *argv[]){
    //double delta;
    int L,k,M,probes,N=1;
    char input_file[256],query_file[256],output_file[256],metricfr[256],algorithm[256];
    vec* nvectors;
    vec* qvectors;
    //vec* testing_nvectors;
    int no_of_vectors,no_of_coordinates;
    int queries_no_of_vectors,queries_no_of_coordinates;
    Lhashtables *lht=NULL;
    hypercube *cube=NULL;

    int alg_flag=0;//1 gia LSH 2 gia Hypercube 3 gia Frechet discrete
    int metricfr_flag=0;//1 gia discrete 2 gia cont

    string lsh_or_hypercube="";

    
    if(input_handler(argc,argv,&k,&L,&probes,&delta,&M,metricfr,(input_file), query_file, output_file,algorithm))//elegxei an ta orismata exoun dothei apo thn grammh h prepei na ta zhthsei
        return -1;

    printf("input_file: %s, query_file: %s, output_file: %s,k:%d,L:%d,probes:%d,delta:%.2f,M:%d,metric: %s,algorithm: %s\n",input_file,query_file,output_file,k,L,probes,delta,M,metricfr,algorithm);
    
    if(strcmp(algorithm,"LSH")==0)
        alg_flag=1;
    else if(strcmp(algorithm,"Hypercube")==0)
        alg_flag=2;
    else if(strcmp(algorithm,"Frechet")==0){
        alg_flag=3;
        if(strcmp(metricfr,"discrete")==0){
            metric="LSH_Frechet_Discrete";
            metricfr_flag=1;
        }else if(strcmp(metricfr,"continuous")==0){
            metric="LSH_Frechet_Continuous";
            metricfr_flag=2;
        }else{
            cout<<"Unknown metric. Exiting."<<endl;
            return -1;
        }
    }
    else{
        cout<<"Unknown algorithm. Exiting."<<endl;
        return -1;
    }


    //Kwdikas gia Ai 
    //if(alg_flag==1 || alg_flag==2){
        if(alg_flag==1)
            lsh_or_hypercube="LSH_Vector";//string gia thn ektypwsh sto output file
        else if(alg_flag==2)
            lsh_or_hypercube="Hypercube"; 
        else if(alg_flag==3){
            if(metricfr_flag==1)
                lsh_or_hypercube=" LSH_Frechet_Discrete"; 
            else
                lsh_or_hypercube="LSH_Frechet_Continuous"; 
        }

        int flag=0;//flag gia to menu
        while(flag!=-1){
            //an to flag einai 0 exoume kainourio input kai query file
            //an to flag einai 1 exoume mono kainourio input file
            //an to flag einai 2 exoume mono kainourio query file
            if(flag==0 || flag==1){//ama exoume kainourio input file
                nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);//diavazoume to arxeio kai to apothikevvoume
                //testing_nvectors=open_and_create_vectors(input_file,&no_of_coordinates,&no_of_vectors);
                if(nvectors==NULL)
                    return -1;
                printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
                if(metric=="LSH_Frechet_Continuous")
                	stand_dev(nvectors,no_of_vectors,no_of_coordinates);
                
            }
           // cout<<"sdasdsds"<<endl;

            if(flag==0 || flag==2){//ama exoume kainourio query file
                qvectors=open_and_create_vectors(query_file,&queries_no_of_coordinates,&queries_no_of_vectors);//diavazoume to arxeio kai to apothikevoume
                if(qvectors==NULL)
                    return -1;
                printf("Queries:: queries_no_of_vectors: %d, queries_no_of_coordinates: %d\n",queries_no_of_vectors,queries_no_of_coordinates);
                
            }

            cout<<"Now using "<<algorithm<<endl;

            auto start1 = high_resolution_clock::now();//https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
            if(flag==0 || flag==1){//an exoume kainourio input file tote prepei na ksanadimourgisoume ta L ht gia to kainourio input file
                if(alg_flag==1 || alg_flag==3){
                    if(alg_flag==1)
                        lht=new Lhashtables(L,no_of_coordinates,k);//synarthsh arxikopoihshs
                    else if(alg_flag==3)
                        {
                        if(metricfr_flag==1)
                            lht=new Lhashtables(L,2*no_of_coordinates,k);//synarthsh arxikopoihshs//EDW
                        else if(metricfr_flag==2)
                            lht=new Lhashtables(1,no_of_coordinates,k);//synarthsh arxikopoihshs//EDW
                        }
                    lht->lsh_start(no_of_vectors,nvectors);//gemizoume ta ht 
                }else{
                    
                    cube=new hypercube(M,probes,no_of_coordinates,k,no_of_vectors);//arxikopoioume thn domh tou hypercube
                    cube->cube_start(no_of_vectors,nvectors);//insert ola ta vector sto cube
                }
            }
            auto stop1 = high_resolution_clock::now();
            auto duration1 = duration_cast<microseconds>(stop1 - start1);
            double time1=((double)duration1.count()/1000000);
            time1=time1/no_of_vectors;
            double time_per_query_lsh[queries_no_of_vectors];
            for(int i=0;i<queries_no_of_vectors;i++)//apothikevoume ton xrono poy xreiastike gia na dhmiourgithoun oi domes gia to lsh
                time_per_query_lsh[i]=time1;
            
            cout<<"Now using KNN"<<endl;
            vector<vector<dist_vec>*>* dsvec2=NULL;//kai prostetoume ton xrono pou xreiastike h knn gia kathe query antistoixa
            
            if(alg_flag==1 || alg_flag==3)
                dsvec2=lht->find_k_nearest(qvectors,N,queries_no_of_vectors,time_per_query_lsh);//synarthsh gia to knn
            else
                dsvec2=cube->all_NN_search(qvectors,N,queries_no_of_vectors,time_per_query_lsh);//knn search gia ola ta query
           
            if(dsvec2==NULL){//failsafe
                if(alg_flag==1 || alg_flag==3)
                    delete lht;
                else
                    delete cube;
                delete [] nvectors;
                delete [] qvectors;
                return -1;
            }

            double time_per_query_brute[queries_no_of_vectors];
            for(int i=0;i<queries_no_of_vectors;i++)//arxikopoioume me 0
                time_per_query_brute[i]=0;


            cout<<"Now using brute calculation"<<endl;//ypologizoume tous k nearest me brute force
            vector<vector<dist_vec>*>* dsvec3=NULL;
            dsvec3=brute_calculate_all(qvectors,nvectors,no_of_vectors,no_of_coordinates,queries_no_of_vectors,N,time_per_query_brute);
            
           
           
            //synarthsh pou ektypwnei ta dedomena pou mazepsame mesw twn prohgoumenwn synarthsewn kai ta ektypwnei sto arxeio
            print_to_file(output_file,lsh_or_hypercube,dsvec2,queries_no_of_vectors,qvectors,time_per_query_lsh,time_per_query_brute,dsvec3);

            cout<<"Output File Created!!"<<endl;
        //synarthsh gia to menu analoga me to ti thelei o xrhsths, an epistrepsei -1 tote h efarmogh termatizei
        flag=repeat_handler(nvectors,qvectors,input_file,query_file,output_file,lht,alg_flag,cube);
        for(int i=0;i<queries_no_of_vectors;i++){//Freeing memory

            vector<dist_vec>* dstemp2 =(*dsvec2)[i];
            vector<dist_vec>* dstemp3 =(*dsvec3)[i];
            //vector<dist_vec>* dstemp4 =(*dsvec4)[i];

            dstemp2->clear();delete dstemp2;
            dstemp3->clear();delete dstemp3;
            //dstemp4->clear();delete dstemp4;
        }
        

        dsvec2->clear();delete dsvec2;
        dsvec3->clear();delete dsvec3;
        //dsvec4->clear();delete dsvec4;
        }
        if(alg_flag==1 || alg_flag==3)
            delete lht;
        if(alg_flag==2)
            delete cube;
            
        delete [] nvectors;
        delete [] qvectors;
    //}
}
