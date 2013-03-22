#include "main_sub_windows.h"

gboolean display = TRUE; //是否显示
GtkWidget *sub_window; //子窗口
GtkWidget *mi_table;
GtkWidget *notebook;
PangoFontDescription *fontdesc;
GtkWidget *dialog;
GtkWidget *dialog1;
gboolean activity_mode;
int p1;

extern void file();
extern void calc();
extern int client( char buffer[1024] );
void create_sub_window ();

//定义列表的列标题
static gchar	*titles[4] = {"编号","  标题  ","         内容         ","  时间  "};
const gchar *new_row[4] ;//定义字符串数组指针，指向要向表格中保存的数据
static GtkCList *clist; //列表
static GtkWidget *add_win; //添加数据窗口
static GtkWidget *entry_id; //编号
static GtkWidget *entry_name; //姓名
static GtkWidget *entry_sex; //性别
static GtkWidget *entry_birthday; //出生年月
static GtkWidget *entry_email; //电子邮件
gint current_row = 0 ; //当前行
gint row_count = 0 ; //总行数

static gchar	*titles1[4] = {"编号","课程名称","     考试时间      ","   考场   "};
const gchar *new_row1[4] ;//定义字符串数组指针，指向要向表格中保存的数据
static GtkCList *clist1; //列表
static GtkWidget *add_win1; //添加数据窗口
static GtkWidget *entry_id1; //编号
static GtkWidget *entry_name1; //姓名
static GtkWidget *entry_sex1; //性别
static GtkWidget *entry_birthday1; //出生年月
static GtkWidget *entry_email1; //电子邮件
gint current_row1 = 0 ; //当前行
gint row_count1 = 0 ; //总行数

//颜色设置
static GdkColor yellow = { 0, 0xffff , 0xffff, 0 };	//淡黄色
static GdkColor pink = { 0,0xffff , 0,  0xffff };	//粉色
static GdkColor littleviolet = { 0,0xffff , 0xaaaa,  0xffff };//淡紫色	
static GdkColor white = { 0,0xffff , 0xffff,  0xffff };	//白色
static GdkColor blue = { 0, 0, 0, 0xffff };		//蓝色...
static GdkColor cyan = { 0, 0 , 0xffff, 0xffff };	//淡蓝
static GdkColor red = { 0, 0xffff, 0x3333, 0x3333 };	//红色
static GdkColor green = { 0, 0, 0xffff, 0 };	//绿色

void on_show1 (GtkButton* button,gpointer data)
{ 
	//如果显示则消除
	if(display == FALSE)
	{
		gtk_widget_destroy(sub_window);
		display = TRUE;
	}
	else
	{
		//如果未显示则显示
		create_sub_window();
		display = FALSE;
	}
}

gint delete_event1( GtkWidget *widget,GdkEvent *event,gpointer data )
{
	gtk_widget_destroy(sub_window);
	display = TRUE;

	return FALSE;
}

/* 更新进度条,这样就能够看到进度条的移动 */
gint progress_timeout( GtkWidget *pbar )
{
	gdouble new_val;
	pid_t  parent_pid;

	parent_pid = getppid();
	if( parent_pid == 1 )
	{
		exit(0);		
	}

	if (activity_mode)
		gtk_progress_bar_pulse (GTK_PROGRESS_BAR (pbar));		//仅指示活动状态
	else
	{
		/* 使用在调整对象中设置的取值范围计算进度条的值 */
		new_val = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (pbar)) + 0.01;
		if (new_val > 1.0)
			new_val = 0.0;
		/* 设置进度条的新值 */
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (pbar), new_val);
	}
	/* 这是一个 timeout 函数,返回 TRUE,这样它就能够继续被调用 */

	return TRUE;
}

