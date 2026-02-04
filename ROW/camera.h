#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera{
    public: 

    double aspect_ratio = 1.0; //width over height
    int image_width = 100; //rendered image width in pixels
    int samples_per_pixel = 10; // number of random samples per pixel
    int max_depth = 10; // maximum number of ray bounces

    double vfov = 90; //vertical view angle
    point3 lookfrom = point3(0,0,0); // point camera is looking from
    point3 lookat = point3(0,0,-1); // point camera is looking at
    vec3 vup = vec3(0,1,0); // camera-relative "up" direction

    double defocus_angle = 0; // variation angle of rays through each pixel for focal point
    double focus_dist = 10; //distance from camera to plane of perfect focus

    void render(const hittable& world){
        initialize();

        std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for(int j = 0; j < image_height; j++){
            std::clog << "\rScanlines Remaining: " << (image_height - j) << " " << "Percent Complete: " << (static_cast<float>(j)/image_height)*100 << "%      " << std::flush;
            for(int i = 0; i < image_width; i++){
                color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++){
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale*pixel_color);
            }
        }
        std::clog << "\rDone.                                            \n";
    }

    /*
    void renderPNG(const hittable& world){
        initialize();
        double image_matrix[3][image_height][image_width] = {0};

        for(int sample = 0; sample < samples_per_pixel; sample++){
            
        }


    }*/

    private:
    
    int image_height;    // rendered image height
    double pixel_samples_scale; // color scale factor for a sum of pixel samples
    point3 center;       // camera center
    point3 pixel00_loc;  // location of pixel (0,0)
    vec3 pixel_delta_u;  // offset to horizontal pixel
    vec3 pixel_delta_v;  // offset to vertical pixel
    vec3 u, v, w;        // camera frame basis vectors
    vec3 defocus_disk_u; // defocus disk horizontal radius
    vec3 defocus_disk_v; // defocus disl vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        // making sure image height is at least one.
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0/samples_per_pixel;

        center = lookfrom;

        // viewport dimensions.
        double theta = degrees_to_radians(vfov);
        double h = std::tan(theta/2);
        double viewport_height = 2*h*focus_dist;
        double viewport_width = viewport_height * (double(image_width)/image_height);

        // u,w,v unit basis vectors for the camera coordinate frame
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup,w));
        v = cross(w,u);

        // viewport edges vectors
        vec3 viewport_u = viewport_width*u;
        vec3 viewport_v = viewport_height*-v;

        // viewport pixel delta vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // location of the starting upper-left pixel
        vec3 viewport_upper_left = center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_u = u*defocus_radius;
        defocus_disk_v = v*defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // construct a camera ray originating from the defocus disk and directed at randomly sampled
        // point around the pixel location 1,j

        vec3 offset = sample_square();
        vec3 pixel_sample = pixel00_loc 
        + ((i+offset.x())*pixel_delta_u) 
        + ((j+offset.y())*pixel_delta_v);

        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        vec3 ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const { // returns the vector to a random poiint in the unit square [-.5,.5]
        return vec3(random_double() - 0.5, random_double() - 0.5,0);
    }

    point3 defocus_disk_sample() const {
        //returns a random point in the camera defocus disk
        vec3 p = random_in_unit_disk();
        return center + (p[0]*defocus_disk_u) + (p[1]*defocus_disk_v);
    }
    color ray_color(const ray& r, int depth, const hittable& world) const{
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;

        if (world.hit(r, interval(0.001,infinity), rec)) { //NOTE: ignores hits that are very close to the intersection point.
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r,rec,attenuation,scattered))
                return attenuation * ray_color(scattered,depth-1,world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        double a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }

};

#endif