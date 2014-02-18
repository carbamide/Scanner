#include <pebble.h>
#include <stdio.h>
#include <math.h>
#include <pebble_fonts.h>

// Declarations

/**
 *  Main Window of the application
 */
static Window *window;

/**
 *  Text layer that displays all user text in application
 */
static TextLayer *text_layer;

/**
 *  The main action bar (right side) of the application
 */
static ActionBarLayer *action_bar;

/**
 *  Resource identifier
 */
static uint32_t BEGIN_SCAN_ICON = 0;

/**
 *  Resource identifer
 */
static uint32_t RESET_SCAN_ICON = 1;

/**
 *  Timer to be used to update the percentage label
 */
static AppTimer *update_timer = NULL;

/**
 *  Timer used to show the results of the scan
 */
static AppTimer *result_timer = NULL;

/**
 *  Reference to the start scan GBitmap. Holds a reference so the image doesn't have to be
 *  recreated each time it's needed
 */
GBitmap *start_scan_button_image = NULL;

/**
 *  Reference to the reset scan GBitmap. Holds a reference so the image doesn't have to be
 *  recreated each time it's needed
 */
GBitmap *reset_scan_button_image = NULL;

/**
 *  Current count of the percentage.  Reset after reaching 100%
 */
int current_count = 0;

/**
 *  GFont reference that holds the larger font that is used for the percentage label.
 *  Hold a reference so the font doesn't have to be recreated every time it's needed.
 */
static GFont *percentage_font = NULL;

/**
 *  GFont reference that holds the normal application font.  Hold a reference so the
 *  font doesn't have to be recreated every time it's needecd.
 */
static GFont *normal_font = NULL;

// Functions

/**
 *  Create GRect from parameters.  Similar to CGRectMake from CoreFoundation
 *
 *  @param x      Starting X point
 *  @param y      Starting Y point
 *  @param width  Requested width, in pixels
 *  @param height Requested height, in pixels
 *
 *  @return GRect struct with appropriate values
 */
GRect GRectMake(float x, float y, float width, float height);

/**
 *  Timer handler function
 *
 *  @param data The data that can optionally be passed by the AppTimer object
 */
void handle_timer(void *data);

/**
 *  Timer handler function
 *
 *  @param data The data that can optionally be passed by the AppTimer object
 */
void display_results(void *data);

/**
 *  Click handler
 *
 *  @param recognizer The recognizer that fired the handler
 *  @param context    The context description
 */
static void begin_scan_handler(ClickRecognizerRef recognizer, void *context);

/**
 *  Click Handler
 *
 *  @param recognizer The recognizer that fired the handler
 *  @param context    The context description
 */
static void reset_scan_handler(ClickRecognizerRef recognizer, void *context);

/**
 *  Click handler creation function
 *
 *  @param context The context description of the caller
 */
static void click_config_provider(void *context);
