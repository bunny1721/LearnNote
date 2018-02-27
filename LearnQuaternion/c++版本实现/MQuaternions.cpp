#include "MQuaternions.h"
#include <math.h>
MQuaternions::MQuaternions()
{
}

MQuaternions::~MQuaternions()
{
}
MQuaternions::MQuaternions(float x, float y, float z, float w)
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
	unitary();
}
MQuaternions MQuaternions::AxisRotation(MQuaternions _axis, float _angle)
{
	MQuaternions result , q, p ,qN;
	float radian = (_angle * 3.1415926f) / 180.f;
	q._x = _axis._nx * sinf(radian);
	q._y = _axis._ny * sinf(radian);
	q._z = _axis._nz * sinf(radian);
	q._w = cosf(radian);

	qN._x = -q._x;
	qN._y = -q._y;
	qN._z = -q._z;
	qN._w = q._w;

	p._x = _x;
	p._y = _y;
	p._z = _z;
	p._w = _w;

	result = q.Ride(p).Ride(qN);
	return result;
	
}

MQuaternions MQuaternions::Ride(MQuaternions _target)
{
	float x1 = _y * _target._z - _z * _target._y;
	float y1 = _z * _target._x - _x * _target._z;
	float z1 = _x * _target._y - _y * _target._x;
	
	float x2 = _w * _target._x;
	float y2 = _w * _target._y;
	float z2 = _w * _target._z;

	float x3 = _target._w * _x;
	float y3 = _target._w * _y;
	float z3 = _target._w * _z;
	
	float w1 = _w * _target._w;
	float w2 = _x * _target._x + _y * _target._y + _z * _target._z;

	MQuaternions _result;
	_result._x = x1 + x2 + x3;
	_result._y = y1 + y2 + y3;
	_result._z = z1 + z2 + z3;
	_result._w = w1 - w2;
	return _result;
}

void MQuaternions::unitary()
{
	float divisor = sqrtf(_x * _x + _y * _y + _z * _z);
	_nx = _x / divisor;
	_ny = _y / divisor;
	_nz = _z / divisor;
	_nw = _w;
}
