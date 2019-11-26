

//
// Created by zhiquan on 3/6/19.
//

#ifndef SPH_FLUID_SOLVER_FLUIDSOLVER_H
#define SPH_FLUID_SOLVER_FLUIDSOLVER_H

#include <cmath>
#include <chrono>

#include "FluidParameter.h"
#include "FluidDatabase.h"
#include "RestrictionBox.h"
#include "Vector3.h"

class FluidSolver {
private:
	FluidParameter fluid_parameter{};
	RestrictionBox restriction_box{};
	vector<Particle> realtime_particle_list{};
	float interval;
public:

	FluidSolver(const FluidParameter& _fluid_parameter,
		const RestrictionBox& _restriction_box,
		FluidDatabase& _fluid_database)
		: fluid_parameter(_fluid_parameter),
		restriction_box(_restriction_box) {
	}
	FluidSolver(FluidParameter fp, RestrictionBox rb, float i)
		:fluid_parameter(fp),
		restriction_box(rb),
		interval(i){

	}
	void initialize_particles(const Vector3 & _vertex0, const Vector3 & _vertex1, const double_t & _interval) {
		size_t cur_num = 0;
		for (double_t x = _vertex0[0]; x <= _vertex1[0]; x += _interval) {
			for (double_t z = _vertex0[2]; z <= _vertex1[2]; z += _interval) {
				for (double_t y = _vertex0[1]; y <= _vertex1[1]; y += _interval) {
					++cur_num;
					if (cur_num > fluid_parameter.get_particle_num()) {
						cout << "Particles Initialized : " << realtime_particle_list.size() << endl << endl;
						return;
					}
					float x_offset = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					float z_offset = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
					Particle tmp_particle(cur_num);
					tmp_particle.set_position(Vector3(x, y, z));
					realtime_particle_list.emplace_back(tmp_particle);
				}
			}
		}
		Vector3 tmp_vec = realtime_particle_list[0].get_position();
		realtime_particle_list[0].set_position(Vector3(tmp_vec.x() + 0.5, tmp_vec.y(), tmp_vec.z()));
		cout << "Particles Initialized : " << realtime_particle_list.size() << endl << endl;
	}

	void simulate_particles() {		
		compute_particle_acceleration();
		update_position();
	}

	vector<float> get_pos() {
		vector<float> pos;
		for (int i = 0; i < realtime_particle_list.size(); ++i) {
			pos.push_back(realtime_particle_list[i].get_position().x());
			pos.push_back(realtime_particle_list[i].get_position().y());
			pos.push_back(realtime_particle_list[i].get_position().z());
		}
		return pos;
	}

	void set_time_interval(float i) {
		this->interval = i;
	}
	void set_params(FluidParameter fp) {
		this->fluid_parameter = fp;
	}

	void set_restriction_box(RestrictionBox rb) {
		this->restriction_box = rb;
	}
private:
	void compute_particle_acceleration() {
		compute_density();
		compute_pressure();
		compute_pressure_acceleration();
		compute_viscosity_acceleration();
		compute_surface_tension_acceleration();
		for (auto& p : realtime_particle_list) {
			p.set_acceleration(p.get_pressure_acceleration() +
				p.get_viscosity_acceleration() +
				p.get_surface_tension_acceleration() +
				fluid_parameter.get_gravity_acceleration_coefficient() * Vector3(0, -1, 0));
		}
	}

	void restrict_particles() {
		for (auto& p : realtime_particle_list) {
			restriction_box.restrict_particle(p);
		}
	}

	void update_position() {
		for (auto& p : realtime_particle_list) {
			//            auto tmp_vec = p.get_velocity() + p.get_acceleration() * fluid_database.get_frame_interval();
			//            p.set_velocity(tmp_vec);
			//            p.set_pre_velocity((tmp_vec+p.get_pre_velocity())*0.5);
			p.set_velocity(p.get_velocity() + p.get_acceleration() * interval);
			restrict_particles();
			p.set_position(p.get_position() + p.get_velocity() * interval);
		/*	for (int i = 0; i < 3; ++i) {
				std::cout << p.get_position()[i] << std::endl;
			}std::cout << std::endl;*/
		}

	}

	void compute_density() {
		auto tmp_vector = realtime_particle_list;
		for (auto& p_i : realtime_particle_list) {
			double_t tmp_density = 0;
			for (auto& p_j : tmp_vector) {
				tmp_density += fluid_parameter.get_particle_mass() *
					compute_kernel_poly6(p_i.get_position() - p_j.get_position(),
						fluid_parameter.get_core_radius());
			}
			p_i.set_density(tmp_density);
		}
	}

	void compute_pressure() {
		for (auto& p_i : realtime_particle_list) {
			p_i.set_pressure(
				fluid_parameter.get_gas_constant() * (p_i.get_density() - fluid_parameter.get_rest_density()));
		}
	}

