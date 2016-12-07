#include "Pixel.h"
#include "Blob.h"
#include "TrackedBlob.h"
#include <SimpleTimer.h>
#include <Arduino.h>
#include <Wire.h>
#include <stdarg.h>

const int MINIMUM_TRAVEL_THRESHOLD = 6;
const int ADD_TO_BACKGROUND_DELAY = 20;
const bool INVERT_TRAVEL_DIRECTION = false;
const int FRAME_WIDTH = 16;
const int FRAME_HEIGHT = 4;
const int MAX_BLOBS = 10;
const int MINIMUM_BLOB_SIZE = 4;
const int REFRESH_RATE = 16;

enum directions {
    LEFT    = 0,
    RIGHT   = 1,
    UP      = 2,
    DOWN    = 3,
    NO_DIRECTION  = 4
};

class ThermalTracker{
public:
    ThermalTracker(int _running_average_size = 50, int _max_distance_threshold = 50, int _min_blob_size = 6);
    void process_frame(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    bool finished_building_background();
    void get_averages(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    void get_variances(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    bool has_new_movements();

// private:
    void load_frame(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    void build_background();
    void add_frame_to_to_running_background();

    void track_blobs(Blob new_blobs[MAX_BLOBS], TrackedBlob old_tracked_blobs[MAX_BLOBS]);
    void update_tracked_blobs(Blob new_blobs[MAX_BLOBS], TrackedBlob old_tracked_blobs[MAX_BLOBS]);
    void generate_distance_matrix(TrackedBlob tracked_blobs[MAX_BLOBS], Blob blobs[MAX_BLOBS],float output[MAX_BLOBS][MAX_BLOBS]);
    float get_lowest_distance(float distance_matrix[MAX_BLOBS][MAX_BLOBS], int indexs[2]);
    void remove_distance_row_col(int row, int col, float distance_matrix[MAX_BLOBS][MAX_BLOBS]);
    void process_blob_movements(TrackedBlob blob);
    void add_movement(int direction);
    void get_movements(long _movements[]);
    void reset_movements();
    void sort_tracked_blobs(TrackedBlob tracked_blobs[MAX_BLOBS]);
    void add_remaining_blobs_to_tracked(Blob new_blobs[MAX_BLOBS], TrackedBlob old_tracked_blobs[MAX_BLOBS]);
    void clear_blobs(Blob blobs[MAX_BLOBS]);

    int get_blobs(Blob blobs[]);
    int get_active_pixels(Pixel pixel_buffer[]);
    int remove_small_blobs(Blob blobs[], int num_blobs);
    int get_num_blobs(Blob blobs[]);
    int get_num_blobs(TrackedBlob blobs[]);
    int get_num_unassigned_blobs(Blob blobs[MAX_BLOBS]);
    int get_num_updated_blobs(TrackedBlob tracked_blobs[MAX_BLOBS]);

    TrackedBlob tracked_blobs[MAX_BLOBS];

    float frame[FRAME_HEIGHT][FRAME_WIDTH];
    float pixel_averages[FRAME_HEIGHT][FRAME_WIDTH];
    float pixel_variance[FRAME_HEIGHT][FRAME_WIDTH];

    long movements[5];
    bool movement_changed_since_last_check;
    int running_average_size;
    int num_background_frames;
    int max_distance_threshold;
    int min_blob_size;
};
