#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include "rubik_cube.hpp"

class UI {
private:
	RubicCube rubic_cube;

	void Save();
	void Load();
	void Rotate();
	void Random();
	void Show();
	void Solve();

	void Help();
public:
	void Runtime();

};

#endif