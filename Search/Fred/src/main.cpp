#include <iostream>
#include <string>

#include "config.hpp"
#include "types.hpp"
#include "point.hpp"
#include "interval.hpp"
#include "curve.hpp"
#include "frechet.hpp"
#include "simplification.hpp"


using namespace std;
namespace fc = Frechet::Continuous;
int main(int argc, char *argv[])
{	
	Point temppoint1(5);
	Point temppoint2(5);
	cout<<temppoint1.dimensions();
	for(int i=0;i<5;i++){
		temppoint1.set(i,i+10);
	}
	for(int i=0; i<5;i++){
		temppoint2.set(i,i+10);
	}
	//string curvestring1="Test1";

	const Point constpoint1=temppoint1;
	Points tempPoints(5,constpoint1);
	const Points consttempPoints=tempPoints;
	const Curve curve1(consttempPoints,"Test1");

	const Point constpoint2=temppoint2;
	Points tempPoints2(5,constpoint2);
	const Points consttempPoints2=tempPoints2;
	const Curve curve2(consttempPoints2,"Test2");

	fc::Distance  dist;
	dist=fc::distance(curve1,curve2);
    cout<<"NOthing"<<endl;
    cout<<dist.value<<endl;
    return 0;
}
