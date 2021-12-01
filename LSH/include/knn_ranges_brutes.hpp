/* Georgios Georgiou sdi1800220 - Panagiotis Mavrommatis sdi1800115 */
#ifndef __knn_ranges_brutes__
#define __knn_ranges_brutes__

//node* hashtable::search_nd(long hvalue);
long double vect_dist(vector<double> vecA,vector<double> vecB,int d);//ypologismos tis eyklideias apostashs metaksy 2 vector ston d diastato xwro
vector<dist_vec>* brute_calculate(vec* qvector,vec* nvectors,int no_of_vectors,int no_of_coordinates,int N,int pos);//synarthsh pou ypologizei me brute(ena ena ta vector me kathe query) tous kodinoterous geitones gia ena query
vector<vector<dist_vec>*>* brute_calculate_all(vec* qvectors,vec* nvectors,int no_of_vectors,int no_of_coordinates,int queries_no_of_vectors,int N,double* time_per_query_brute);//synarthsh poy yplogizei me brute gia ola ta query
//vector<dist_vec>* Lhashtables::NN_search(vec* nvector,int N);
//vector<vector<dist_vec>*>* Lhashtables:: find_k_nearest(vec* qvectors,int N,int queries_no_of_vectors);
//vector<dist_vec>* Lhashtables::LRadius_search(vec* nvector,int R);
//vector<vector<dist_vec>*>* Lhashtables:: find_in_LRadius(vec* qvectors,int R,int queries_no_of_vectors);

#endif