#include <string>
#include <vector>
#include <fstream>

#ifndef TRANSPORTSTOP_H
#define TRANSPORTSTOP_H

class transportStop {
private:
	struct data {
		int number;
		std::string type_of_vehicle;
		std::vector<std::string> location;
		std::vector<std::string> routes;
		double coordinates[2];
	};
	std::vector<data> stops;
	std::vector<std::string> differentTransport;

	struct dataForLongest {
		double length;
		double prevCoordinates[2];
	};

public:
	void setStop(std::string, std::string, std::string, std::string, std::string);

	void leastStopRoute(std::ofstream&);
	void longestRoute(std::ofstream&);
	void streetMostStops(std::ofstream&);
};

#endif