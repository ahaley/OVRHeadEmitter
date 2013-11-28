#include <cmath>

class PrecisionTest
{
public:
	PrecisionTest(int precision)
	{
		_precision = pow(10.0f, precision);
	}

	bool Test(float x, float y, float z)
	{
		int newX = x * _precision;
		int newY = y * _precision;
		int newZ = z * _precision;

		bool different = newX != _x || newY != _y || newZ != _z;
		
		if (different) {
			_x = newX;
			_y = newY;
			_z = newZ;
		}
		return different;
	}

private:
	int _x;
	int _y;
	int _z;
	int _precision;
};