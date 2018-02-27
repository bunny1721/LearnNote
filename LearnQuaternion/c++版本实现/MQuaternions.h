#pragma once


class MQuaternions
{
public:
	
	MQuaternions();
	MQuaternions(float x, float y, float z, float w);
	~MQuaternions();
	MQuaternions AxisRotation(MQuaternions _axis, float _angle);
	MQuaternions Ride(MQuaternions _target);
protected:
	void unitary();
public:
	float _x;
	float _y;
	float _z;
	float _w;
	float _nx;
	float _ny;
	float _nz;
	float _nw;
};