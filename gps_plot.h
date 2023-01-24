#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "configuration.h"

//PROVIDED IN "distance.c" BY ASSIGNMENT SPEC
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)

typedef struct Position_co_ords Position;

struct Position_co_ords {
    float lng;
    float lat;
};

Position* create_position_co_ords(float input_lng, float input_lat);

typedef struct GPS_plot *plot_pointer;

struct GPS_plot {
    Position *position;
    struct tm date_and_time;
    plot_pointer next;
};

plot_pointer create_gps_plot(float input_lon, float input_lat, struct tm input_date_and_time);

void read_plots(config_pointer config_settings, plot_pointer *head);

double distance(Position* first, Position* second);

long calculate_total_time(struct tm first, struct tm second);

void create_csv_file(plot_pointer results, config_pointer settings);

void create_graphs(config_pointer settings);