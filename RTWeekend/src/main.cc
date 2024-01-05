#include"rtweekend.h"
//the rat.h and vec3.h is in↑
#include "bvh.h"
#include"camera.h"
#include"color.h"
//#include"hittable_list.h"
#include"material.h"
#include"sphere.h"
#include <chrono>
#include "texture.h"
void RandomSpheres() {
    hittable_list world;

	shared_ptr<checker_texture> checker = make_shared<checker_texture>(0.32,
		make_shared<solid_color>(vec3(0.2, 0.3, 0.1)),
		make_shared<solid_color>(vec3(0.9, 0.9, 0.9))
	);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    //auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    //world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = make_shared<solid_color>( color::random() * color::random());
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = make_shared<solid_color>(color::random(0.5, 1));
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(make_shared<solid_color>(color(0.4, 0.2, 0.1)));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(make_shared<solid_color>(color(0.7, 0.6, 0.5)), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    auto start = std::chrono::high_resolution_clock::now();

    cam.render(world);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
	//std::cout << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
    std::clog << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;

}

int main() {
    RandomSpheres();
}