void  enter_callback(GtkWidget *widget,GtkWidget *entry )
{
	
	int timer;
	GtkWidget *image;
	GtkWidget *hbox;
	GtkWidget *pbar;
	GtkWidget *align;
	const gchar *entry_text;
	

	if( p1=fork() )
	{
		entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
		printf("%s\n",entry_text);
		client( entry_text );
	}
	else
	{
    		dialog1 = gtk_dialog_new_with_buttons("正在获取数据.........", sub_window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_OK, GTK_RESPONSE_OK,NULL);
		gtk_widget_set_size_request(dialog1,250,84); 

      		 image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,GTK_ICON_SIZE_DIALOG);                                                       
      		 hbox = gtk_hbox_new(FALSE,5);
		 pbar = gtk_progress_bar_new ();       
		align = gtk_alignment_new (0, 0.5, 0, 0);
		gtk_widget_show (align);
		gtk_container_add (GTK_CONTAINER (align), pbar);
		timer = gtk_timeout_add (100, progress_timeout, pbar);
		gtk_progress_bar_set_text (GTK_PROGRESS_BAR (pbar), "........正在下载数据.......");

		gtk_dialog_set_has_separator(GTK_DIALOG(dialog1), TRUE);
      		gtk_container_set_border_width(GTK_CONTAINER (hbox),  0);
		gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
		gtk_box_pack_start (GTK_BOX (hbox), align, FALSE, FALSE, 5);
		gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog1)->vbox), hbox);

   		g_signal_connect(G_OBJECT(dialog1), "response",G_CALLBACK(gtk_widget_destroy), NULL);
	

       	gtk_widget_show_all(dialog1);  
		gtk_dialog_run(GTK_DIALOG (dialog1));   
	}
}

void get_message( GtkButton *get_button, GtkWindow *sub_window ) 
{
	GtkWidget *entry;	
	GtkWidget *image;
	GtkWidget *lable;
	GtkWidget *hbox;

    	dialog = gtk_dialog_new_with_buttons("获取数据", sub_window,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_STOCK_OK, GTK_RESPONSE_OK,NULL);
	gtk_widget_set_size_request(dialog,370,100); 
	fontdesc = pango_font_description_from_string("Simsun 12");
    	entry = gtk_entry_new ();
	gtk_entry_set_max_length (GTK_ENTRY (entry), 7);	//设置输入框的最大长度值
	gtk_widget_modify_font(entry,fontdesc);

       image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,GTK_ICON_SIZE_DIALOG);
       lable = gtk_label_new("请输入你要查找的班级学号");                                                        
       hbox = gtk_hbox_new(FALSE,5);
       
	gtk_dialog_set_has_separator(GTK_DIALOG(dialog), TRUE);
      	gtk_container_set_border_width(GTK_CONTAINER (hbox),  0);
	gtk_box_pack_start_defaults(GTK_BOX(hbox), image);
	gtk_box_pack_start_defaults(GTK_BOX(hbox), lable);
	gtk_box_pack_start_defaults(GTK_BOX(hbox), entry);
	gtk_widget_modify_text(entry,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_base(entry,GTK_STATE_NORMAL,&littleviolet);
	gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox);

	g_signal_connect (G_OBJECT (entry), "activate",G_CALLBACK (enter_callback),entry);	//用于连接数出信号
   	g_signal_connect(G_OBJECT(dialog), "response",G_CALLBACK(gtk_widget_destroy), NULL);
	

       gtk_widget_show_all(dialog);  
	gtk_dialog_run(GTK_DIALOG (dialog));           

            
}

