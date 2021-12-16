/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
#ifndef __discrete_cont__
#define __discrete_cont__

void stand_dev(vec* nvects,int no_of_vectors,int no_of_coordinates);
int filtering(vec* nvectors,int no_of_coordinates,int no_of_vectors,int triads);
int snapping_cont(vec* nvectors,int no_of_vectors,double delta,vector<double> td1);
int padding_cont(vec* nvectors,int no_of_vectors,int no_of_coordinates,double delta);
int mini_maxi_cont(vec* nvectors,int no_of_vectors,double delta);
int key_cont(vec* nvectors,int no_of_vectors,int no_of_coordinates,double delta,vector<double> td1);
vec* snapping(vec* nvectors,int no_of_coordinates,int no_of_vectors,double delta,vector<double> td1,vector<double>td2);





#endif