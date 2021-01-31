#include <fstream>
#include <iostream>
#include <string> 
#include <iomanip>

int preflight()
{
    std::string filename;
    std::cout << "Enter a filename: ";
    std::cin >> filename;
    std::ifstream fin(filename);
    if (!fin.is_open())
        return -1;
    //Read in weights
    double pilot_weight;
    fin >> pilot_weight;
    if (pilot_weight <= 0)
    {
        std::cout << "Pilot weight must be positive" << std::endl;
        return -1;
    }
    double pilot_lever = 37;
    double front_pax_weight;
    fin >> front_pax_weight;
    if (front_pax_weight < 0)
    {
        std::cout << "Front pax weight must be non-negative" << std::endl;
        return -1;
    }
    double front_pax_lever = 37;
    double rear_pax_1;
    fin >> rear_pax_1;
    if (rear_pax_1 < 0)
    {
        std::cout << "Rear pax weight must be non-negative" << std::endl;
        return -1;
    }
    double rear_pax_2;
    fin >> rear_pax_2;
    if (rear_pax_2 < 0)
    {
        std::cout << "Rear pax weight must be non-negative" << std::endl;
        return -1;
    }
    double rear_pax_lever = 73;
    double cargo_1;
    fin >> cargo_1;
    if (cargo_1 < 0)
    {
        std::cout << "Cargo weight must be non-negative" << std::endl;
        return -1;
    }
    double cargo_1_lever = 95; //Is this right?
    double cargo_2;
    fin >> cargo_2;
    if (cargo_2 < 0)
    {
        std::cout << "Cargo weight must be non-negative" << std::endl;
        return -1;
    }
    double cargo_2_lever = 123; //Is this right?
    double fuel;
    fin >> fuel;
    if (fuel <= 0)
    {
        std::cout << "Fuel amount must be positive" << std::endl;
        return -1;
    }
    fuel *= 6;
    double fuel_lever = 48;
    //Constants
    double airframe_weight = 1431;
    double airframe_lever = 36.21;
    //Compute moments and weight
    //Weight
    double total_weight = pilot_weight + front_pax_weight + rear_pax_1 + rear_pax_2 + cargo_1 + cargo_2 + fuel + airframe_weight;
    //Moment of components
    double pilot_moment = pilot_weight*pilot_lever;
    double front_pax_moment = front_pax_weight*front_pax_lever;
    double rear_pax_moment = (rear_pax_1 + rear_pax_2)*rear_pax_lever;
    double cargo_1_moment = cargo_1*cargo_1_lever;
    double cargo_2_moment = cargo_2*cargo_2_lever;
    double fuel_moment = fuel*fuel_lever;
    double airframe_moment = airframe_lever*airframe_weight;
    //Total moment 
    double total_moment = pilot_moment + front_pax_moment + rear_pax_moment + cargo_1_moment + cargo_2_moment + fuel_moment + airframe_moment;
    double cg = total_moment/total_weight;
    //Print aircraft properties
    std::cout << std::setprecision(2) << std::fixed;
    std::cout << "Aircraft Weight: " << total_weight << ", Aircraft Moment: " << total_moment << ", Aircraft CG: " << cg << std::endl;
    //Check if properties are ok 
    if (cg < 35)
    {
        //CG out of balance
    }
    return 0;
}

int main(int argc, char** argv)
{
    return 0;
}