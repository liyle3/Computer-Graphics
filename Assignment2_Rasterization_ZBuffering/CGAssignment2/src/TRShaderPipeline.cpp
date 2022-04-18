#include "TRShaderPipeline.h"

#include <algorithm>
//#include <iostream>

namespace TinyRenderer
{
	//----------------------------------------------VertexData----------------------------------------------

	int max(int a, int b) {
		return a > b ? a : b;
	}

	int min(int a, int b) {
		return a < b ? a : b;
	}

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::lerp(
		const TRShaderPipeline::VertexData &v0,
		const TRShaderPipeline::VertexData &v1,
		float frac)
	{
		//Linear interpolation
		VertexData result;
		result.pos = (1.0f - frac) * v0.pos + frac * v1.pos;
		result.col = (1.0f - frac) * v0.col + frac * v1.col;
		result.nor = (1.0f - frac) * v0.nor + frac * v1.nor;
		result.tex = (1.0f - frac) * v0.tex + frac * v1.tex;
		result.cpos = (1.0f - frac) * v0.cpos + frac * v1.cpos;
		result.spos.x = (1.0f - frac) * v0.spos.x + frac * v1.spos.x;
		result.spos.y = (1.0f - frac) * v0.spos.y + frac * v1.spos.y;

		return result;
	}

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::barycentricLerp(
		const VertexData &v0, 
		const VertexData &v1, 
		const VertexData &v2,
		glm::vec3 w)
	{
		VertexData result;
		result.pos = w.x * v0.pos + w.y * v1.pos + w.z * v2.pos;
		result.col = w.x * v0.col + w.y * v1.col + w.z * v2.col;
		result.nor = w.x * v0.nor + w.y * v1.nor + w.z * v2.nor;
		result.tex = w.x * v0.tex + w.y * v1.tex + w.z * v2.tex;
		result.cpos = w.x * v0.cpos + w.y * v1.cpos + w.z * v2.cpos;
		result.spos.x = w.x * v0.spos.x + w.y * v1.spos.x + w.z * v2.spos.x;
		result.spos.y = w.x * v0.spos.y + w.y * v1.spos.y + w.z * v2.spos.y;

		return result;
	}

