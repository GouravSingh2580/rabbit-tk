#include <map>
#include "window.hpp"

using namespace std;

void rtk_process_one_event(xcb_generic_event_t * e);

/* rtk_main_event_loop - main event loop
 * works by running select(), then calling xcb_poll_for_event if the 
 * select() returns data available on the X connection socket, and 
 * rtk_process_one_event if the returned event is non-null.
 *
 * Other things can also be plugged into the event loop, most immediately,
 * callbacks for reacting to select conditions.
 * 
 *
 * Eventually, this will become a multithread event dispatch system, with
 * events potentially getting dispatched to event loops in other threads. 
 */

map<xcb_window_t, Window *> windows;

void rtk_main_event_loop(xcb_connection_t * c)
{
	xcb_screen_t * screen;

	if(c == NULL) {
		c = xcb_connect (NULL, NULL);
		screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;
	}

	while(1) {
		xcb_generic_event_t * e = xcb_poll_for_event(c);
		if(!e) continue;

		// XXX: dispatch to other thread if needed
		rtk_process_one_event(e);
	}
	fprintf(stderr, "RTK: exited from event loop");
}

/* rtk_process_one_event - takes an xcb_generic_event_t
 * and does something useful with it, 
 */
void rtk_process_one_event(xcb_generic_event_t * e)
{
	// we don't do much processing beyond casting the event to the right type and
	// calling the right window's callback
	switch(e->response_type & ~0x80) {
	case XCB_EXPOSE:
	{
		xcb_expose_event_t * expose = (xcb_expose_event_t*)e;
		// find window with id expose->window
		// give it region to redraw
		if(windows.find(expose->window) == windows.end()) return;
		windows[expose->window]->redraw(expose->x, expose->y,
						expose->width, expose->height);
		break;
	}
	case XCB_BUTTON_PRESS:
	{
		xcb_button_press_event_t * butt = (xcb_button_press_event_t*)e;
		if(windows.find(butt->event) == windows.end()) return;
		// TODO: deal with modifiers
		windows[butt->event]->click(butt->detail, 0, 
					    butt->event_x, butt->event_y);
		break;
	}
	case XCB_BUTTON_RELEASE:
	{
		xcb_button_press_event_t * butt = (xcb_button_press_event_t*)e;
		if(windows.find(butt->event) == windows.end()) return;
		// TODO: deal with modifiers
		windows[butt->event]->unclick(butt->detail, 0, 
					      butt->event_x, butt->event_y);
		break;
	}
	default:
		fprintf(stderr, "unknown event %d\n", e->response_type & ~0x80);
		break;
	}
	free(e);

}

/* for the threading stuff, we need an atomic queue construct,
   which can be done lock-free with test-and-set quite easily
   via a retry mechanism */