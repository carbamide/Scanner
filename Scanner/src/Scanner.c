#include "Scanner.h"

// Handlers

static void begin_scan_handler(ClickRecognizerRef recognizer, void *context)
{
    // Scanning.  Really, the user will probably never see this as the next text
    // update happens too quickly
    text_layer_set_text(text_layer, "Scanning");
    
    // Set the text_layer's text alignment to be center
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    
    // Set the font to the larger pertage_font
    text_layer_set_font(text_layer, percentage_font);
    
    // Fire off the update timer, 100ms out
    update_timer = app_timer_register(100, handle_timer, NULL);
}

void handle_timer(void *data)
{
    // Create a random number.  Used to figure out how much percentage to add to the current_count
    int random_number = rand() % 10;
    
    current_count += random_number;
    
    // If the current count is greater than or equal to 100%, let the user
    // know what the "scan" was able to tell about the person
    if (current_count >= 100) {
        text_layer_set_text(text_layer, "100%");
        
        // Cancel the update_timer since we're done
        app_timer_cancel(update_timer);
        
        // Reset the current_count so another scan can heppen
        current_count = 0;
        
        // Notify the user of the results
        result_timer = app_timer_register(200, display_results, NULL);
    }
    else {
        // Create a buffer to hold the scan percentage
        static char buffer[25];
        
        // Pebble doesn't have malloc, so we can't use sprintf
        snprintf(buffer, 25, "%d%%", current_count);
        
        // Set the text layer's text to the buffer
        text_layer_set_text(text_layer, buffer);
        
        // Get ready for the next update, 100ms out
        update_timer = app_timer_register(100, handle_timer, NULL);
    }
}

void display_results(void *data)
{
    // Create a random number.  Used to tell the user what the "scan" has returned
    int random_number = rand() % 10;

    switch (random_number) {
        case 0:
            text_layer_set_text(text_layer, "Loser");

            break;
        case 1:
            text_layer_set_text(text_layer, "Idiot");

            break;
        case 2:
            text_layer_set_text(text_layer, "Republican");

            break;
        case 3:
            text_layer_set_text(text_layer, "Libertarian");

            break;
        case 4:
            text_layer_set_text(text_layer, "Democrat");

            break;
        case 5:
            text_layer_set_text(text_layer, "Awesome");

            break;
        case 6:
            text_layer_set_text(text_layer, "Insane");

            break;
        case 7:
            text_layer_set_text(text_layer, "Horse");

            break;
        case 8:
            text_layer_set_text(text_layer, "Invalid");

            break;
        case 9:
            text_layer_set_text(text_layer, "Terrorist");

            break;
        case 10:
            text_layer_set_text(text_layer, "Masturbator");

            break;
        default:
            text_layer_set_text(text_layer, "Error");
            
            break;
    }

    // Reset the font and cancel the timer
    text_layer_set_font(text_layer, normal_font);
    app_timer_cancel(result_timer);
}

static void reset_scan_handler(ClickRecognizerRef recognizer, void *context)
{
    // Reset the app to the default state
    text_layer_set_text(text_layer, "Top Button to Begin Scan");
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
}

static void click_config_provider(void *context)
{
    // Subscribe to the click events of the top and bottom buttons
    window_single_click_subscribe(BUTTON_ID_UP, begin_scan_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, reset_scan_handler);
}

// Window Lifecycle

static void window_load(Window *window)
{
    // Create the fonts that are used in the app
    percentage_font = fonts_get_system_font(FONT_KEY_GOTHIC_28);
    normal_font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
    
    // Create the main bitmaps.  This doesn't currently work, but I'm not sure why.
    start_scan_button_image = gbitmap_create_with_resource(BEGIN_SCAN_ICON);
    reset_scan_button_image = gbitmap_create_with_resource(RESET_SCAN_ICON);
    
    // Call the root layer of the window to get a bounds of it.  Used for positioning other elements
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    // Create the main text layer
    text_layer = text_layer_create(GRectMake(0, 45, bounds.size.w, 28));
    text_layer_set_font(text_layer, normal_font);
    text_layer_set_text(text_layer, "Top Button to Begin Scan");
    text_layer_set_text_alignment(text_layer, GTextAlignmentLeft);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
    
    // Create and configure the action_bar.  This may go away.  I'm still feeling out the UI paradigm
    action_bar = action_bar_layer_create();
    action_bar_layer_add_to_window(action_bar, window);
    action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, start_scan_button_image);
    action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, reset_scan_button_image);
}

static void window_unload(Window *window)
{
    // Destroy the reference to the text_layer
    text_layer_destroy(text_layer);
    
    // Destroy the used bitmaps
    gbitmap_destroy(start_scan_button_image);
    gbitmap_destroy(reset_scan_button_image);
    
    // Destroy the action_bar
    action_bar_layer_destroy(action_bar);
}

// App Initilization

static void init(void)
{
    // Create a window and hold a reference to it
    window = window_create();
    
    // Set the window handlers
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    
    // Push the newly created window to the window stack
    window_stack_push(window, true);
}

static void deinit(void)
{
    window_destroy(window);
}

int main(void)
{
    // Main init
    init();
    
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    
    // Begin event loop
    app_event_loop();
    
    // Deinit
    deinit();
}

// Helpers that will be useful to strip out in the future!

GRect GRectMake(float x, float y, float width, float height)
{
    GRect rect = (GRect){ .origin = { x, y }, .size = { width, height } };
    
    return rect;
}
