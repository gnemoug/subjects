#include "main_sub_windows.h"

GtkWidget *filew;
/* 获得文件名,并将它打印到控制台(console)上 */
void file_ok_sel( GtkWidget *w,GtkFileSelection *fs )
{
	gtk_widget_destroy(filew);
}

void file()
{
	/* 创建一个新的文件选择构件 */
	filew = gtk_file_selection_new ("File selection");
	g_signal_connect (G_OBJECT (filew), "destroy",G_CALLBACK (gtk_widget_destroy), NULL);
	/* 为 ok_button 按钮设置回调函数,连接到 file_ok_sel function 函数 */
	g_signal_connect (G_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),"clicked",G_CALLBACK (file_ok_sel), filew);
	/* 为 cancel_button 设置回调函数,销毁构件 */
	g_signal_connect_swapped (G_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),"clicked",G_CALLBACK (gtk_widget_destroy), filew);
	/* 设置文件名,比如这个一个文件保存对话框,我们给了一个缺省文件名 */
	gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew),"");	// 该程序运行时缺省打开的文件名

	gtk_widget_show (filew);

}