void creat_not1()
{
	GtkWidget *note_lable1;
	GtkWidget *note_lable2;
	GtkWidget *frame;
	GtkWidget *scrolled_window;
	GtkWidget *last_table;
	int m, n;
	GtkWidget *fixed2;
	GtkWidget *get_button;
	FILE *course_fptr = NULL;
	char *ptr;
	char buffer[100];
	GtkWidget *hbox;
	GtkWidget *separator;

	note_lable1 = gtk_label_new ("课程管理");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable1, fontdesc);
	pango_font_description_free(fontdesc); 
	mi_table = gtk_table_new (100, 4, FALSE);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  mi_table, note_lable1);
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 15);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//设置滚动窗口出现方式
	gtk_table_attach_defaults (GTK_TABLE (mi_table), scrolled_window, 0, 4, 0, 99);
	gtk_widget_show (scrolled_window);
	/* 创建一个包含 20×6 个格子的表格 */
	last_table = gtk_table_new (20, 6, TRUE);
	/* 设置 x 和 y 方向的行间间距为 10 像素 */
	gtk_table_set_row_spacings (GTK_TABLE (last_table), 0);
	gtk_table_set_col_spacings (GTK_TABLE (last_table), 0);
	/* 将表格组装到滚动窗口中 */
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), last_table);
	gtk_widget_show (last_table);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("课序号");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 0, 1, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("课程名称");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 1, 4, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc); 
	
	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("任课教师");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 4, 6, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	for( m = 0 ; m <= 5; m++ )
	{
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
		note_lable2 = gtk_label_new ("～ ～ ～");
		gtk_widget_modify_font(note_lable2, fontdesc);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, m,m + 1, 1,  2 );
		gtk_widget_show (note_lable2);
		pango_font_description_free(fontdesc);
	}



	if((course_fptr = fopen("./wh_hit/coures_management.txt","r")) == NULL)
	{
		fprintf(stderr,"Open %s Error:%s\n","coures_management.txt",strerror(errno));
		exit(1);
	}

	for(m = 2; m <= 19; m++ )
	{
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_widget_modify_bg(note_lable2,GTK_STATE_NORMAL,&green);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 0, 1, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 1, 4, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 4, 6, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
	}
	fclose(course_fptr);

	fixed2 = gtk_fixed_new ();
	gtk_table_attach_defaults(GTK_TABLE (mi_table), fixed2, 0, 2, 99, 100);
	gtk_widget_show (fixed2);
	fontdesc = pango_font_description_from_string("bold 11");
	get_button = gtk_button_new_with_label ("自动获取");
	gtk_widget_set_size_request (get_button, 70, 25);	//设置按钮最小大小
	gtk_widget_modify_fg(GTK_BIN(get_button)->child,GTK_STATE_NORMAL,&pink);
	gtk_widget_modify_fg(GTK_BIN(get_button)->child,GTK_STATE_PRELIGHT,&cyan);
	GTK_WIDGET_SET_FLAGS (get_button, GTK_CAN_DEFAULT);
	gtk_container_add(GTK_CONTAINER(fixed2),get_button);
	gtk_fixed_put (GTK_FIXED (fixed2), get_button, 10, 2);
	g_signal_connect_swapped (G_OBJECT (get_button), "clicked",G_CALLBACK (get_message), sub_window);
	gtk_widget_show (get_button);
}

