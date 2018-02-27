#include <stdio.h>
#include <iostream>

#include "MQuaternions.h"
using namespace std;

int main()
{
	MQuaternions q1(1, 0, 1, 0);
	MQuaternions q2(0, 1, 0, 0);

	MQuaternions result;
	result = q1.AxisRotation(q2, 45);
	cout << "(" << result._x << "," << result._y << "," << result._z << "," << result._w << ")";
	cin.get();
	return 0;
}