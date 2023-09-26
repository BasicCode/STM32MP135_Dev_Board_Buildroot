#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#define DISP_BUF_SIZE (1024 * 1024)

//Private functions
static lv_obj_t * lvgl_screen1();
static lv_obj_t * lvgl_screen2();
static void roller_event_handler(lv_event_t * e);
static void button1_event_handler(lv_event_t * e);
static void button2_event_handler(lv_event_t * e);
static void dropdown_event_handler(lv_event_t * e);
static void messagebox_event_handler(lv_event_t * e);

int main(void)
{
    /*LittlevGL init*/
    lv_init();

    /*Linux frame buffer device init*/
    fbdev_init();

    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];

    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);

    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);

    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    LV_IMG_DECLARE(mouse_cursor_icon); /*Set a cursor for the mouse*/
    lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
    lv_img_set_src(cursor_obj, &mouse_cursor_icon); /*Set the image source*/
    lv_indev_set_cursor(mouse_indev, cursor_obj); /*Connect the image  object to the driver*/


    /*Create a Demo*/
    lv_demo_widgets();
    //lv_demo_music();
    //lv_demo_stress();


    //Load the first screen
    //lv_scr_load(lvgl_screen1());

    /*Handle LitlevGL tasks (tickless mode)*/
    while(1) {
        lv_timer_handler();
        usleep(5000);
    }

    return 0;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

/**
 * My Screens
 */
static lv_obj_t * lvgl_screen1() {
	//Create a new screen
	lv_obj_t * scr1 = lv_obj_create(NULL);

	// Change the active screen's background color
	lv_obj_set_style_bg_color(scr1, lv_color_hex(0x003a57), LV_PART_MAIN);
	lv_obj_set_style_text_color(scr1, lv_color_hex(0xffffff), LV_PART_MAIN);

    /*Create a normal drop down list*/
    lv_obj_t * dd = lv_dropdown_create(scr1);

    lv_obj_t *list = lv_dropdown_get_list(dd);
    lv_obj_set_style_max_height(list, 100, LV_PART_MAIN);

    lv_dropdown_set_options(dd, "Apple\n"
                            "Banana\n"
                            "Orange\n"
                            "Cherry\n"
                            "Grape\n"
                            "Raspberry\n"
                            "Melon\n"
                            "Orange\n"
                            "Lemon\n"
                            "Nuts");

    lv_obj_align(dd, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_add_event_cb(dd, dropdown_event_handler, LV_EVENT_ALL, NULL);

    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(scr1);
    lv_obj_add_event_cb(btn1, button1_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Screen 2");
    lv_obj_center(label);

    return scr1;
}

static lv_obj_t * lvgl_screen2() {
	//Create a new screen
	lv_obj_t * scr = lv_obj_create(NULL);

	// Change the active screen's background color
	lv_obj_set_style_bg_color(scr, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
	lv_obj_set_style_text_color(scr, lv_color_hex(LV_PALETTE_GREY), LV_PART_MAIN);

    lv_obj_t * label;

    lv_obj_t * btn1 = lv_btn_create(scr);
    lv_obj_add_event_cb(btn1, button2_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_BOTTOM_MID, 0, -20);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Screen 1");
    lv_obj_center(label);

    return scr;
}

/**
 * LVGL event handlers
 */
static void dropdown_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Option: %s", buf);
    }
}
static void messagebox_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    char buttonPressed = lv_msgbox_get_active_btn_text(obj);

    if(buttonPressed) {
    	lv_msgbox_close(obj);
    	//lv_scr_load(lvgl_screen2());
    	lv_scr_load_anim(lvgl_screen2(), LV_SCR_LOAD_ANIM_NONE, 0, 0, 1);
    }
}
static void button1_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {

        static const char * btns[] = {"OK", ""};

        lv_obj_t * mbox1 = lv_msgbox_create(NULL, "Hello", "Click OK to go to the next screen...", btns, true);
        lv_obj_add_event_cb(mbox1, messagebox_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_center(mbox1);
    }
}
static void button2_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {

        //lv_scr_load(lvgl_screen1());

    	lv_scr_load_anim(lvgl_screen1(), LV_SCR_LOAD_ANIM_NONE, 0, 0, 1);
    }
}
static void roller_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected month: %s\n", buf);
    }
}