void creat_not2()
{
	GtkWidget *note_lable1;
	GtkWidget *note_lable2;
	GtkWidget *frame;
	GtkWidget *scrolled_window;
	GtkWidget *last_table;
	int m, n;
	GtkWidget *fixed2;
	GtkWidget *get_button;
	FILE *course_fptr = NULL;
	char *ptr;
	char buffer[100];
	GtkWidget *hbox;
	GtkWidget *separator;

	note_lable1 = gtk_label_new ("课时管理");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable1, fontdesc);
	pango_font_description_free(fontdesc); 
	mi_table = gtk_table_new (100, 4, FALSE);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  mi_table, note_lable1);
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 15);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//设置滚动窗口出现方式
	gtk_table_attach_defaults (GTK_TABLE (mi_table), scrolled_window, 0, 4, 0, 99);
	gtk_widget_show (scrolled_window);
	/* 创建一个包含 10×10 个格子的表格 */
	last_table = gtk_table_new (20, 9, TRUE);
	/* 设置 x 和 y 方向的行间间距为 10 像素 */
	gtk_table_set_row_spacings (GTK_TABLE (last_table), 0);
	gtk_table_set_col_spacings (GTK_TABLE (last_table), 0);
	/* 将表格组装到滚动窗口中 */
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), last_table);
	gtk_widget_show (last_table);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("课序号");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 0, 1, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("课程名称");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 1, 4, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc); 
	
	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("星期");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 4, 5, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("节次");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 5, 6, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("地点");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 6, 7, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	fontdesc = pango_font_description_from_string("bold 11");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	note_lable2 = gtk_label_new ("开课周次");
	gtk_widget_modify_font(note_lable2, fontdesc);
	gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 7, 9, 0,  1 );
	GTK_WIDGET_SET_FLAGS (note_lable2, GTK_CAN_DEFAULT);
	gtk_widget_show (note_lable2);
	pango_font_description_free(fontdesc);

	for( m = 0 ; m <= 8; m++ )
	{
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
		note_lable2 = gtk_label_new ("～ ～ ～");
		gtk_widget_modify_font(note_lable2, fontdesc);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, m,m + 1, 1,  2 );
		gtk_widget_show (note_lable2);
		pango_font_description_free(fontdesc);
	}



	if((course_fptr = fopen("./wh_hit/time_management.txt","r")) == NULL)
	{
		fprintf(stderr,"Open %s Error:%s\n","time_management.txt",strerror(errno));
		exit(1);
	}

	for(m = 2; m <= 19; m++ )
	{
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_widget_modify_bg(note_lable2,GTK_STATE_NORMAL,&green);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 0, 1, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 1, 4, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 4, 5, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 5, 6, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 6, 7, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
		if( (ptr = fgets(buffer,100,course_fptr)) == NULL ) 
		{
			break;
		}
		for( n = 0; n <= 30; n ++ )
		{
			if( buffer[n] == '\n' )
			{
				buffer[n] = '\0';
			}
		}
		note_lable2 = gtk_label_new (buffer);
		gtk_table_attach_defaults (GTK_TABLE (last_table), note_lable2, 7, 9, m,  m + 1 );
		gtk_widget_show (note_lable2);
		memset(buffer,'\0',100);
		fontdesc = pango_font_description_from_string("Sans");
		pango_font_description_set_size (fontdesc, 9 * PANGO_SCALE);	//设置字体大小
		gtk_widget_modify_font(note_lable2, fontdesc);
		pango_font_description_free(fontdesc); 
	}
	fclose(course_fptr);

	fixed2 = gtk_fixed_new ();
	gtk_table_attach_defaults(GTK_TABLE (mi_table), fixed2, 0, 2, 99, 100);
	gtk_widget_show (fixed2);
	fontdesc = pango_font_description_from_string("bold 11");
	get_button = gtk_button_new_with_label ("自动获取");
	gtk_widget_set_size_request (get_button, 70, 25);	//设置按钮最小大小
	gtk_widget_modify_fg(GTK_BIN(get_button)->child,GTK_STATE_NORMAL,&pink);
	gtk_widget_modify_fg(GTK_BIN(get_button)->child,GTK_STATE_PRELIGHT,&cyan);
	GTK_WIDGET_SET_FLAGS (get_button, GTK_CAN_DEFAULT);
	gtk_container_add(GTK_CONTAINER(fixed2),get_button);
	gtk_fixed_put (GTK_FIXED (fixed2), get_button, 10, 2);
	g_signal_connect_swapped (G_OBJECT (get_button), "clicked",G_CALLBACK (get_message), sub_window);
	gtk_widget_show (get_button);
}

void on_ok_clicked (GtkButton *button,gpointer data)
{
	new_row[0] = gtk_entry_get_text(GTK_ENTRY(entry_id));
	new_row[1] = gtk_entry_get_text(GTK_ENTRY(entry_name));
	new_row[2] = gtk_entry_get_text(GTK_ENTRY(entry_sex));
	new_row[3] = gtk_entry_get_text(GTK_ENTRY(entry_birthday));
	//以上代码取得用户输入
	row_count++;
	gtk_clist_append(GTK_CLIST(clist),new_row);//向表格中添加
	gtk_widget_destroy(add_win);
}

void on_cancel_clicked (GtkButton *button,gpointer data)
{
	gtk_widget_destroy(add_win);//销毁添加数据的窗口
}

GtkWidget* create_addwin (void)
{ 
	//创建添加数据窗口
	GtkWidget* win;
	GtkWidget* vbox;
	GtkWidget* table;
	GtkWidget* bbox;
	GtkWidget* label;
	GtkWidget* button;

	//创建添加数据窗口
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"添加记录");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(win),"delete_event",G_CALLBACK(gtk_widget_destroy),win);
	gtk_container_set_border_width(GTK_CONTAINER(win),10);


	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);
	table = gtk_table_new(4,2,FALSE);
	gtk_box_pack_start(GTK_BOX(vbox),table,FALSE,FALSE,5);
	label = gtk_label_new("编号");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,1);
	entry_id = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_id,1,2,0,1);
	label = gtk_label_new("标题");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,1,2);
	entry_name = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_name,1,2,1,2);
	label = gtk_label_new("内容");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,2,3);
	entry_sex = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_sex,1,2,2,3);
	label = gtk_label_new("时间");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,3,4);
	entry_birthday = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_birthday,1,2,3,4);

	bbox = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
	gtk_box_set_spacing(GTK_BOX(bbox),5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_END);

	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_ok_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_cancel_clicked),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	gtk_widget_show_all(win);

	return win;
}

