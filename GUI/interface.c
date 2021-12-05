#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "interface.h"
//#include "../resize.h"
#include "../Image/rotation.h"
#include "../OCR.h"

#define M_PI 3.14159265358979323846


char *path;
SDL_Surface *image_display;
SDL_Surface *image_copy;
SDL_Surface *image_rotate;
SDL_Surface *image_process;
// Get the Widgets.
GtkImage *imageIni;
GtkWidget *window;
GtkWidget *file_chooser;
GtkWidget *resolve;
GtkWidget *saveButton;
GtkWidget *rotateButton;
GtkWidget *about;
GtkWidget *aboutButton;
GtkWidget *quitButton;
GtkEntry *entryAngle;



// Create strutures for GUI.
typedef enum State
{
	ROTATE,				//Rotate State
	ABOUT,				//About State
	QUIT,				//Quit State
}State;

typedef struct UserInterface
{
	GtkImage *imageIni;		//Image
	GtkWidget *window;		//Main Window
	GtkWidget *saveButton;		//Save Button
	GtkWidget *rotateButton;	//Rotate Button
	GtkWidget *aboutButton;		//About Button
	GtkWidget *quitButton;		//Quit Button
}UserInterface;

typedef struct GUI
{
	State state;			//State of GTK
	UserInterface ui;		//User Interface
}GUI;



// Display and Load an Image in the GUI.
void Display(char *path)
{
    g_print("file = %s\n", path);
    gtk_image_set_from_file(imageIni, path);
}
void Load()
{
    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    image_display = SDL_LoadBMP(path);
    //ResizeImage(image_display);
    //path = "ProcessedImage/rescaled.bmp";
    Display(path);
}



void RunOCR()
{
    path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_chooser));
    solve_image(path, "GUI/ProcessedImage/grid");
    Display("GUI/ProcessedImage/grid_solved.bmp");
}



/*
// Resize an image according to GtkImage size.
void ResizeImage(SDL_Surface *image)
{
    int x_max = gtk_widget_get_allocated_width(GTK_WIDGET(imageIni));
    int y_max = gtk_widget_get_allocated_height(GTK_WIDGET(imageIni));
    g_print("x = %i and y = %i\n", x_max, y_max);
    image_process = rescale(image, x_max, y_max);
    SDL_SaveBMP(image_process, "ProcessedImage/rescaled.bmp");
}
*/


// Save a current image into a folder.
void Save()
{
    if (image_display != NULL)
    {
	SDL_SaveBMP(image_display, "GUI/ProcessedImage/saved.bmp");
    }
}



// Rotate an image with a given angle, occurs an error if there is no image to rotate.
void Rotation()
{
	const char *angle_text = gtk_entry_get_text(GTK_ENTRY(entryAngle));
	double angle = atof(angle_text);
	angle = angle  * M_PI / 180;
	image_copy = image_display;
	if(image_display == NULL)
	{
		g_print("No image to rotate\n");
		return;
	}
	SDL_Surface *image_rotate = rotate(image_copy, angle);
	SDL_SaveBMP(image_rotate, "GUI/ProcessedImage/rotated.bmp");
	Display("GUI/ProcessedImage/rotated.bmp");
}



// Display a quick aboutDialogWindow.
void About()
{
	gtk_widget_show_all(about);
}



int main_gui()
{

    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder *builder = gtk_builder_new();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError *error = NULL;
    if(gtk_builder_add_from_file(builder, "GUI/interface.glade", &error) == 0)
    {
	g_printerr("Error loading file: %s\n", error->message);
	g_clear_error(&error);
	return 1;
    }


    window = GTK_WIDGET(gtk_builder_get_object(builder, "main"));


    // Gets the widgets for the first window.
    imageIni = GTK_IMAGE(gtk_builder_get_object(builder, "ImageIni"));
    file_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "File_chooser"));
    resolve = GTK_WIDGET(gtk_builder_get_object(builder, "Resolve"));
    saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "Save"));
    rotateButton = GTK_WIDGET(gtk_builder_get_object(builder, "RotateButton"));
    about = GTK_WIDGET(gtk_builder_get_object(builder, "About"));
    aboutButton = GTK_WIDGET(gtk_builder_get_object(builder, "AboutButton"));
    quitButton = GTK_WIDGET(gtk_builder_get_object(builder, "QuitButton"));
    entryAngle = GTK_ENTRY(gtk_builder_get_object(builder, "EntryAngle"));
    gtk_window_set_transient_for(GTK_WINDOW(about), GTK_WINDOW(window));



    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(file_chooser, "file-set", G_CALLBACK(Load), NULL);
    g_signal_connect(rotateButton, "clicked", G_CALLBACK(Rotation), NULL);
    g_signal_connect(aboutButton, "clicked", G_CALLBACK(About), NULL);
    g_signal_connect(quitButton, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(saveButton, "clicked", G_CALLBACK(Save), NULL);
    g_signal_connect(resolve, "clicked", G_CALLBACK(RunOCR), NULL);


    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
