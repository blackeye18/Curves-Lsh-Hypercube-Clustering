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
#include <queue>
#include <bitset>


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
unsigned int hammingDistance(unsigned int n1, unsigned int n2)//https://www.geeksforgeeks.org/hamming-distance-between-two-integers/
	{
    unsigned int x = n1 ^ n2;//btwise exclusive or(XOR vgazi 1 otan n1 0 kai n2 1 h n1 1 n2 0)
    unsigned int setBits = 0;
 
    while (x > 0) {//ousiastika opou exei 0 to kanume 1 kai opou 1 to kanume 0 giati thelume to XNOR(NOT XOR) tu n1 n2  
        setBits += x & 1;
        x >>= 1;
    }
 
    	return setBits;
	}

/*
//https://stackoverflow.com/questions/11714555/euclidean-integer-modulo-in-c
long int euclidean_remainder(long int a,long int b)//ypologismos tou eyklidiou apolytou
{
  assert(b != 0);//sigourevoume oti den kanoume diairesh me 0
  long int r = a % b;
  return r >= 0 ? r : r + abs(b);//an ypoloipo megalytero 0 epistrefoume to ypoloipo diaforetika epistrefoume to apolyto prosthetontas thn apolyth timh tou b
}

int h_function(vector<double> p,vector<double> v,double t){//ypologismos ths synarthshs h opws anaferetai stis diafanies
    double in_prod;
    in_prod=inner_product(p.begin(),p.end(),v.begin(),0);//vriskoume to ginomeno twn 2 vector(p kai v ) ta opoia einai idiwn diastasewn
    in_prod+=t;//prosthetoume thn tyxaia timh t 
    double sum=in_prod/W;//diairoume me w meta3i 0 kai 6, define sto header classes
    int h=floor(sum);//pairnoume to katw orio tou apotelesmatos
    return h;

}
*/
hypercube::hypercube(int M,int probes,int d ,int k,int no_of_vectors)//constructor
	:M(M),probes(probes),d(d),k(k),no_of_vectors(no_of_vectors)
	{
	labels_count=no_of_vectors/BUCKET_RATIO;//labels_count gia to pinaka tis f(h(x))
	}

void hypercube::Hashfun_init(void)        
    {
    v.resize(k,vector<double> (d));//disdiastatos pinakas gia ta v

    labels.resize(k,vector<int>(labels_count));//disdiastatos pinakas gia tin f(h(x))


    cube_vec.resize(powk,vector<vec*>(0));

    unsigned seed=std::chrono::steady_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    std::uniform_int_distribution<int>  distrl(0,1);//paragei ena akereo 0 1 me vasi uniform katanomis
    std::uniform_real_distribution<double>  distrT(0.0,W);//paragei ena akereo 0 - w me vasi uniform real katanomis

    for (int ki = 0; ki < k; ++ki)
        {
        t.push_back(distrT(e));

        for (int di = 0; di < d; ++di)
            {
            v[ki][di]=normal_dist_generator();
            }

        for (int la = 0; la < labels_count; ++la)
            {
            labels[ki][la]=distrl(e);
            }
        }

    }
unsigned int hypercube::hash_calc(vec* vect)//ipologismos tu bin hashvalue
    {
    unsigned int bits=0;
    for (int ki = 0; ki < k; ++ki)
        {
        int h=h_function(vect->coord,v[ki],t[ki]);//kalume tin h_function
        //cout<<"labels_count: "<<labels_count<<endl;
        int hmod=euclidean_remainder(h,labels_count);
        bits=bits<<1;//aristero shift kata ena
        //cout<<labels[ki][hmod];
        bits+=labels[ki][hmod];//to vriskume sto pinaka me ta labels ousiastika i f(h(x))
        }
      //cout<<endl;
    return bits;
    }
int hypercube::vec_insert(vec* vect)
    {
    unsigned int ki=this->hash_calc(vect);

    cube_vec[ki].push_back(vect);//isagoume to vec stin katalili thesi ston cube_vec
    return 0;
    }

void hypercube::cube_start(int no_of_vectors,vec*nvectors){
	powk=pow(2,k);//ipologizoume to megethos tu  cube_vec
	this->Hashfun_init();//arxikopoioume tus pinakes pu xrisomopoioume gia to hashing
	int ret=0;
	for(int i=0;i<no_of_vectors;i++){
		ret=vec_insert(&(nvectors[i]));//kanoume insert ola ta vec ston cube_vec
		if(ret!=0){
            cout<<"Something went wrong with vec_insert"<<endl;
            return;
        }
	}
}