GtkWidget* create_button (gchar* stockid)
{ 
	//创建带图像的按钮
	GtkWidget *button;
	GtkWidget *image;
	image = gtk_image_new_from_stock(stockid,GTK_ICON_SIZE_MENU);
	button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(button),image);

	return button;
}

void goto_first (GtkButton *button,gpointer data)
{ 
	//转首行
	current_row = 0 ;
	gtk_clist_select_row(GTK_CLIST(clist),current_row,0);
}

void goto_last (GtkButton *button,gpointer data)
{
	//转尾行
	current_row = row_count - 1 ;
	gtk_clist_select_row(GTK_CLIST(clist),current_row,0);
}

void go_back (GtkButton *button,gpointer data)
{ 
	//前一行
	current_row -- ;
	if(current_row == -1) 
		return ;
	gtk_clist_select_row(GTK_CLIST(clist),current_row,0);
}

void go_forward (GtkButton *button,gpointer data)
{ 
	//下一行
	current_row ++;
	if(current_row > row_count ) 
		return;
	gtk_clist_select_row(GTK_CLIST(clist),current_row,0);
}

void append_row (GtkButton *button,gpointer data)
{ 
	//添加数据
	add_win = create_addwin();
	gtk_widget_show(add_win);
}

void delete_row(GtkButton *button,gpointer data)
{
	gtk_clist_remove( clist,current_row );

}

void creat_not3()
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkTooltips* button_tips;
	GtkWidget *scrolled_window;
	GtkWidget *note_lable1;

	note_lable1 = gtk_label_new ("事件管理");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable1, fontdesc);
	pango_font_description_free(fontdesc); 
	mi_table = gtk_table_new (100, 4, FALSE);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  mi_table, note_lable1);
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 15);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//设置滚动窗口出现方式
	gtk_table_attach_defaults (GTK_TABLE (mi_table), scrolled_window, 0, 4, 0, 99);
	gtk_widget_show (scrolled_window);

	clist = gtk_clist_new_with_titles(4,titles);	//创建分栏列表
	gtk_clist_set_column_justification(clist,0,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist,1,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist,2,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist,3,GTK_JUSTIFY_CENTER);
	gtk_clist_set_foreground(clist,3,&red);
	gtk_clist_set_shadow_type(clist,GTK_SHADOW_IN);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), clist );
	bbox = gtk_hbutton_box_new();
	button_tips = gtk_tooltips_new();	 	//创建工具提示对象
	gtk_table_attach_defaults(GTK_TABLE (mi_table), bbox, 0, 4, 99, 100);
	gtk_box_set_spacing(GTK_BOX(bbox),5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_SPREAD);

	gtk_button_box_set_child_size(GTK_BUTTON_BOX(bbox),20,20);
	button = create_button(GTK_STOCK_GOTO_FIRST);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到首行","首行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(goto_first),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GO_BACK);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到前一行","前一行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(go_back),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GO_FORWARD);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到下一行","下一行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(go_forward),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GOTO_LAST);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到尾行","尾行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(goto_last),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_ADD);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"新增一行","新增");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(append_row),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_DELETE);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"删除一行","删除");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(delete_row),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);
}

void on_ok_clicked1 (GtkButton *button,gpointer data)
{
	new_row1[0] = gtk_entry_get_text(GTK_ENTRY(entry_id1));
	new_row1[1] = gtk_entry_get_text(GTK_ENTRY(entry_name1));
	new_row1[2] = gtk_entry_get_text(GTK_ENTRY(entry_sex1));
	new_row1[3] = gtk_entry_get_text(GTK_ENTRY(entry_birthday1));
	//以上代码取得用户输入
	row_count1++;
	gtk_clist_append(GTK_CLIST(clist1),new_row1);//向表格中添加
	gtk_widget_destroy(add_win1);
}

void on_cancel_clicked1 (GtkButton *button,gpointer data)
{
	gtk_widget_destroy(add_win1);//销毁添加数据的窗口
}

