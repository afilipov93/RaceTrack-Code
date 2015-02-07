#ifndef GameVariables_h
#define GameVariables_h

static int GROUND_X = 100;
static int GROUND_Z = 100;

// Used to isolate the track from the end of the ground
static float PADDING = (float)(GROUND_Z + GROUND_X) / 10;

static int TURNS_NUM = 30;
// From 1.0f being easiest to 0.1f being hardest
static float TRACK_DIFFICULTY = 0.1f;

// Used for pushing apart points to avoid overlapping
static int TRACK_REFINEMENT = 5;

#endif