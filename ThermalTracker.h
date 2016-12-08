#include "Pixel.h"
#include "Blob.h"
#include "TrackedBlob.h"
#include <Arduino.h>
#include <Wire.h>
#include <stdarg.h>

const int MINIMUM_TRAVEL_THRESHOLD = 5;
const int ADD_TO_BACKGROUND_DELAY = 20;
const bool INVERT_TRAVEL_DIRECTION = false;
const int FRAME_WIDTH = 16;
const int FRAME_HEIGHT = 4;
const int MAX_BLOBS = 8;
const int MINIMUM_BLOB_SIZE = 4;
const int MAX_DISTANCE_THRESHOLD = 200;
const int RUNNING_AVERAGE_SIZE = 80;
const int REFRESH_RATE = 16;
const int UNCHANGED_FRAME_DELAY = REFRESH_RATE * 2;
const int NUM_DIRECTION_CATEGORIES = 5;

enum directions {
    LEFT    = 0,
    RIGHT   = 1,
    UP      = 2,
    DOWN    = 3,
    NO_DIRECTION  = 4
};

class ThermalTracker{
public:
    ThermalTracker(int _running_average_size = RUNNING_AVERAGE_SIZE, int _max_distance_threshold = MAX_DISTANCE_THRESHOLD, int _min_blob_size = MINIMUM_BLOB_SIZE);
    void reset_background();
    void process_frame(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    bool finished_building_background();
    void get_averages(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    void get_variances(float frame_buffer[FRAME_HEIGHT][FRAME_WIDTH]);
    bool has_new_movements();
    int get_num_last_blobs();

private:     // Should be private, but left public for testing.
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
    void get_movements(long _movements[NUM_DIRECTION_CATEGORIES]);
    void reset_movements();
    void sort_tracked_blobs(TrackedBlob tracked_blobs[MAX_BLOBS]);
    void add_remaining_blobs_to_tracked(Blob new_blobs[MAX_BLOBS], TrackedBlob old_tracked_blobs[MAX_BLOBS]);
    void clear_blobs(Blob blobs[MAX_BLOBS]);

    int get_blobs(Blob blobs[]);
    int get_active_pixels(Pixel pixel_buffer[]);
    void remove_small_blobs(Blob blobs[MAX_BLOBS]);
    int get_num_blobs(Blob blobs[MAX_BLOBS]);
    int get_num_blobs(TrackedBlob blobs[MAX_BLOBS]);
    int get_num_unassigned_blobs(Blob blobs[MAX_BLOBS]);
    int get_num_updated_blobs(TrackedBlob tracked_blobs[MAX_BLOBS]);

    TrackedBlob tracked_blobs[MAX_BLOBS]; /**<Blobs that are tracked between frames. Contains movement info*/

    float frame[FRAME_HEIGHT][FRAME_WIDTH];     /**< Currently loaded frame; contains temperture information for each pixel*/
    float pixel_averages[FRAME_HEIGHT][FRAME_WIDTH];    /**< Background average of the previously loaded frames*/
    float pixel_variance[FRAME_HEIGHT][FRAME_WIDTH];    /**< Background variance of the previously loaded frames*/

    long movements[5];  /**< Array to keep track of the movements detected by the tracking script*/
    bool movement_changed_since_last_check; /**< Movement flag; True if movement has occurred since last check*/
    int running_average_size;   /**< The number of frames needed in the background average before detection can occur*/
    int num_background_frames;  /**< The current number of frames included in the background calculations*/
    int max_distance_threshold; /**< The maximum distance between blobs where the blobs can be considered the same blob*/
    int min_blob_size;  /**< The minimum number of pixels needed in a blob to avoid being cut at detection time*/
    int num_unchanged_frames;   /**< The number of consecutive frames where the number of blobs hasn't changed*/
    int num_last_blobs; /**< Number of blobs in the previously loaded frame*/
};
