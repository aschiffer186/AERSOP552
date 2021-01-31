#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>

static const double PI = 3.14159;
static const double DEG_TO_RAD = PI/180;
static const double RAD_TO_DEG = 180/PI;
static const double R = 3440;

void fly(double latA, double longA, double latB, double longB)
{
    //Validate Input
    if (abs(latA) > 90) {
        std::cerr << "Invalid value for latA" << std::endl;
        exit(EXIT_FAILURE);
    } 

    if (abs(latB) > 90) {
        std::cerr << "Invalid value for latB" << std::endl;
        exit(EXIT_FAILURE);
    } 

    if(abs(longA) > 180) {
        std::cerr << "Invalid value for longA" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(abs(longB) > 180) {
        std::cerr << "Invalid value for longB" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Convert from deg to rad
    double latA_rad = latA*DEG_TO_RAD;
    double longA_rad = longA*DEG_TO_RAD;
    double latB_rad = latB*DEG_TO_RAD;
    double longB_rad = longB*DEG_TO_RAD;

    //Compute heading and distance
    double a = pow(sin(0.5*(latB_rad - latA_rad)),2) + cos(latA_rad)*cos(latB_rad)* pow(sin(0.5*(longB_rad - longA_rad)),2);
    double d = 2*R*atan2(sqrt(a), sqrt(1-a));
    double theta = atan2(sin(longB_rad - longA_rad)*cos(latB_rad), 
                         cos(latA_rad)*sin(latB_rad)-sin(latA_rad)*cos(latB_rad)*cos(longB_rad - longA_rad));
    std::cout << std::setprecision(2) << std::fixed << "Fly heading " << theta*RAD_TO_DEG << " degrees for " << d << " nautical miles." << std::endl; 
}

int main(int argc, char** argv)
{
    fly(42.278046, -83.738220, 42.3601, -71.0589);
    return 0;
}