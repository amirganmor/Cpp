#include "Color.h"



Color::Color()
{
	//set values for default constructor
	setType("null");
	setColour(Scalar(0, 0, 0));

}

Color::Color(string name) {

	setType(name);

	if (name == "green") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(30, 59, 50));
		setHSVmax(Scalar(79, 252, 171));

		//BGR value for Green:
		setColour(Scalar(0, 255, 0));

		//set name
		setType("green");
	}
	if (name == "red") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0, 100, 100));
		setHSVmax(Scalar(10, 255, 255));

		//BGR value for Red:
		setColour(Scalar(0, 0, 255));

		//set name
		setType("red");
	}
	if (name == "yellow") {

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(20, 100, 100));
		setHSVmax(Scalar(30, 255, 255));



		//BGR value for Yellow:
		setColour(Scalar(0, 255, 255));

		//set name
		setType("Yellow");

	}



}

Color::~Color(void)
{
}

int Color::getXPos() {

	return Color::xPos;

}

void Color::setXPos(int x) {

	Color::xPos = x;

}

int Color::getYPos() {

	return Color::yPos;

}

void Color::setYPos(int y) {

	Color::yPos = y;

}

Scalar Color::getHSVmin() {

	return Color::HSVmin;

}
Scalar Color::getHSVmax() {

	return Color::HSVmax;
}

void Color::setHSVmin(Scalar min) {

	Color::HSVmin = min;
}


void Color::setHSVmax(Scalar max) {

	Color::HSVmax = max;
}
