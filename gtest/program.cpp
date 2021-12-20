
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
#include <algorithm>
#include <random>
using namespace std;
#define FILTER_E 4
#define M_big_num 9999

int Mean_filter(vector<double>* coord,int triads)//sxolia sto search
    {
        int temp_no_coord=coord->size()-1;
        for (int j = 1; j < temp_no_coord; ++j)
            {
            double a=coord->at(j-1);
            double b=coord->at(j);
            double c=coord->at(j+1);

            if(abs(a-b)<=FILTER_E && abs(b-c)<=FILTER_E)
                {
                coord->erase(coord->begin() + j);
                temp_no_coord--;

                if(!triads)
                    j--;
                }

            }
        
    return 0;
    }
 
 long double dfd(vector<double> nvec,vector<double> qvec,int mv,int mq)
    {

    vector<vector<long double>> C;
    C.resize(mv,vector<long double>(mq));

    long double dist=abs(nvec[0]-qvec[0]);
    C[0][0]=dist;


    for (int i = 1; i < mq; ++i)
        {
        long double dist=abs(nvec[0]-qvec[i]);
        C[0][i]=max(dist,C[0][i-1]);
        }

    for (int i = 1; i < mv; ++i)
        {
        long double dist=abs(nvec[i]-qvec[0]);
        C[i][0]=max(dist,C[i-1][0]);
        }

    for (int i = 1; i < mv; ++i)
        {
        for (int j = 1; j < mq; ++j)
            {
            long double dist=abs(nvec[i]-qvec[j]);

            long double mprev=min(min(C[i-1][j],C[i-1][j-1]),C[i][j-1]);
            C[i][j]=max(dist,mprev);
            }
        }
        //cout<<"FINISHED CALCULATION"<<endl;
    return C[mv-1][mq-1];
    }


    long double vect_dist(vector<double> vecA,vector<double> vecB,int d)
{
    //cout<<"using vect_dist"<<endl;
    long double sum=0;
    for (int i = 0; i < d; ++i)
        {
        long double temp=abs(vecA[i]-vecB[i]);
        sum+=temp*temp;
        }
    return sqrt(sum);
}

long int euclidean_remainder(long int a,long int b)
{
  assert(b != 0);
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);
}

/*
int main(int argc, char const *argv[])
	{


	vector<double> nvec{5,6,7,8,11.5,12,14};int mv=nvec.size();
	vector<double> qvec{6.5,6.6,3.1,11};int mq=qvec.size();


	long double dist=dfd(nvec,qvec,mv,mq);

	cout<<"dist "<<dist<<endl;

	vec nvect[4];
	
	nvect[0].coord.insert(nvect[0].coord.end(),{5,6,7,8,11.5,12,14});

	cout<<"before filtering"<<endl;
	for (int i = 0; i < nvect[0].coord.size(); ++i)
		{
		cout<<nvect[0].coord[i]<<" ";
		}
	cout<<endl;

	nvect[1].coord.insert(nvect[1].coord.end(),{5,6,7,8,11.5,12,14});
	nvect[2].coord.insert(nvect[2].coord.end(),{5,6,7,8,11.5,12,14});
	nvect[3].coord.insert(nvect[3].coord.end(),{5,6,7,8,11.5,12,14});

	filtering(nvect,nvect[0].coord.size(),4,0);

	cout<<"after filtering"<<endl;
	for (int i = 0; i < nvect[0].coord.size(); ++i)
		{
		cout<<nvect[0].coord[i]<<" ";
		}
	cout<<endl;



	return 0;
	}*/