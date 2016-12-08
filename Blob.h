// #ifndef BLOB_H
// #define BLOB_H

#include "Pixel.h"
class Blob{
public:
    Blob();
    void copy(Blob blob);
    void clear();
    void add_pixel(Pixel);
    bool is_active();
    int get_size();
    void set_assigned();
    void clear_assigned();
    bool is_assigned();

    int min[2]; /**< The minimum bounds for the blob*/
    int max[2]; /**< The maximum bounds for the blob*/
    float centroid[2];  /**< The centroid location of the blob*/
    float aspect_ratio; /**< Ratio of the blobs width to its height*/
    float average_temperature;  /**< Average temperature of the pixels in the blob*/
    int width;  /**< Maximum width of the blob in pixels*/
    int height; /**< Maximum height of the blob in pixels*/
    int num_pixels; /**< Number of pixels contained in the blob*/

private:
    void recalculate_centroid(float pixel_x, float pixel_y);
    void recalculate_bounds(int x, int y);

    float total_x;  /**< Sum of all the pixel's x coordinates - used for averaging*/
    float total_y;  /**< Sum of all the pixel's y coordinates - used for averaging*/
    bool _is_assigned;  /**< Flag indicating if a blob has been assigned to a tracked blob*/
};

enum COORDINATES{
    X = 1,
    Y = 0
};

// #endif
