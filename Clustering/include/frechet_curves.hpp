#ifndef __frechet_curves__
#define __frechet_curves__

void stand_dev(vec* nvects,int no_of_vectors,int no_of_coordinates);
int Mean_duplicates(vector<double>* coord);

int Mean_filter(vector<double>* coord,int triads);
vector<double>* MeanCurve(vector<double> nvec,vector<double> qvec,int mv,int mq);
long double MeanNCurves(vector<vec*> nvects,vec*  cvec);
vec* snapping(vec* nvectors,int no_of_coordinates,int no_of_vectors,double delta,vector<double> td1,vector<double>td2);



#endif