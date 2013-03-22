#include "main_sub_windows.h"
#define BUFFER_SIZE 1024

GtkWidget* create_main_window ()
{ 
	//变量声明
	GtkWidget *window;
	GtkWidget *image;
	GtkWidget *table;
	GtkWidget *hbox, *hhbox, *hhbox2,*hhbox3 ;
	GtkWidget *button1, *button2, *button;
   	GdkPixbuf *backpixbuf = NULL;
   	GdkBitmap *backbitmap = NULL;
   	GdkPixmap *backpixmap = NULL;
	GdkPixbuf *pixbuf;
	GtkWidget *v1box;	
	GtkWidget *fixed1, *fixed2, *fixed3, *fixed4;	
	GtkWidget *label;
	FILE *wh_hit_fp;	
	char buffer[BUFFER_SIZE], buffer1[BUFFER_SIZE], buffer3[BUFFER_SIZE];
	char *ptr;	
	PangoFontDescription *fontdesc;
	int i = 0, j = 0, m = 0, n = 0, filenum;
	int beginday;
	FILE *fptr = NULL;
	char buffer2[100];
	struct tm *dateptr;
	time_t lt;
	char file[20] = "./wh_hit/wh_hit";	
	char charbeginday[10];

	//颜色设置
	static GdkColor yellow = { 0, 0xffff , 0xffff, 0 };	//淡黄色
	static GdkColor pink = { 0,0xffff , 0,  0xffff };	//粉色
	static GdkColor littleviolet = { 0,0xffff , 0xaaaa,  0xffff };//淡紫色	
	static GdkColor white = { 0,0xffff , 0xffff,  0xffff };	//白色
	static GdkColor blue = { 0, 0, 0, 0xffff };		//蓝色...
	static GdkColor cyan = { 0, 0 , 0xffff, 0xffff };	//淡蓝
	static GdkColor red = { 0, 0xffff, 0x3333, 0x3333 };	//红色
	static GdkColor green = { 0, 0, 0xffff, 0 };	//绿色

	//创建主窗口
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
	gtk_window_set_title(GTK_WINDOW(window),"工大日程");
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE); 
	gtk_widget_set_usize(window,650,442);	//设置窗口大小
	
	//设置主窗口背景图片
   	gtk_widget_set_app_paintable(window, TRUE);
   	gtk_widget_realize(window);
  	backpixbuf = gdk_pixbuf_new_from_file("hit_wh2.png", NULL);       // gdk函数读取png文件
   	gdk_pixbuf_render_pixmap_and_mask(backpixbuf, &backpixmap, &backbitmap, 200);    // alpha小于128认为透明
   	gtk_widget_shape_combine_mask(window, backbitmap, 0, 0);          // 设置透明蒙板
   	gdk_window_set_back_pixmap(window->window, backpixmap, FALSE);          // 设置窗口背景
   	g_object_unref(backpixbuf);
   	g_object_unref(backbitmap);
   	g_object_unref(backpixmap);
	
	//创建最底层v1box
	v1box = gtk_vbox_new (FALSE, 0);	
	gtk_container_add (GTK_CONTAINER (window), v1box);
	
	//创建最上层横向盒（为了固定其位置，用了固定容器）
	fixed1 = gtk_fixed_new ();
	gtk_container_add (GTK_CONTAINER (v1box), fixed1);
	gtk_widget_show (fixed1);
	hbox = gtk_hbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(fixed1),hbox);
	gtk_container_set_border_width (GTK_CONTAINER (hbox), 15);
	gtk_fixed_put (GTK_FIXED (fixed1), hbox, 2, 1);
	
	//获取本学期第一天日期
	if((fptr = fopen("./wh_hit/begin.txt","r")) == NULL)
	{
		fprintf(stderr,"Open %s Error:%s\n","begin.txt",strerror(errno));
		exit(1);
	}	
	ptr = fgets(buffer2,100,fptr);
	beginday = atoi(buffer2);
	fclose(fptr);

	//获取今天对应第一天日期
	lt = time(NULL);
	dateptr = localtime(&lt);
	filenum = ( dateptr->tm_yday - beginday ) / 7;
	sprintf(charbeginday, "%d", filenum);
	strcat( file, charbeginday );
	strcat( file, ".txt" );	

	//读取文件获取日期
	if((wh_hit_fp = fopen( file,"r"))==NULL)
	{
		fprintf(stderr,"Open %s Error:%s\n","./wh_hit/wh_hit.txt",strerror(errno));
		exit(1);
	}	
	ptr = fgets(buffer,100,wh_hit_fp);
	ptr = fgets(buffer3,100,wh_hit_fp);
	strcat(buffer,buffer3);
	
	//设置时间显示
	label = gtk_label_new (buffer);
	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(label, fontdesc);
	pango_font_description_free(fontdesc); 
	gtk_widget_modify_fg(label,GTK_STATE_NORMAL,&littleviolet);
	gtk_widget_set_size_request(label,180,30);	//设置位置和大小
	gtk_box_pack_start (GTK_BOX(hbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);
	
	pixbuf = gdk_pixbuf_new_from_file("subject.png",NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_box_pack_start(GTK_BOX(hbox),image,FALSE,FALSE,5);

	//设置"日期设置"按钮
	button2 = gtk_button_new_with_label("日期设置");
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(on_show2),NULL);
	gtk_widget_set_size_request(button2,80,30);
	gtk_box_pack_start(GTK_BOX(hbox),button2,FALSE,FALSE,5);	
	gtk_widget_show ( button2);	

	//设置"日程设置"按钮
	button1 = gtk_button_new_with_label("日程设置");
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(on_show1),NULL);
	gtk_widget_set_size_request(button1,80,30);
	gtk_box_pack_start (GTK_BOX(hbox), button1, TRUE, TRUE, 0);
	gtk_widget_show (button1);

	//创建课表横向盒
	hhbox = gtk_hbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(fixed1),hhbox);	
	gtk_fixed_put (GTK_FIXED (fixed1), hhbox, 63, 96);
	table = gtk_table_new (7, 6, TRUE);
	/* 设置 x 和 y 方向的行间间距为 1 像素 */
	gtk_table_set_row_spacings (GTK_TABLE (table), 3);
	gtk_table_set_col_spacings (GTK_TABLE (table), 10);
	gtk_box_pack_start (GTK_BOX(hhbox), table, TRUE, TRUE, 0);
	gtk_widget_show (table);
	for(i=0;i<=6;i++)
	{
		for(j=0;j<=5;j++)
		{
			memset(buffer1,'\0',1024);
			for(m=0;m <=3;m++)
			{
				memset(buffer,'\0',1024);
				ptr = fgets(buffer,100,wh_hit_fp);
				if(m == 3)
				{
					for( n = 0; n <= 20; n ++ )
					{
						if( buffer[n] == '\n' )
						{
							buffer[n] = '\0';
						}
					}
				}
				strcat(buffer1,buffer);
			}
			label = gtk_label_new (buffer1);
			gtk_table_attach_defaults (GTK_TABLE (table), label,i, i + 1, j, j + 1);	//设置插入按钮的位置
			fontdesc = pango_font_description_from_string("Sans");
			pango_font_description_set_size (fontdesc, 8 * PANGO_SCALE);	//设置字体大小
			gtk_widget_modify_font(label, fontdesc);
			pango_font_description_free(fontdesc); 
			gtk_widget_show (label);
		}
	}

	//显示所有窗口
	gtk_widget_show_all(window);
	fclose(wh_hit_fp);	

	return window ;
}