	void compute_pressure_acceleration() {
		auto tmp_vector = realtime_particle_list;
		for (auto& p_i : realtime_particle_list) {
			auto tmp_force = Vector3();
			for (auto& p_j : tmp_vector) {
				if (p_i.get_index() == p_j.get_index()) {
					continue;
				}
				tmp_force += (p_i.get_pressure() + p_j.get_pressure()) / p_j.get_density() *
					compute_kernel_spiky_hamiltonian(p_i.get_position() - p_j.get_position(),
						fluid_parameter.get_core_radius());

			}
			p_i.set_pressure_acceleration(
				-1 * fluid_parameter.get_particle_mass() / (p_i.get_density() * 2) * tmp_force);
			//            cout << p_i.get_density() << " : " << p_i.get_pressure() << " : " << p_i.get_position()<< endl;
			//            cout << p_i.get_pressure_acceleration() << endl;
		}
	}

	void compute_viscosity_acceleration() {
		auto tmp_vector = realtime_particle_list;
		for (auto& p_i : realtime_particle_list) {
			Vector3 tmp_force = Vector3();
			for (auto& p_j : tmp_vector) {
				if (p_i.get_index() == p_j.get_index()) {
					continue;
				}
				tmp_force += (p_j.get_velocity() - p_i.get_velocity()) / p_j.get_density() *
					compute_kernel_viscosity_laplacian(
						p_i.get_position() -
						p_j.get_position(),
						fluid_parameter.get_core_radius());

				p_i.set_viscosity_acceleration(
					fluid_parameter.get_viscosity_coefficient() * fluid_parameter.get_particle_mass() /
					p_i.get_density() * tmp_force);
			}
			//            cout<< endl;
			//            cout << p_i.set_viscosity_acceleration() << endl;
			//            cout<< endl;
		}
	}

	void compute_surface_tension_acceleration() {
		auto tmp_vector = realtime_particle_list;
		for (auto& p_i : realtime_particle_list) {
			auto tmp_force = Vector3();
			auto tmp_gradient = Vector3();
			auto tmp_curvature = 0.0;
			for (auto& p_j : tmp_vector) {
				if (p_i.get_index() == p_j.get_index()) {
					continue;
				}
				if ((p_i.get_position() - p_j.get_position()).length() <= fluid_parameter.get_core_radius()) {
					tmp_gradient += (1.0 / p_j.get_density()) *
						compute_kernel_poly6_hamiltonian(p_i.get_position() - p_j.get_position(),
							fluid_parameter.get_core_radius());
					tmp_curvature += (1.0 / p_j.get_density()) *
						compute_kernel_poly6_laplacian(p_i.get_position() - p_j.get_position(),
							fluid_parameter.get_core_radius());
				}
			}
			tmp_gradient *= -1.0 * fluid_parameter.get_particle_mass();
			tmp_curvature *= fluid_parameter.get_particle_mass();
			p_i.set_surface_tension_acceleration(
				-1.0 * fluid_parameter.get_tension_coefficient() / p_i.get_density() * tmp_curvature *
				tmp_gradient.normalize());
		}
	}

	const double_t compute_kernel_poly6(const Vector3& _offset_vec, const double_t& _core_radius) {
		double_t tmp_dis = _offset_vec.length();
		if (tmp_dis > _core_radius) {
			return 0.0;
		}
		return 315.0 / (65.0 * glm::pi<float>() * pow(_core_radius, 9)) *
			pow(pow(_core_radius, 2) - _offset_vec.squared_distance(), 3);
	}

	const Vector3 compute_kernel_poly6_hamiltonian(const Vector3& _offset_vec, const double_t& _core_radius) {
		double_t tmp_dis = _offset_vec.length();
		if (tmp_dis > _core_radius) {
			return Vector3();
		}
		return -945.0 / (32.0 * glm::pi<float>() * pow(_core_radius, 9)) *
			pow((pow(_core_radius, 2) - _offset_vec.squared_distance()), 2) *
			_offset_vec;
	}

	const double_t compute_kernel_poly6_laplacian(const Vector3& _offset_vec, const double_t& _core_radius) {
		double_t tmp_dis = _offset_vec.length();
		double_t squared_distance = _offset_vec.squared_distance();
		double_t squared_radius = pow(_core_radius, 2);
		if (tmp_dis > _core_radius) {
			return 0.0;
		}
		return 945.0 / (8.0 * glm::pi<float>() * pow(_core_radius, 9)) * pow(squared_radius - squared_distance, 2) *
			(squared_distance - (3.0 / 4.0) * (squared_distance - squared_radius));
	}

	const Vector3 compute_kernel_spiky_hamiltonian(const Vector3& _offset_vec, const double_t& _core_radius) {
		double_t tmp_dis = _offset_vec.length();
		if (tmp_dis > _core_radius) {
			return Vector3();
		}
		return -45.0 / (glm::pi<float>() * pow(_core_radius, 6)) * pow(_core_radius - tmp_dis, 2) * _offset_vec.normalize();
	}

	const double_t compute_kernel_viscosity_laplacian(const Vector3& _offset_vec, const double_t& _core_radius) {
		double_t tmp_dis = _offset_vec.length();
		if (tmp_dis > _core_radius) {
			return 0.0;
		}
		return 45.0 / (glm::pi<float>() * pow(_core_radius, 6)) * (_core_radius - tmp_dis);
	}

	const vector<Particle>& get_realtime_particle_list() const {
		return realtime_particle_list;
	}

	void set_realtime_particle_list(const vector<Particle>& realtime_particle_list) {
		FluidSolver::realtime_particle_list = realtime_particle_list;
	}

	

};


#endif //SPH_FLUID_SOLVER_FLUIDSOLVER_H
