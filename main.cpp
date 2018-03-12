#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

short m_box_err(const char* message			= "No message given...",
				const char* content_title	= "Error!",
				const char* title			= "Error!"){
	switch(al_show_native_message_box(al_get_current_display(), title,
										content_title, message, NULL,
										ALLEGRO_MESSAGEBOX_ERROR)){
		case 0: return 0; // User closed the error screen lmao!
		case 1: return 1; // ...ok? ...I guess? (user pressed the ok button)
		case 2: return 2; // Pressing the imaginary no button lmfao
	}
}

ALLEGRO_BITMAP *make_err_img(){
	//create a 16x16 bitmap, as a template to draw on.
	ALLEGRO_BITMAP *temp_img = al_create_bitmap(16,16);
	
	//if somehow it's unable to be created, return a null pointer.
	if(!temp_img) return NULL;
	
	//switch drawing target to the temporary image
	al_set_target_bitmap(temp_img);
	
	//use black as the background.
	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	//begin writing alternating layers of black (from evens to odds until end)
	for(int y = 0; y <= 15; y++){
		for(int x = 0; x <= 15; x++){
			if((x + y) % 2 == 1)
				al_draw_circle(x+.5,y+.5, .2, al_map_rgb(255, 0, 222), 1);
		}
	}
	
	//switch drawing back to display
	al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
	
	//give the image back to the system.
	return temp_img;
}

ALLEGRO_BITMAP *smart_img_loader(const char *img_to_load){
	
	//create and attempt to load a bitmap.
	ALLEGRO_BITMAP *temp_img = NULL;
	temp_img = al_load_bitmap(img_to_load);
	
	//pop up error box if unable to load the image, ask to proceed or quit.
	if(!temp_img){
		switch(m_box_err("There's a problem with loading an image!",
				"Would you like to continue? Exit the box to quit.",
				img_to_load)){
			
			//if they exited the box, quit program.
			case 0: return NULL;
			
			//if they checked yes, create the 16x16 error bitmap.
			case 1: return make_err_img();
		}
	}
	
	//if it does not fail, proceed as normal.
	else
		return temp_img;
}


int main(int argc, char **argv){
	ALLEGRO_DISPLAY *disp = NULL;
	ALLEGRO_EVENT_QUEUE *evq = NULL;
	
	//error checking and stuff pretty much.
	if(!al_init()){
		m_box_err("Error in allegro5/allegro.h!", "Allegro did not initialize!",
				"Critical Error!");
	}
	
	if(!al_init_image_addon()){
		m_box_err("Error in allegro5/allegro_image.h!", "Allegro failed to make an image!",
				"Critical Error!");
	}
	
	if(!al_init_primitives_addon()){
		m_box_err("Error in allegro5/allegro_primitives.h!", "Allegro failed to initialize the primitives addon!",
				"Critical Error!");
	}
	
	disp = al_create_display(640, 480);
	if(!disp){
		m_box_err("Error in display creation!", "Allegro failed to create a display!",
				"Critical Error!");
	}
	
	//create an event queue
	evq = al_create_event_queue();
	
	//set the event queue to listen to the display
	al_register_event_source(evq, al_get_display_event_source(disp));
	
	//set entire screen buffer to white
	al_clear_to_color(al_map_rgb(255,255, 255));
	
	//draw the error image for testing purposes
	al_draw_bitmap(make_err_img(), 100, 100, 0);
	
	//flip buffer to make viewable
	al_flip_display();
	
	//main loop
	while(1){
		
		//create event and timeout
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;
		
		//register timeout to specific value
		al_init_timeout(&timeout, 0.06);
		
		//attempt to get an event using timeout
		bool get_event = al_wait_for_event_until(evq, &ev, &timeout);
		
		//if closing screen, pop up error message asking why :P
		if(get_event && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			int oof = m_box_err("Are you sure you want to quit?", "Really?", "WHY THO");
			if(oof < 2)
				break;
			else
				m_box_err("HOW DID YOU DO THIS?!?!?", "Wow, just wow...", "OOF");
		}
		
		//clear the screen
		al_clear_to_color(al_map_rgb(0,0,0));
		
		//draw the error test again
		al_draw_bitmap(make_err_img(), 100, 100, 0);
		
		//flip buffer again
		al_flip_display();
	}
	
	//destroy the components
	al_destroy_display(disp);
	al_destroy_event_queue(evq);
	
	return 0;
}