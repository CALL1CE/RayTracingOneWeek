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
#include <filesystem>
#include "quad.h"
void random_spheres() {
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
    cam.background = color(0.70, 0.80, 1.00);

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

void two_spheres() {
	hittable_list world;

	auto checker = make_shared<checker_texture>(0.8, color(.2, .3, .1), color(.9, .9, .9));

	world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
	world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 20;
	cam.lookfrom = point3(13, 2, 3);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;

    auto start = std::chrono::high_resolution_clock::now();
	cam.render(world);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	//std::cout << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
	std::clog << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;

}


void earth() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::clog << "Current path is: " << currentPath << std::endl;
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(0, 0, 12);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void two_perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
	hittable_list world;

	// Materials
	auto left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
	auto back_green = make_shared<lambertian>(color(0.2, 1.0, 0.2));
	auto right_blue = make_shared<lambertian>(color(0.2, 0.2, 1.0));
	auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
	auto lower_teal = make_shared<lambertian>(color(0.2, 0.8, 0.8));

	// Quads
	world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
	world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
	world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
	world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
	world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
    cam.background = color(0.70, 0.80, 1.00);

	cam.vfov = 80;
	cam.lookfrom = point3(0, 0, 9);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	cam.render(world);
}

void simple_light() {
	hittable_list world;

	auto pertext = make_shared<noise_texture>(4);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    world.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
    world.add(make_shared<quad>(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

	camera cam;

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;
	cam.samples_per_pixel = 500;
	cam.max_depth = 50;
	cam.background = color(0, 0, 0);

	cam.vfov = 20;
	cam.lookfrom = point3(26, 3, 6);
	cam.lookat = point3(0, 2, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	auto start = std::chrono::high_resolution_clock::now();
	cam.render(world);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	//std::cout << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
	std::clog << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
}

void cornell_box() {
	hittable_list world;

	auto red = make_shared<lambertian>(color(.65, .05, .05));
	auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));

	world.add(make_shared<quad>(point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
	world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
	world.add(make_shared<quad>(point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
	world.add(make_shared<quad>(point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
	world.add(make_shared<quad>(point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));


	shared_ptr<hittable> box1 = box(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	world.add(box1);

	shared_ptr<hittable> box2 = box(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	world.add(box2);

	camera cam;

	cam.aspect_ratio = 1.0;
	cam.image_width = 600;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;
	cam.background = color(0, 0, 0);

	cam.vfov = 40;
	cam.lookfrom = point3(278, 278, -800);
	cam.lookat = point3(278, 278, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0;

	auto start = std::chrono::high_resolution_clock::now();
	cam.render(world);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	//std::cout << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
	std::clog << "代码执行花费的时间: " << duration.count() << " 秒" << std::endl;
}


int main() {
	switch (7) {
	case 1:  random_spheres();     break;
	case 2:  two_spheres();        break;
	case 3:  earth();              break;
	case 4:  two_perlin_spheres(); break;
	case 5:  quads();              break;
	case 6:  simple_light();       break;
	case 7:  cornell_box();        break;
	}
}
