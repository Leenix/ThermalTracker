#ifndef BLOB_H
#define BLOB_H

#include "Pixel.h"
class Blob{
public:
    // Constructor
    Blob();

    // Public Methods
    void clear();
    void add_pixel(Pixel);
    void copy(Blob blob);
    int get_size();
    bool is_active();
    void set_assigned();
    void clear_assigned();
    bool is_assigned();

    int min[2];
    int max[2];
    float centroid[2];
    float aspect_ratio;
    float average_temperature;
    int width;
    int height;
    int num_pixels;

private:
    void recalculate_centroid(float pixel_x, float pixel_y);
    void recalculate_bounds(int x, int y);

    float total_x;
    float total_y;
    bool _is_assigned;
};

enum COORDINATES{
    X = 0,
    Y = 1
};

#endif
