//
// Created by zhiquan on 3/5/19.
//

#ifndef SPH_FLUID_SOLVER_PARTICLE_H
#define SPH_FLUID_SOLVER_PARTICLE_H
#include "Vector3.h"


class Particle {
private:
    size_t index{};
    Vector3 position;
    Vector3 velocity;
    Vector3 pre_velocity;
    Vector3 acceleration;
    float density{};
    float pressure{};
    Vector3 gravity_acceleration;
    Vector3 pressure_acceleration;
    Vector3 viscosity_acceleration;
    Vector3 surface_tension_acceleration;
	Vector3 external_acceleration;
	glm::vec3 color;
public:
    Particle() = default;

    explicit Particle(size_t index) : index(index) {
    }

    size_t get_index() const {
        return index;
    }

    void set_index(size_t index) {
        Particle::index = index;
    }

	void set_color(glm::vec3 c) {
		Particle::color = c;
	}
	glm::vec3 get_color() {
		return Particle::color;
	}
    const Vector3 &get_position() const {
        return position;
    }

    void set_position(const Vector3 &position) {
        Particle::position = position;
    }

    const Vector3 &get_velocity() const {
        return velocity;
    }

    void set_velocity(const Vector3 &velocity) {
        Particle::velocity = velocity;
    }

    const Vector3 &get_pre_velocity() const {
        return pre_velocity;
    }

    void set_pre_velocity(const Vector3 &velocity) {
        Particle::pre_velocity = pre_velocity;
    }


    const Vector3 &get_acceleration() const {
        return acceleration;
    }

    void set_acceleration(const Vector3 &acceleration) {
        Particle::acceleration = acceleration;
    }
	void add_acceleration(const Vector3& acceleration) {
		Particle::external_acceleration = acceleration*1000;

	}

	const Vector3& get_external_acceleration() const {
		//if (external_acceleration.x() != 0) {
		//	std::cout << external_acceleration.x() << " " << external_acceleration.y() << std::endl;

		//}

		return external_acceleration;
	}
    float get_density() const {
        return density;
    }

    void set_density(float density) {
        Particle::density = density;
    }

    float get_pressure() const {
        return pressure;
    }

    void set_pressure(float pressure) {
        Particle::pressure = pressure;
    }

    const Vector3 &get_gravity_acceleration() const {
        return gravity_acceleration;
    }

    void set_gravity_acceleration(const Vector3 &gravity_acceleration) {
        Particle::gravity_acceleration = gravity_acceleration;
    }

    const Vector3 &get_pressure_acceleration() const {
        return pressure_acceleration;
    }

    void set_pressure_acceleration(const Vector3 &pressure_acceleration) {
        Particle::pressure_acceleration = pressure_acceleration;
    }

    const Vector3 &get_viscosity_acceleration() const {
        return viscosity_acceleration;
    }

    void set_viscosity_acceleration(const Vector3 &viscosity_acceleration) {
        Particle::viscosity_acceleration = viscosity_acceleration;
    }

    const Vector3 &get_surface_tension_acceleration() const {
        return surface_tension_acceleration;
    }

    void set_surface_tension_acceleration(const Vector3 &surface_tension_acceleration) {
        Particle::surface_tension_acceleration = surface_tension_acceleration;
    }

};


#endif //SPH_FLUID_SOLVER_PARTICLE_H
