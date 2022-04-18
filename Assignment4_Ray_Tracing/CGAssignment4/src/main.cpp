/*The MIT License (MIT)

Copyright (c) 2021-Present, Wencong Yang (yangwc3@mail2.sysu.edu.cn).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include <array>
#include <vector>
#include <thread>
#include <iostream>
#include "rtweekend.h"
#include "WindowsApp.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "material.h"
#include "BVH.h"

using namespace std;
static std::vector<std::vector<color>> gCanvas;		//Canvas

// The width and height of the screen
const auto aspect_ratio = 3.0 / 2.0;
const int gWidth = 600;
const int gHeight = static_cast<int>(gWidth / aspect_ratio);

void rendering();

int main(int argc, char* args[])
{

	// Create window app handle
	WindowsApp::ptr winApp = WindowsApp::getInstance(gWidth, gHeight, "CGAssignment4: Ray Tracing");
	if (winApp == nullptr)
	{
		std::cerr << "Error: failed to create a window handler" << std::endl;
		return -1;
	}

	// Memory allocation for canvas
	gCanvas.resize(gHeight, std::vector<color>(gWidth));

	// Launch the rendering thread
	// Note: we run the rendering task in another thread to avoid GUI blocking
	std::thread renderingThread(rendering);

	// Window app loop
	while (!winApp->shouldWindowClose())
	{
		// Process event
		winApp->processEvent();

		// Display to the screen
		winApp->updateScreenSurface(gCanvas);

	}

	renderingThread.join();

	return 0;
}

void write_color(int x, int y, color pixel_color)
{
	// Out-of-range detection
	if (x < 0 || x >= gWidth)
	{
		std::cerr << "Warnning: try to write the pixel out of range: (x,y) -> (" << x << "," << y << ")" << std::endl;
		return;
	}

	if (y < 0 || y >= gHeight)
	{
		std::cerr << "Warnning: try to write the pixel out of range: (x,y) -> (" << x << "," << y << ")" << std::endl;
		return;
	}

	// Note: x -> the column number, y -> the row number
	double r = clamp(sqrt(pixel_color.x()), 0.0, 0.999);
	double g = clamp(sqrt(pixel_color.y()), 0.0, 0.999);
	double b = clamp(sqrt(pixel_color.z()), 0.0, 0.999);
	color temp(r, g, b);

	gCanvas[y][x] = temp;

}


color ray_color(const ray& r, const hittable& world, int depth) {
	if(depth <= 0) return color(0, 0, 0);

	hit_record record;
	if (world.hit(r, 0.001, infinity, record)) {
		ray scattered;
		color attenuation;

		if (record.mat_ptr->scatter(r, record, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth-1);
		}
		return color(0, 0, 0);
	
	}
	vec3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b +
				0.9 * random_double());
			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;
				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2,
						sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2,
						sphere_material));
				}
			}
		}
	} 

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
	return static_cast<hittable_list>(make_shared<BVH>(world, 0, 1));
}


void rendering()
{
	double startFrame = clock();
	char a[] = "Nov 19 2021" ;
	char b[] = "17:48:36";
	printf("CGAssignment4 (built %s at %s) \n", a, __TIME__); //__DATE__
	std::cout << "Ray-tracing based rendering launched..." << std::endl;


	/*hittable_list world;

	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

	world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));*/
	
	//world
	auto world = random_scene();


	//摄像机构建

	point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);

	double distance_focus = 10.0;
	double aperture = 0.1;

	camera CAM(lookfrom, lookat, vup, 20, aspect_ratio, aperture, distance_focus);
	//camera CAM(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 20, aspect_ratio);

	// Image

	const int image_width = gWidth;
	const int image_height = gHeight;


	

	// Render

	//渲染图像
	const int num_samples_per_pixel = 500;
	const int max_depth = 50;


	for (int j = image_height - 1; j >= 0; j--) {
		for (int i = 0; i < image_width; i++) {
			color pixel_color(0, 0, 0);
			for (int k = 0; k < num_samples_per_pixel; k++) {
				double u = double(i + random_double()) / (image_width - 1);
				double v = double(j + random_double()) / (image_height - 1);
				//u = clamp(u, 0.0, 1.0);
				//v = clamp(v, 0.0, 1.0);
				ray r = CAM.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);

			}
			pixel_color /= (double) num_samples_per_pixel;
			write_color(i, j, pixel_color);
		}
	}



	double endFrame = clock();
	double timeConsuming = static_cast<double>(endFrame - startFrame) / CLOCKS_PER_SEC;
	std::cout << "Ray-tracing based rendering over..." << std::endl;
	std::cout << "The rendering task took " << timeConsuming << " seconds" << std::endl;
}