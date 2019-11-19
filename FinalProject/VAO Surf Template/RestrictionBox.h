//
// Created by zhiquan on 3/6/19.
//

#ifndef SPH_FLUID_SOLVER_RESTRICTIONBOX_H
#define SPH_FLUID_SOLVER_RESTRICTIONBOX_H

#include <cmath>
#include "Vector3.h"
#include "Particle.h"
#include <GL/glew.h>

#define vec3 Vector3

class RestrictionBox {
private:
    double left_right_detection[2]{};
    double front_back_detection[2]{};
    double bottom_top_detection[2]{};
    double_t bound_damping_coefficient{};
public:
    RestrictionBox() = default;

    RestrictionBox(const vec3 &vertex0, const vec3 &vertex1, double_t _bound_damping_coefficient) :
            bound_damping_coefficient(_bound_damping_coefficient) {
//        cout << vertex0 << " " << vertex1 << endl;
        left_right_detection[0] = vertex0.x();
        left_right_detection[1] = vertex1.x();
        bottom_top_detection[0] = vertex0.y();
        bottom_top_detection[1] = vertex1.y();
        front_back_detection[0] = vertex0.z();
        front_back_detection[1] = vertex1.z();
    }

    void restrict_particle(Particle &in_particle) {
        vec3 tmp_pos = in_particle.get_position();
        const vec3 &tmp_vel = in_particle.get_velocity();
        vec3 new_vel = tmp_vel;
        if (tmp_pos.x() < left_right_detection[0]) {
            new_vel.set_x(-bound_damping_coefficient * tmp_vel.x());
            tmp_pos.set_x(left_right_detection[0]);
            //-= 2 * dot(normal_arr[0], tmp_vel) * normal_arr[0];
        }
        if (tmp_pos.x() > left_right_detection[1]) {
            new_vel.set_x(-bound_damping_coefficient * tmp_vel.x());
            tmp_pos.set_x(left_right_detection[1]);
            // new_vel -= 2 * dot(normal_arr[1], tmp_vel) * normal_arr[1];
        }
        if (tmp_pos.y() < bottom_top_detection[0]) {
            new_vel.set_y(-bound_damping_coefficient * tmp_vel.y());
            tmp_pos.set_y(bottom_top_detection[0]);
            //new_vel -= 2 * dot(normal_arr[2], tmp_vel) * normal_arr[2];
        }
        if (tmp_pos.y() > bottom_top_detection[1]) {
            new_vel.set_y(-bound_damping_coefficient * tmp_vel.y());
            tmp_pos.set_y(bottom_top_detection[1]);
            //new_vel -= 2 * dot(normal_arr[3], tmp_vel) * normal_arr[3];
        }
        if (tmp_pos.z() < front_back_detection[0]) {
            new_vel.set_z(-bound_damping_coefficient * tmp_vel.z());
            tmp_pos.set_z(front_back_detection[0]);
            //new_vel -= 2 * dot(normal_arr[4], tmp_vel) * normal_arr[4];
        }
        if (tmp_pos.z() > front_back_detection[1]) {
            new_vel.set_z(-bound_damping_coefficient * tmp_vel.z());
            tmp_pos.set_z(front_back_detection[1]);
            //new_vel -= 2 * dot(normal_arr[5], tmp_vel) * normal_arr[5];
        }
        in_particle.set_velocity(new_vel);
        in_particle.set_position(tmp_pos);
    }

	GLuint create_vbo() {
		float pos[] = {
			left_right_detection[0],bottom_top_detection[0],front_back_detection[0],
			left_right_detection[0],bottom_top_detection[0],front_back_detection[1],
			left_right_detection[0],bottom_top_detection[1],front_back_detection[0],
			left_right_detection[0],bottom_top_detection[1],front_back_detection[1],
			left_right_detection[1],bottom_top_detection[0],front_back_detection[0],
			left_right_detection[1],bottom_top_detection[0],front_back_detection[1],
			left_right_detection[1],bottom_top_detection[1],front_back_detection[0],
			left_right_detection[1],bottom_top_detection[1],front_back_detection[1]
		};
		GLuint vbo = -1;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
		return vbo;
	}

	GLuint create_ebo() {
		float index[] = {
			0,1,2,// left 0
			1,2,3,// left 1
			0,1,4,// bottom 0
			1,4,5,// bottom 1
			2,0,4,// front 0
			2,4,6,// front 1
			2,3,6,// top 0
			3,6,7,// top 1
			4,5,6,// right 0
			5,6,7,// right 1
			1,3,5,// back 0
			3,6,7 // back 1
		};
		GLuint ebo;
		glGenBuffers(1, &ebo); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
		return ebo;
	}

	GLuint create_vao() {
		GLuint vao = -1;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}
};


#endif //SPH_FLUID_SOLVER_RESTRICTIONBOX_H
