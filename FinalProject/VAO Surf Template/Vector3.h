//
// Created by ZhiquanWang on 2018/7/4.
//

#ifndef MATRIXOPERATION_CPP_Vector3_H
#define MATRIXOPERATION_CPP_Vector3_H


#include <iostream>
#include <cmath>

class Vector3 {
private:
	float vec3[3]{};
public:
	Vector3();

	Vector3(const Vector3&);

	Vector3(float, float, float);

	Vector3(glm::vec3);

	inline void set_vector(float, float, float);

	inline float x() const;

	inline float y() const;

	inline float z() const;

	inline float r() const;

	inline float g() const;

	inline float b() const;

	inline void set_x(float _x);

	inline void set_y(float _y);

	inline void set_z(float _z);

	inline const Vector3& operator+() const;

	inline Vector3 operator-();

	inline float operator[](int) const;

	inline float& operator[](int);

	inline Vector3& operator+=(const Vector3&);

	inline Vector3& operator-=(const Vector3&);

	inline Vector3& operator*=(const Vector3&);

	inline Vector3& operator*=(const float&);

	inline Vector3& operator/=(const Vector3&);

	inline Vector3& operator/=(const float&);

	inline bool operator==(const Vector3&);

	inline bool operator!=(const Vector3&);

	inline float length() const;

	inline float distance() const;

	inline float squared_distance() const;

	inline Vector3 normalize() const;
	inline glm::vec3 to_glm_vec3() const;
};

Vector3::Vector3() = default;

Vector3::Vector3(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		this->vec3[i] = _v[i];
	}
}

Vector3::Vector3(float _n0, float _n1, float _n2) {
	this->vec3[0] = _n0;
	this->vec3[1] = _n1;
	this->vec3[2] = _n2;
}

Vector3::Vector3(glm::vec3 v) {
	this->vec3[0] = v[0];
	this->vec3[1] = v[1];
	this->vec3[2] = v[2];
}

inline void Vector3::set_vector(float _p0, float _p1, float _p2) {
	vec3[0] = _p0;
	vec3[1] = _p1;
	vec3[2] = _p2;
}

inline float Vector3::x() const {
	return this->vec3[0];
}

inline float Vector3::y() const {
	return this->vec3[1];
}

inline float Vector3::z() const {
	return this->vec3[2];
}

inline float Vector3::r() const {
	return this->vec3[0];
}

inline float Vector3::g() const {
	return this->vec3[1];
}

inline float Vector3::b() const {
	return this->vec3[2];
}

inline void Vector3::set_x(float _x) {
	this->vec3[0] = _x;
}

inline void Vector3::set_y(float _y) {
	this->vec3[1] = _y;
}

inline void Vector3::set_z(float _z) {
	this->vec3[2] = _z;
}


inline const Vector3& Vector3::operator+() const {
	return *this;
}

inline Vector3 Vector3::operator-() {
	return Vector3(-1 * this->vec3[0], -1 * this->vec3[1], -1 * this->vec3[2]);
}

inline float Vector3::operator[](int _i) const {
	return this->vec3[_i];
}

inline float& Vector3::operator[](int _i) {
	return this->vec3[_i];
}

inline Vector3& Vector3::operator+=(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		this->vec3[i] += _v[i];
	}
	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		this->vec3[i] -= _v[i];
	}
	return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		this->vec3[i] *= _v[i];
	}
	return *this;
}

inline Vector3& Vector3::operator*=(const float& _d) {
	for (float& data : this->vec3) {
		data *= _d;
	}
	return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		this->vec3[i] /= _v[i];
	}
	return *this;
}

inline Vector3& Vector3::operator/=(const float& _d) {
	for (float& data : this->vec3) {
		data /= _d;
	}
	return *this;
}

inline float Vector3::length() const {
	return sqrt(this->vec3[0] * this->vec3[0] + this->vec3[1] * this->vec3[1] + this->vec3[2] * this->vec3[2]);
}

inline float Vector3::distance() const {
	return sqrt(this->vec3[0] * this->vec3[0] + this->vec3[1] * this->vec3[1] + this->vec3[2] * this->vec3[2]);
}

inline float Vector3::squared_distance() const {
	return this->vec3[0] * this->vec3[0] + this->vec3[1] * this->vec3[1] + this->vec3[2] * this->vec3[2];
}

inline Vector3 Vector3::normalize() const {
	float tmp_len = this->length();
	if (tmp_len == 0.0) {
		return Vector3();
	}
	Vector3 tmp_v = *this;
	for (float& data : tmp_v.vec3) {
		data /= tmp_len;
	}
	return tmp_v;
}

inline bool Vector3::operator==(const Vector3& _v) {
	for (int i = 0; i < 3; ++i) {
		if (this->vec3[i] != _v[i]) {
			return false;
		}
	}
	return true;
}

inline bool Vector3::operator!=(const Vector3& _v) {
	return !(*this == _v);
}

inline std::istream& operator>>(std::istream& _is, Vector3& _v) {
	_is >> _v[0] >> _v[1] >> _v[2];
	return _is;
}

inline std::ostream& operator<<(std::ostream& _os, const Vector3& _v) {
	_os << "(" << _v[0] << "," << _v[1] << "," << _v[2] << ")";
	return _os;
}

inline float dot(const Vector3& _v0, const Vector3& _v1) {
	float tmp_result = 0;
	for (int i = 0; i < 3; ++i) {
		tmp_result += _v0[i] * _v1[i];
	}
	return tmp_result;
}

inline Vector3 cross(const Vector3& _v0, const Vector3& _v1) {
	return { _v0[1] * _v1[2] - _v0[2] * _v1[1], _v0[2] * _v1[0] - _v0[0] * _v1[2], _v0[0] * _v1[1] - _v0[1] * _v1[0] };
}

inline Vector3 operator+(const Vector3& _v0, const Vector3& _v1) {
	return { _v0[0] + _v1[0], _v0[1] + _v1[1], _v0[2] + _v1[2] };
}

inline Vector3 operator-(const Vector3& _v0, const Vector3& _v1) {
	return { _v0[0] - _v1[0], _v0[1] - _v1[1], _v0[2] - _v1[2] };
}

inline Vector3 operator*(const Vector3& _v0, const Vector3& _v1) {
	return { _v0[0] * _v1[0], _v0[1] * _v1[1], _v0[2] * _v1[2] };
}

inline Vector3 operator*(const Vector3& _v0, const float& _d) {
	return { _v0[0] * _d, _v0[1] * _d, _v0[2] * _d };
}

inline Vector3 operator*(const float& _d, const Vector3& _v0) {
	return { _v0[0] * _d, _v0[1] * _d, _v0[2] * _d };
}

inline Vector3 operator/(const Vector3& _v0, const Vector3& _v1) {
	return { _v0[0] / _v1[0], _v0[1] / _v1[1], _v0[2] / _v1[2] };
}

inline Vector3 operator/(const Vector3& _v0, const float& _d) {
	return { _v0[0] / _d, _v0[1] / _d, _v0[2] / _d };
}

inline glm::vec3 Vector3::to_glm_vec3() const {
	return glm::vec3(this->vec3[0], this->vec3[1], this->vec3[2]);
}

#endif //MATRIXOPERATION_CPP_Vector3_H