GtkWidget* create_addwin1 (void)
{ 
	//创建添加数据窗口
	GtkWidget* win;
	GtkWidget* vbox;
	GtkWidget* table;
	GtkWidget* bbox;
	GtkWidget* label;
	GtkWidget* button;

	//创建添加数据窗口
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win),"添加记录");
	gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(win),"delete_event",G_CALLBACK(gtk_widget_destroy),win);
	gtk_container_set_border_width(GTK_CONTAINER(win),10);


	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(win),vbox);
	table = gtk_table_new(4,2,FALSE);
	gtk_box_pack_start(GTK_BOX(vbox),table,FALSE,FALSE,5);
	label = gtk_label_new("编号");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,1);
	entry_id1 = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_id1,1,2,0,1);
	label = gtk_label_new("标题");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,1,2);
	entry_name1 = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_name1,1,2,1,2);
	label = gtk_label_new("内容");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,2,3);
	entry_sex1 = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_sex1,1,2,2,3);
	label = gtk_label_new("时间");
	gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,3,4);
	entry_birthday1 = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(table),entry_birthday1,1,2,3,4);

	bbox = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(vbox),bbox,FALSE,FALSE,5);
	gtk_box_set_spacing(GTK_BOX(bbox),5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_END);

	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_ok_clicked1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(on_cancel_clicked1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,5);
	gtk_widget_show_all(win);

	return win;
}

GtkWidget* create_button1 (gchar* stockid)
{ 
	//创建带图像的按钮
	GtkWidget *button;
	GtkWidget *image;
	image = gtk_image_new_from_stock(stockid,GTK_ICON_SIZE_MENU);
	button = gtk_button_new();
	gtk_container_add(GTK_CONTAINER(button),image);

	return button;
}

void goto_first1 (GtkButton *button,gpointer data)
{ 
	//转首行
	current_row1 = 0 ;
	gtk_clist_select_row(GTK_CLIST(clist1),current_row1,0);
}

void goto_last1 (GtkButton *button,gpointer data)
{
	//转尾行
	current_row1 = row_count1-1 ;
	gtk_clist_select_row(GTK_CLIST(clist1),current_row1,0);
}

void go_back1 (GtkButton *button,gpointer data)
{ 
	//前一行
	current_row1 -- ;
	if(current_row1 == -1) 
		return ;
	gtk_clist_select_row(GTK_CLIST(clist1),current_row1,0);
}

void go_forward1 (GtkButton *button,gpointer data)
{ 
	//下一行
	current_row1 ++;
	if(current_row1 > row_count1 ) 
		return;
	gtk_clist_select_row(GTK_CLIST(clist1),current_row1,0);
}

void append_row1 (GtkButton *button,gpointer data)
{ 
	//添加数据
	add_win1 = create_addwin1();
	gtk_widget_show(add_win1);
}

void delete_row1(GtkButton *button,gpointer data)
{
	gtk_clist_remove( clist1,current_row1 );

}

void creat_not4()
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkTooltips* button_tips;
	GtkWidget *scrolled_window;
	GtkWidget *note_lable1;

	note_lable1 = gtk_label_new ("考试管理");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable1, fontdesc);
	pango_font_description_free(fontdesc); 
	mi_table = gtk_table_new (100, 4, FALSE);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  mi_table, note_lable1);
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 15);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);	//设置滚动窗口出现方式
	gtk_table_attach_defaults (GTK_TABLE (mi_table), scrolled_window, 0, 4, 0, 99);
	gtk_widget_show (scrolled_window);

	clist1 = gtk_clist_new_with_titles(4,titles1);	//创建分栏列表
	gtk_clist_set_column_justification(clist1,0,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist1,1,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist1,2,GTK_JUSTIFY_CENTER);
	gtk_clist_set_column_justification(clist1,3,GTK_JUSTIFY_CENTER);
	gtk_clist_set_shadow_type(clist1,GTK_SHADOW_ETCHED_OUT);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), clist1 );
	bbox = gtk_hbutton_box_new();
	button_tips = gtk_tooltips_new();	 	//创建工具提示对象
	gtk_table_attach_defaults(GTK_TABLE (mi_table), bbox, 0, 4, 99, 100);
	gtk_box_set_spacing(GTK_BOX(bbox),5);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox),GTK_BUTTONBOX_SPREAD);

	gtk_button_box_set_child_size(GTK_BUTTON_BOX(bbox),20,20);
	button = create_button(GTK_STOCK_GOTO_FIRST);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到首行","首行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(goto_first1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GO_BACK);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到前一行","前一行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(go_back1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GO_FORWARD);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到下一行","下一行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(go_forward1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_GOTO_LAST);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"转到尾行","尾行");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(goto_last1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_ADD);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"新增一行","新增");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(append_row1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

	button = create_button(GTK_STOCK_DELETE);
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips),button,"删除一行","删除");
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(delete_row1),NULL);
	gtk_box_pack_start(GTK_BOX(bbox),button,FALSE,FALSE,2);
	gtk_widget_show(button);

}

