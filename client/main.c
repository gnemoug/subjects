#include "main_sub_windows.h"

static GtkWidget *main_window; //主窗口

int main ( int argc , char* argv[])
{
	gtk_init( &argc,&argv);
	main_window = create_main_window();
	gtk_widget_show(main_window);
	gtk_window_move(GTK_WINDOW(main_window),600, 100);//移动窗口
	gtk_main();

	return FALSE;
}



