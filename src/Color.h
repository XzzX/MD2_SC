#ifndef COLOR_H
#define COLOR_H

/**
Class to store a rgb-color value. red, green and blue values are between 0.0 and 1.0.
**/
class Color{
	public:
		///red color value
		double mR;
		///green color value
		double mG;
		///blue color value
		double mB;
		Color(const double r, const double g, const double b);
};

///global color constant red
const Color gRed   = Color(1.0f, 0.0f, 0.0f);
///global color constant green
const Color gGreen = Color(0.0f, 1.0f, 0.0f);
///global color constant blue
const Color gBlue  = Color(0.0f, 0.0f, 1.0f);
///global color constant orange
const Color gOrange = Color(1.0f, 0.5f, 0.0f);
///global color constant yellow
const Color gYellow = Color(1.0f, 1.0f, 0.0f);
///global color constant magenta
const Color gMagenta = Color(1.0f, 0.0f, 1.0f);
///global color constant white
const Color gWhite = Color(1.0f, 1.0f, 1.0f);
///global color constant black
const Color gBlack = Color(0.0f, 0.0f, 0.0f);

#endif COLOR_H