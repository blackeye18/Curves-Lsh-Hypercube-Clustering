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

//string 
string metric = "euclidean_distance";

double delta;


int filtering(vec* nvectors,int no_of_coordinates,int no_of_vectors,int triads)
    {

    for (int i = 0; i < no_of_vectors; ++i)
        {
        int temp_no_coord=no_of_coordinates-1;
        for (int j = 1; j < temp_no_coord; ++j)
            {
            double a=nvectors[i].coord[j-1];
            double b=nvectors[i].coord[j];
            double c=nvectors[i].coord[j+1];

            if(abs(a-b)<=FILTER_E && abs(b-c)<=FILTER_E)
                {
                nvectors[i].coord.erase(nvectors[i].coord.begin() + j);
                temp_no_coord--;

                if(!triads)
                    j--;
                }

            }
        }
    return 0;
    }

int snapping_cont(vec* nvectors,int no_of_vectors,double delta)
    {

    int same_counter=0;
    double prev=0;

    for(int i=0;i<no_of_vectors;i++)
        {

        int no_of_coord=nvectors[i].coord.size();
        for(int j=0;j<no_of_coord;j++)
            {
            nvectors[i].coord[j]=(floor((nvectors[i].coord[j]/delta)+1/2)*delta);
            }
        }

    return 0;
    }

int padding_cont(vec* nvectors,int no_of_vectors,int no_of_coordinates,double delta)
    {
     for(int i=0;i<no_of_vectors;i++)
        {//kanw adistoixish me meres 

        int no_of_coord=nvectors[i].coord.size();
        for(int j=no_of_coord;j<no_of_coordinates;j++)
            {
            nvectors[i].coord.push_back(M_big_num);
            }
        }
    return 0;
    }

int mini_maxi_cont(vec* nvectors,int no_of_vectors,double delta)
    {
    for(int i=0;i<no_of_vectors;i++)
        {//kanw adistoixish me meres 

        int no_of_coord=nvectors[i].coord.size();
        for(int j=1;j<no_of_coord-1;j++)
            {
            double mini=min(nvectors[i].coord[j-1],nvectors[i].coord[j+1]);
            if(mini<nvectors[i].coord[j])
                {
                double maxi=max(nvectors[i].coord[j-1],nvectors[i].coord[j+1]);
                if(nvectors[i].coord[j]<maxi)
                    nvectors[i].coord.erase(nvectors[i].coord.begin() + j);
                }
            }
        }
    return 0;
    }
/*
int preprocessing_cont(vec* nvectors,int no_of_vectors,int no_of_coordinates,double delta)
    {
    filtering(nvectors,no_of_coordinates,no_of_vectors,0);
    //snapping_cont(nvectors,no_of_vectors,delta);
    //mini_maxi_cont(nvectors,no_of_vectors,delta);
    padding_cont(nvectors,no_of_vectors,no_of_coordinates,delta);
    return 0;
    }*/

int key_cont(vec* nvectors,int no_of_vectors,int no_of_coordinates,double delta)
    {
    filtering(nvectors,no_of_coordinates,no_of_vectors,0);
    snapping_cont(nvectors,no_of_vectors,delta);
    mini_maxi_cont(nvectors,no_of_vectors,delta);
    padding_cont(nvectors,no_of_vectors,no_of_coordinates,delta);
    return 0;
    }






