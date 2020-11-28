#include <gtk/gtk.h>
#include "message.h"

// Classic MVC. The View is the window_main.glade file, an XML file containing glade code.
// The Controller is the bshop.c file and this is the Model
void changeToEmpty(GtkImage *imageholder);
void *getMessagesThread();

// All of these are images
GtkWidget *window;
GtkWidget *street;
GtkWidget *barber;

GtkWidget *chair1;
GtkWidget *chair2;
GtkWidget *chair3;
GtkWidget *chair4;
GtkWidget *chair5;
GtkWidget *chair6;
GtkWidget *chair7;
GtkWidget *chair8;
GtkWidget *chair9;
GtkWidget *chair10;

// create an """"Observable"""" list of chairs
GtkWidget *chairs[10];
int logicalchairs[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Chairs being used rn
int MAX_CHAIRS = 10;

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    acquireMessageQueue();
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("glade/window_main.glade");

    // get handles for relevant widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    street = GTK_WIDGET(gtk_builder_get_object(builder, "streetholder"));
    barber = GTK_WIDGET(gtk_builder_get_object(builder, "barberchair"));
    chair1 = GTK_WIDGET(gtk_builder_get_object(builder, "chair1"));
    chair2 = GTK_WIDGET(gtk_builder_get_object(builder, "chair2"));
    chair3 = GTK_WIDGET(gtk_builder_get_object(builder, "chair3"));
    chair4 = GTK_WIDGET(gtk_builder_get_object(builder, "chair4"));
    chair5 = GTK_WIDGET(gtk_builder_get_object(builder, "chair5"));
    chair6 = GTK_WIDGET(gtk_builder_get_object(builder, "chair6"));
    chair7 = GTK_WIDGET(gtk_builder_get_object(builder, "chair7"));
    chair8 = GTK_WIDGET(gtk_builder_get_object(builder, "chair8"));
    chair9 = GTK_WIDGET(gtk_builder_get_object(builder, "chair9"));
    chair10 = GTK_WIDGET(gtk_builder_get_object(builder, "chair10"));

    // populate observable array
    GtkWidget *localchairs[] = {chair1, chair2, chair3, chair4, chair5, chair6, chair7, chair8, chair9, chair10};
    for (int i = 0; i < 10; i++)
    {
        chairs[i] = localchairs[i];
    }

    // Add signals to window
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    pthread_t thread1_id;
    pthread_create(&thread1_id, NULL, getMessagesThread, NULL);

    gtk_widget_show(window);
    gtk_main();

    return 0;
}

// Set chairs that have an image
void initChairs(int chair_num)
{
    MAX_CHAIRS = chair_num;
    printf("%d", MAX_CHAIRS);
    for (int i = chair_num; i < 10; i++)
    {
            changeToEmpty(chairs[i]);
    }
}
// Change image to client
void changeToClient(GtkImage *imageholder)
{
    gtk_image_set_from_file(imageholder, "glade/res/netsonar.jpg");
    sleep(1);
}

// change image to chairImage
void changeToChair(GtkImage *imageholder)
{
    gtk_image_set_from_file(imageholder, "glade/res/bridge.jpg");
    sleep(1);
}

// Delete image from chair
void changeToEmpty(GtkImage *imageholder)
{
    gtk_image_set_from_file(imageholder, NULL);
    sleep(1);
}

void putClientOnStreet()
{
    changeToClient(street);
}

void putClientOnWaitingRoom()
{
    //get first empty chair
    for (int i = 0; i < MAX_CHAIRS; i++)
    {
        if (logicalchairs[i] == 0)
        {
            logicalchairs[i] = 1;
            changeToClient(chairs[i]);
            break;
        }
    }
}

void removeClientFromWaitingRoom()
{
    //get first occupied chair
    for (int i = 0; i < MAX_CHAIRS; i++)
    {
        if (logicalchairs[i] == 1)
        {
            logicalchairs[i] = 0;
            changeToChair(chairs[i]);
            break;
        }
    }
}

void removeClientFromStreet()
{
    changeToEmpty(street);
}

void putClientOnBarberChair()
{
    changeToClient(barber);
}

void removeClientFromBarberChair()
{
    changeToChair(barber);
}

// SIGNAL called when window is closed
void on_main_window_destroy()
{
    gtk_main_quit();
}

void getMessages()
{
    while (1)
    {
        Message m;
        msgrcv(own_id, &m, sizeof(Message), 0, 0);
        processMessage(m);
    }
}

void *getMessagesThread()
{
    getMessages();
}

void processMessage(Message m)
{
    // 1: initChairs()
    // 2: putonstreet
    // 3: putonwaiting
    // 4: removestreet
    // 5: removewaiting
    // 6: putonbarber
    // 7: removefrombarber
    switch (m.op)
    {

    case 1:
    {
        initChairs(m.chairs);
        break;
    }
    case 2:
    {
        putClientOnStreet();
        break;
    }
    case 3:
    {
        putClientOnWaitingRoom();
        break;
    }
    case 4:
    {
        removeClientFromStreet();
        break;
    }
    case 5:
    {
        removeClientFromWaitingRoom();
        break;
    }
    case 6:
    {
        putClientOnBarberChair();
        break;
    }
    case 7:
    {
        removeClientFromBarberChair();
        break;
    }
    }
}