#ifndef COLOR_H
#define COLOR_H

class Color{
	public:
		double mR;
		double mG;
		double mB;
		Color(const double r, const double g, const double b);
};

const Color gRed   = Color(1.0f, 0.0f, 0.0f);
const Color gGreen = Color(0.0f, 1.0f, 0.0f);
const Color gBlue  = Color(0.0f, 0.0f, 1.0f);
const Color gWhite = Color(1.0f, 1.0f, 1.0f);

#endif COLOR_H