vec* snapping(vec* nvectors,int no_of_coordinates,int no_of_vectors,double delta,vector<double> td1,vector<double>td2){
    vector<vector<vector<double>>> temp;
    temp.resize(no_of_vectors,vector<vector<double> >(no_of_coordinates,vector<double>(2)));////resize analoga 
    for(int i=0;i<no_of_vectors;i++){//kanw adistoixish me meres 
        for(int j=0;j<no_of_coordinates;j++){
            temp[i][j][0]=(j);
            temp[i][j][1]=nvectors[i].coord[j];
        }
    }
    
    int same_counter=0;
    double prev1=0,prev2=0;
    int first_flag=0;
    vector<vector<vector<double>>> temp2;
    temp2.resize(no_of_vectors,vector<vector<double> >(no_of_coordinates));////resize analoga 
    for(int i=0;i<no_of_vectors;i++){//kanw adistoixish me meres 
        first_flag=0;

        for(int j=0;j<no_of_coordinates;j++){
            if(first_flag==0){
                temp2[i][j].push_back(floor(((temp[i][j][0]-td1[j])/delta)+1/2)*delta +td1[j]);
                temp2[i][j].push_back(floor(((temp[i][j][1]-td2[j])/delta)+1/2)*delta + td2[j]);
                prev1=temp2[i][j][0];
                prev2=temp2[i][j][1];
                first_flag=1;
            }else{
                double temphold1,temphold2;
                temphold1=(floor((temp[i][j][0]/delta)+1/2));
                temphold2=(floor((temp[i][j][1]/delta)+1/2));
                if(temphold1==prev1 && temphold2==prev2){
                    same_counter++;
                }else{
                    temp2[i][j].push_back(temphold1);
                    temp2[i][j].push_back(temphold2);
                    prev1=temphold1;
                    prev2=temphold2;
                }

            }
        }
    }
    //cout<<same_counter<<endl;
    //cout<<temp2[0][1].size()<<endl;
    vec* newnvectors;
    newnvectors = new vec[no_of_vectors];//desmevoume xwro gia ta vectors
    for(int i=0;i<no_of_vectors;i++){
        newnvectors[i].name=nvectors[i].name;
    }
    for(int i=0;i<no_of_vectors;i++){
        for(int j=0;j<no_of_coordinates;j++){
            for(int k=0;k<temp2[i][j].size();k++){
                newnvectors[i].coord.push_back(temp2[i][j][k]);

            }
        }
    }
    for(int i=0;i<no_of_vectors;i++){
        int tempint=newnvectors[i].coord.size();
        while(tempint!=no_of_coordinates*2){
            newnvectors[i].coord.push_back(M_big_num);
            newnvectors[i].coord.push_back(M_big_num);
            tempint=newnvectors[i].coord.size();
        }
    }
    //delete [] nvectors;
    // for(int i=0;i<no_of_vectors;i++){
    //     cout<<newnvectors[i].name<<endl;
    // }
    return newnvectors;
}







int main(int argc, char *argv[]){
    //double delta;
    int L,k,M,probes,N=1;
    char input_file[256],query_file[256],output_file[256],metricfr[256],algorithm[256];
    vec* nvectors;
    vec* qvectors;
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
                if(nvectors==NULL)
                    return -1;
                printf("Input:: no_of_vectors: %d, no_of_coordinates: %d\n",no_of_vectors,no_of_coordinates);
                /*
                if(metricfr_flag==2){
                    preprocessing_cont(nvectors,no_of_vectors,no_of_coordinates,delta);
                    //no_of_coordinates=no_of_coordinates*2;
                    }*/
            }
           // cout<<"sdasdsds"<<endl;

            if(flag==0 || flag==2){//ama exoume kainourio query file
                qvectors=open_and_create_vectors(query_file,&queries_no_of_coordinates,&queries_no_of_vectors);//diavazoume to arxeio kai to apothikevoume
                if(qvectors==NULL)
                    return -1;
                printf("Queries:: queries_no_of_vectors: %d, queries_no_of_coordinates: %d\n",queries_no_of_vectors,queries_no_of_coordinates);
                //if(alg_flag==3){
                   // qvectors=snapping(qvectors,queries_no_of_coordinates,queries_no_of_vectors,delta);
                   // queries_no_of_coordinates=queries_no_of_coordinates*2;
               // }
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
                            lht=new Lhashtables(L,no_of_coordinates,k);//synarthsh arxikopoihshs//EDW
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
            vector<vector<dist_vec>*>* dsvec2;//kai prostetoume ton xrono pou xreiastike h knn gia kathe query antistoixa
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
            vector<vector<dist_vec>*>* dsvec3;
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
