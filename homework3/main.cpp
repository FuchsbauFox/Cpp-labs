#include <iostream>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "transportStop.h"
#include <string>
#include <fstream>

int main() {

    std::ofstream fout("output.txt");

    pugi::xml_document main_file;
    pugi::xml_parse_result result = main_file.load_file("C:/Users/vovas/source/repos/OOP/homework3/homework3/data.xml");
    if (!result)
        return -1;
    auto Stops = transportStop();
    for (pugi::xml_node obj : main_file.child("dataset").children("transport_station")) {
        Stops.setStop(
            obj.child_value("number"),
            obj.child_value("type_of_vehicle"),
            obj.child_value("location"),
            obj.child_value("routes"),
            obj.child_value("coordinates"));
    }
    Stops.leastStopRoute(fout);
    Stops.longestRoute(fout);
    Stops.streetMostStops(fout);
    return 0;
}