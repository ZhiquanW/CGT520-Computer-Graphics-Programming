//
// Created by zhiquan on 3/6/19.
//

#ifndef SPH_FLUID_SOLVER_RESTRICTIONBOX_H
#define SPH_FLUID_SOLVER_RESTRICTIONBOX_H

#include <cmath>
#include "Particle.h"
#include <GL/glew.h>
#include "Vector3.h"

class RestrictionBox {
private:
    float left_right_detection[2]{};
    float front_back_detection[2]{};
    float bottom_top_detection[2]{};
    float bound_damping_coefficient{};
public:
    RestrictionBox() = default;

    RestrictionBox(const Vector3 &vertex0, const Vector3 &vertex1, float _bound_damping_coefficient) :
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
        Vector3 tmp_pos = in_particle.get_position();
        const Vector3 &tmp_vel = in_particle.get_velocity();
        Vector3 new_vel = tmp_vel;
        if (tmp_pos.x() < left_right_detection[0]) {
            new_vel.set_x(-bound_damping_coefficient * tmp_vel.x());
            tmp_pos.set_x(left_right_detection[0]+0.01f);
            //-= 2 * dot(normal_arr[0], tmp_vel) * normal_arr[0];
        }
        if (tmp_pos.x() > left_right_detection[1]) {
            new_vel.set_x(-bound_damping_coefficient * tmp_vel.x());
            tmp_pos.set_x(left_right_detection[1]-0.01f);
            // new_vel -= 2 * dot(normal_arr[1], tmp_vel) * normal_arr[1];
        }
        if (tmp_pos.y() < bottom_top_detection[0]) {
            new_vel.set_y(-bound_damping_coefficient * tmp_vel.y());
            tmp_pos.set_y(bottom_top_detection[0]+0.01f);
            //new_vel -= 2 * dot(normal_arr[2], tmp_vel) * normal_arr[2];
        }
        if (tmp_pos.y() > bottom_top_detection[1]) {
            new_vel.set_y(-bound_damping_coefficient * tmp_vel.y());
            tmp_pos.set_y(bottom_top_detection[1]-0.01f);
            //new_vel -= 2 * dot(normal_arr[3], tmp_vel) * normal_arr[3];
        }
        if (tmp_pos.z() < front_back_detection[0]) {
            new_vel.set_z(-bound_damping_coefficient * tmp_vel.z());
            tmp_pos.set_z(front_back_detection[0]+0.01f);
            //new_vel -= 2 * dot(normal_arr[4], tmp_vel) * normal_arr[4];
        }
        if (tmp_pos.z() > front_back_detection[1]) {
            new_vel.set_z(-bound_damping_coefficient * tmp_vel.z());
            tmp_pos.set_z(front_back_detection[1]-0.01f);
            //new_vel -= 2 * dot(normal_arr[5], tmp_vel) * normal_arr[5];
        }
        in_particle.set_velocity(new_vel);
        in_particle.set_position(tmp_pos);
    }
	void set_bound_damping_coefficient(float v) {
		this->bound_damping_coefficient = v;
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
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo;
	}

	GLuint create_ebo() {
		GLuint indices[] = {
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		return ebo;
	}

	GLuint create_vao() {
		GLuint vao = -1;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		GLuint vbo = create_vbo();
		GLuint ebo = create_ebo();
		const GLuint pos_loc = 0;
		glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glEnableVertexAttribArray(pos_loc);
		glBindVertexArray(0);
		return vao;
	}

	void draw(GLuint vao,glm::mat4 m,GLuint shader_program) {
		glUseProgram(shader_program);
		int PVM_loc = glGetUniformLocation(shader_program, "PVM");
		if (PVM_loc != -1) {
			glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(m));
		}
		glBindVertexArray(vao);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);
		//glDrawArrays(GL_TRIANGLES, 0, 8);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(0);
	}
};


#endif //SPH_FLUID_SOLVER_RESTRICTIONBOX_H