	void TRShaderPipeline::VertexData::prePerspCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by 1/w before rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		v.pos.w = 1.0f / v.cpos.w;
		v.pos = glm::vec4(v.pos.x * v.pos.w, v.pos.y * v.pos.w, v.pos.z * v.pos.w, v.pos.w);
		v.tex = v.tex * v.pos.w;
		v.nor = v.nor * v.pos.w;
		v.col = v.col * v.pos.w;
	}

	void TRShaderPipeline::VertexData::aftPrespCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by w after rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		float w = 1.0f / v.pos.w;
		v.pos = v.pos * w;
		v.tex = v.tex * w;
		v.nor = v.nor * w;
		v.col = v.col * w;
	}

	//----------------------------------------------TRShaderPipeline----------------------------------------------

	void TRShaderPipeline::rasterize_wire(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screene_height,
		std::vector<VertexData> &rasterized_points)
	{
		//Draw each line step by step
		rasterize_wire_aux(v0, v1, screen_width, screene_height, rasterized_points);
		rasterize_wire_aux(v1, v2, screen_width, screene_height, rasterized_points);
		rasterize_wire_aux(v0, v2, screen_width, screene_height, rasterized_points);
	}


	void TRShaderPipeline::rasterize_fill_edge_function(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screen_height,
		std::vector<VertexData> &rasterized_points)
	{
		//Edge-function rasterization algorithm

		//Task4: Implement edge-function triangle rassterization algorithm
		// Note: You should use VertexData::barycentricLerp(v0, v1, v2, w) for interpolation, 
		//       interpolated points should be pushed back to rasterized_points.
		//       Interpolated points shold be discarded if they are outside the window. 

		//       v0.spos, v1.spos and v2.spos are the screen space vertices.

		//For instance:
        
		int left = min(v0.spos.x, min(v1.spos.x, v2.spos.x));
		int right = max(v0.spos.x, max(v1.spos.x, v2.spos.x));
		int bottom = min(v0.spos.y, min(v1.spos.y, v2.spos.y));
		int top = max(v0.spos.y, max(v1.spos.y, v2.spos.y));  
		for (int i = left; i <= right; i++) {
			for(int j = bottom; j <= top; j++){
				glm::vec3 xvector(v1.spos.x - v0.spos.x, v2.spos.x - v0.spos.x, v0.spos.x - i);
				glm::vec3 yvector(v1.spos.y - v0.spos.y, v2.spos.y - v0.spos.y, v0.spos.y - j);
				glm::vec3 u = glm::cross(xvector, yvector);
				glm::vec3 w = glm::vec3(1.f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
				if ((w.x - 0.0 >= 1e-5 || fabs(w.x - 0.0) < 1e-5) && 
					(w.y - 0.0 >= 1e-5 || fabs(w.y - 0.0) < 1e-5) &&
					(w.z - 0.0 >= 1e-5 || fabs(w.z - 0.0) < 1e-5)) {

					VertexData node = VertexData::barycentricLerp(v0, v1, v2, w);
					node.spos.x = i;
					node.spos.y = j;
					
					if (i <= screen_width && j <= screen_height) {
						rasterized_points.push_back(node);
					}
				}
			}
		}
		rasterized_points.push_back(v0);
		rasterized_points.push_back(v1);
		rasterized_points.push_back(v2);

	}
	
	
    void TRShaderPipeline::rasterize_wire_aux(
            const VertexData& from,
            const VertexData& to,
            const unsigned int& screen_width,
            const unsigned int& screen_height,
            std::vector<VertexData>& rasterized_points) {
          // Task1: Implement Bresenham line rasterization
          // Note: You shold use VertexData::lerp(from, to, weight) for
          // interpolation,
          //       interpolated points should be pushed back to
          //       rasterized_points. Interpolated points shold be discarded if
          //       they are outside the window.

          //       from.spos and to.spos are the screen space vertices.

          // For instance:

          int delta_x = to.spos.x - from.spos.x;
          int delta_y = to.spos.y - from.spos.y;

          int xstep = 1, ystep = 1;
          if (delta_x < 0) {
            xstep = -1;
            delta_x = -delta_x;
          }
          if (delta_y < 0) {
            ystep = -1;
            delta_y = -delta_y;
          }
          glm::ivec2 pre = from.spos, cur;
          int P;

          if (delta_y <= delta_x) {
            P = 2 * delta_y - delta_x;
            for (int i = 1; i < delta_x; i++) {
              if (P <= 0) {
                cur.x = pre.x + xstep;
                cur.y = pre.y;
                P = P + 2 * delta_y;
              } else {
                cur.x = pre.x + xstep;
                cur.y = pre.y + ystep;
                P = P + 2 * delta_y - 2 * delta_x;
              }

              pre = cur;

              VertexData node = VertexData::lerp(from, to, i / delta_x);
              node.spos = cur;
              if (cur.x > screen_width || cur.y > screen_height)
                continue;
              else
                rasterized_points.push_back(node);
            }
          } else {
            P = 2 * delta_x - delta_y;
            for (int i = 1; i < delta_y; i++) {
              if (P <= 0) {
                cur.y = pre.y + ystep;
                cur.x = pre.x;
                P = P + 2 * delta_x;
              } else {
                cur.y = pre.y + ystep;
                cur.x = pre.x + xstep;
                P = P + 2 * delta_x - 2 * delta_y;
              }

              pre = cur;

              VertexData node = VertexData::lerp(from, to, i / delta_y);
              node.spos = cur;
              if (cur.x > screen_width || cur.y > screen_height)
                continue;
              else
                rasterized_points.push_back(node);
            }
          }

          rasterized_points.push_back(from);
          rasterized_points.push_back(to);
        }

	void TRDefaultShaderPipeline::vertexShader(VertexData &vertex)
	{
		//Local space -> World space -> Camera space -> Project space
		vertex.pos = m_model_matrix * glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f);
		vertex.cpos = m_view_project_matrix * vertex.pos;
	}

	void TRDefaultShaderPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Just return the color.
		fragColor = glm::vec4(data.tex, 0.0, 1.0f);
	}
}