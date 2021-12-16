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

extern string metric;
extern double delta;



double FILTER_E=2;

void stand_dev(vec* nvects,int no_of_vectors,int no_of_coordinates)
	{
	double c_sum=0;
	for (int i = 0; i < no_of_vectors; ++i)
		{
		double n_sum=0;
		for (int j = 0; j < no_of_coordinates; ++j)
			{
			n_sum+=nvects[i].coord[j];
			}
		double n_avg=n_sum/no_of_coordinates;

		double d_sum=0;
		for (int j = 0; j < no_of_coordinates; ++j)
			{
			double temp=nvects[i].coord[j]- n_avg;
			d_sum+=abs(temp);
			}
		c_sum+=(d_sum/no_of_coordinates);
		}
	FILTER_E=PERC_E*c_sum/no_of_vectors;cout<<"FILTER_E "<<FILTER_E<<endl;
	}

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
    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);
 	std::uniform_real_distribution<double>  distrTd(0.0,delta);

    int same_counter=0;
    double prev=0;

    for(int i=0;i<no_of_vectors;i++)
        {

        int no_of_coord=nvectors[i].coord.size();
        for(int j=0;j<no_of_coord;j++)
            {
            nvectors[i].coord[j]=(floor(((nvectors[i].coord[j]+distrTd(e))/delta))*delta);
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
