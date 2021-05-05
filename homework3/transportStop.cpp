#include "transportStop.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <cmath>

double myStod(const std::string& valueAsString) {
	std::istringstream totalSString(valueAsString);
	double valueAsDouble;
	totalSString >> valueAsDouble;
	return valueAsDouble;
}
	
//fixed u can use default constructor

void transportStop::setStop(std::string num, std::string type, std::string loc, std::string rout, std::string coord) {
	stops.resize(stops.size() + 1);
	stops[stops.size() - 1].number = std::stoi(num);
	stops[stops.size() - 1].type_of_vehicle = type;
	
	bool notInDefferent = true;
	if (differentTransport.empty())
		differentTransport.push_back(type);
	else {
		for (auto& it : differentTransport)
			if (it == type)
				notInDefferent = false;
		if (notInDefferent)
			differentTransport.push_back(type);
	}

	std::string str = "";
	for (auto& iLoc : loc) {
		if (iLoc == ',' || (iLoc == ' ' && *(&iLoc - 1) == ','))
			continue;
		str += iLoc;
		if (*(&iLoc + 1) == '\0' || *(&iLoc + 1) == ',') {
			stops[stops.size() - 1].location.push_back(str);
			str = "";
		}
	}

	//fixed range based for
	for (auto& iRout : rout) {
		if (iRout == ',' || iRout == '.')
			continue;
		str += iRout;
		if (*(&iRout + 1) == '\0' || *(&iRout + 1) == ',' || *(&iRout + 1) == '.') {
			stops[stops.size() - 1].routes.push_back(str);
			str = "";
		}
	}

	for (auto& iCoord : coord) {
		if (iCoord == ',')
			continue;
		str += iCoord;
		if (*(&iCoord + 1) == '\0') {
			stops[stops.size() - 1].coordinates[1] = myStod(str);
			str = "";
		}
		else if (*(&iCoord + 1) == ',') {
			stops[stops.size() - 1].coordinates[0] = myStod(str);
			str = "";
		}
	}
}

void transportStop::leastStopRoute(std::ofstream& fout) {
	std::vector<std::map<std::string, int>> route;
	route.resize(differentTransport.size());
	std::vector<std::string> answer;
	answer.resize(differentTransport.size());

	int thisType = -1;
	for (auto itStop = stops.begin(); itStop != stops.end(); ++itStop) {
		for (int i = 0; i < int(differentTransport.size()); ++i) {
			if (itStop->type_of_vehicle == differentTransport[i])
				thisType = i;
		}
		//fixed range-based for (for (auto& : routes))
		for (auto& iRout : itStop->routes){
			if (thisType == -1)
				exit(1);
			if (route[thisType].count(iRout) == 0)
				route[thisType][iRout] = 1;
			else
				route[thisType][iRout]++;

			if (answer[thisType] == "" || route[thisType][answer[thisType]] < route[thisType][iRout])
				answer[thisType] = iRout;
		}
	}

	fout << "1) leastStopRoute:\n";
	for (int i = 0; i < int(answer.size()); ++i) {
		fout << differentTransport[i] << ": " << answer[i] << '\n';
	}
}

void transportStop::longestRoute(std::ofstream& fout) {
	std::vector<std::map<std::string, dataForLongest>> route;
	route.resize(differentTransport.size());
	std::vector<std::string> answer;
	answer.resize(differentTransport.size());

	int thisType = -1;
	for (auto itStop = stops.begin(); itStop != stops.end(); ++itStop) {
		for (int i = 0; i < int(differentTransport.size()); ++i) {
			if (itStop->type_of_vehicle == differentTransport[i])
				thisType = i;
		}
		//fixed range-based for
		for (auto& iRoute : itStop->routes) {
			if (thisType == -1)
				exit(1);
			if (route[thisType].count(iRoute) == 0) {
				route[thisType][iRoute].prevCoordinates[0] = itStop->coordinates[0];
				route[thisType][iRoute].prevCoordinates[1] = itStop->coordinates[1];
				route[thisType][iRoute].length = 0;
			}
			else {
				//fixed find another ones range-based fors yourself
				double x = route[thisType][iRoute].prevCoordinates[0];
				double y = route[thisType][iRoute].prevCoordinates[1];
				route[thisType][iRoute].length += sqrt(pow((x - itStop->coordinates[0]), 2) + pow((y - itStop->coordinates[1]), 2));
				route[thisType][iRoute].prevCoordinates[0] = itStop->coordinates[0];
				route[thisType][iRoute].prevCoordinates[1] = itStop->coordinates[1];
			}

			if (answer[thisType] == "" || route[thisType][answer[thisType]].length < route[thisType][iRoute].length)
				answer[thisType] = iRoute;
		}
	}

	fout << "2) longestRoute:\n";
	for (int i = 0; i < int(answer.size()); ++i) {
		fout << differentTransport[i] << ": " << answer[i] << '\n';
	}
}

void transportStop::streetMostStops(std::ofstream& fout) {
	std::map<std::string, int> stopsOnStreet;
	
	std::string streetMaxStops;
	for (auto itStop = stops.begin(); itStop != stops.end(); ++itStop) {

		//for (auto itLoc = itStop->location.begin(); itLoc != itStop->location.end(); ++itLoc) {
		//	if (stopsOnStreet.count(*itLoc) == 0)
		//		stopsOnStreet[*itLoc] = 1;
		//	else
		//		stopsOnStreet[*itLoc]++;
		//	if (streetMaxStops == "" || stopsOnStreet[*itLoc] > stopsOnStreet[streetMaxStops])
		//		streetMaxStops = *itLoc;
		//}

		for (auto& itLoc : itStop->location) {
			if (stopsOnStreet.count(itLoc) == 0)
				stopsOnStreet[itLoc] = 1;
			else
				stopsOnStreet[itLoc]++;
			if (streetMaxStops == "" || stopsOnStreet[itLoc] > stopsOnStreet[streetMaxStops])
				streetMaxStops = itLoc;
		}
	}

	fout << "3) streetMostStops:\n" << streetMaxStops << '\n';
}