void create_sub_window ()
{ 
	//创建子窗口
	GtkWidget *label, *note_lable, *label1;
	GtkWidget *main_table;
	GtkWidget *note_button;
	GtkWidget *button1, *button2;
	GtkWidget *hbox,*hbox1,*hbox2;	
	GtkWidget *fixed1;
	GtkWidget *fixed2;
	GdkPixbuf *pixbuf;	
	GtkWidget *v1box, *v2box;
	GtkWidget *image;
	GtkWidget *separator;
   	GdkPixbuf *backpixbuf = NULL;
   	GdkBitmap *backbitmap = NULL;
   	GdkPixmap *backpixmap = NULL;

	sub_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect (G_OBJECT (sub_window), "delete_event",G_CALLBACK (delete_event1), NULL);
	gtk_window_set_title(GTK_WINDOW(sub_window),"工大日程");
	gtk_window_set_default_size(GTK_WINDOW(sub_window),375,490);	//设置窗口长宽
	gtk_widget_set_size_request (sub_window, 410, 500);		//设置窗口最小大小
	gtk_container_set_border_width(GTK_CONTAINER(sub_window),10);
	

	//设置主窗口背景图片
   	gtk_widget_set_app_paintable(sub_window, TRUE);
   	gtk_widget_realize(sub_window);
  	backpixbuf = gdk_pixbuf_new_from_file("sub_ba.png", NULL);       // gdk函数读取png文件
   	gdk_pixbuf_render_pixmap_and_mask(backpixbuf, &backpixmap, &backbitmap, 200);    // alpha小于128认为透明
   	gtk_widget_shape_combine_mask(sub_window, backbitmap, 0, 0);          // 设置透明蒙板
   	gdk_window_set_back_pixmap(sub_window->window, backpixmap, FALSE);          // 设置窗口背景
   	g_object_unref(backpixbuf);
   	g_object_unref(backbitmap);
   	g_object_unref(backpixmap);


	//gtk_container_add (GTK_CONTAINER (sub_window), fixed1);
	//gtk_widget_show (fixed1);

	//创建最外层表格
	main_table = gtk_table_new (30, 6, FALSE);
	gtk_container_add (GTK_CONTAINER (sub_window), main_table);
	gtk_table_set_row_spacings (GTK_TABLE (main_table), 1);
	gtk_table_set_col_spacings (GTK_TABLE (main_table), 1);

	//创建笔记本
	notebook = gtk_notebook_new ();
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);	//设置页标签位置
	gtk_table_attach_defaults (GTK_TABLE (main_table), notebook, 0, 6, 0, 29);
	gtk_widget_show (notebook);

	//在笔记本后面追加几个页面
	creat_not1(); 
	creat_not2();
	creat_not3();
	creat_not4();

	//在笔记本后面追加几个页面
	note_lable = gtk_label_new ("时间设置");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable, fontdesc);
	pango_font_description_free(fontdesc); 
	v1box = gtk_vbox_new (FALSE, 0);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  v1box, note_lable);
	pixbuf = gdk_pixbuf_new_from_file("sub_time.png",NULL);
	image = gtk_image_new_from_pixbuf(pixbuf);
	gtk_box_pack_start(GTK_BOX(v1box),image,FALSE,FALSE,5);

	//在笔记本后面追加几个页面
	note_lable = gtk_label_new ("工具管家");
	fontdesc = pango_font_description_from_string("Sans");
	pango_font_description_set_size (fontdesc, 10 * PANGO_SCALE);	//设置字体大小
	gtk_widget_modify_font(note_lable, fontdesc);
	pango_font_description_free(fontdesc); 
	v2box = gtk_vbox_new (FALSE, 0);
	gtk_notebook_append_page (GTK_NOTEBOOK (notebook),  v2box, note_lable);

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (v2box), hbox, FALSE, FALSE, 0);
	fixed2 = gtk_fixed_new ();
	gtk_box_pack_start(GTK_BOX(hbox),fixed2,FALSE,FALSE,5);
	gtk_widget_show (fixed2);

	button2 = gtk_button_new( );
	g_signal_connect(G_OBJECT(button2),"clicked",G_CALLBACK(calc),NULL);
	gtk_widget_set_size_request(button2,110,50);
	gtk_container_add(GTK_CONTAINER(fixed2),button2);
	gtk_fixed_put (GTK_FIXED (fixed2), button2, 20, 30);
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox1), 2);
	image = gtk_image_new_from_file ("cal.png");
	label1 = gtk_label_new ("计算机");
	gtk_box_pack_start (GTK_BOX (hbox1), image, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX (hbox1), label1, FALSE, FALSE, 3);
	gtk_widget_show (image);
	gtk_widget_show (label1);
	gtk_container_add (GTK_CONTAINER (button2), hbox1);
	gtk_widget_show ( button2);	
	

	button1 = gtk_button_new();
	g_signal_connect(G_OBJECT(button1),"clicked",G_CALLBACK(file),NULL);
	gtk_widget_set_size_request(button1,125,50);
	gtk_container_add(GTK_CONTAINER(fixed2),button1);
	gtk_fixed_put (GTK_FIXED (fixed2), button1, 140,30);
	hbox1 = gtk_hbox_new (FALSE, 0);
	gtk_container_set_border_width (GTK_CONTAINER (hbox1), 2);
	image = gtk_image_new_from_file ("paper.png");
	label1 = gtk_label_new ("文档管理");
	gtk_box_pack_start (GTK_BOX (hbox1), image, FALSE, FALSE, 3);
	gtk_box_pack_start (GTK_BOX (hbox1), label1, FALSE, FALSE, 3);
	gtk_widget_show (image);
	gtk_widget_show (label1);
	gtk_container_add (GTK_CONTAINER (button1), hbox1);
	gtk_widget_show (button1);

	label1 = gtk_label_new ("此页用来添加一些常用小工具,方便用户使用！\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n作者：郭猛\n软件功能介绍：用户可以通过自动获取按钮，\n输入要查询的班级号码，从服务端获取信息，\n从而系统会自动更新每星期课程，并且还会显\n示：本学期课程及其详细信息，每日上课时间\n同时允许用户设置事件及考试安排等并增加了\n计算器、日历和文档管理软件功能!\n");
	gtk_container_add(GTK_CONTAINER(fixed2),label1);
	gtk_fixed_put (GTK_FIXED (fixed2), label1, 30,200);
	gtk_widget_show (label1);


	//设置固定容器来固定退出按钮的位置，将固定容器放在表格中
	fixed1 = gtk_fixed_new ();
	gtk_table_attach_defaults(GTK_TABLE (main_table), fixed1, 4, 6, 29, 30);
	gtk_widget_show (fixed1);
	fontdesc = pango_font_description_from_string("bold 11");
	note_button = gtk_button_new_with_label ("关闭");
	gtk_widget_set_size_request (note_button, 60, 30);	//设置按钮最小大小
	gtk_widget_modify_fg(GTK_BIN(note_button)->child,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_fg(GTK_BIN(note_button)->child,GTK_STATE_PRELIGHT,&cyan);
	GTK_WIDGET_SET_FLAGS (note_button, GTK_CAN_DEFAULT);
	gtk_container_add(GTK_CONTAINER(fixed1),note_button);
	gtk_fixed_put (GTK_FIXED (fixed1), note_button, 10, 5);
	g_signal_connect_swapped (G_OBJECT (note_button), "clicked",G_CALLBACK (delete_event1), NULL);
	gtk_widget_show (note_button);

	gtk_widget_show_all(sub_window);
	gtk_window_move(GTK_WINDOW(sub_window),400,100);//移动窗口

}



