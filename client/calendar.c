#include "main_sub_windows.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define DEF_PAD 10
#define DEF_PAD_SMALL 5
#define TM_YEAR_BASE 1900

GtkWidget *Main_window;
gboolean displaycal =  TRUE; //是否显示

typedef struct _CalendarData 
{
	GtkWidget *flag_checkboxes[5];
	gboolean settings[5];
	gchar  *font;
	GtkWidget *font_dialog;
	GtkWidget *window;
	GtkWidget *prev2_sig;
	GtkWidget *prev_sig;
	GtkWidget *last_sig;
	GtkWidget *month;
} CalendarData;

enum 
{
	calendar_show_header,
	calendar_show_days,
	calendar_month_change,
	calendar_show_week,
	calendar_monday_first
};


/*
* GtkCalendar 日历构件
*/
void calendar_date_to_string( CalendarData *data,char *buffer,gint buff_len )
{
	struct tm tm;
	time_t time;

	memset (&tm, 0, sizeof (tm));
	gtk_calendar_get_date (GTK_CALENDAR (data->window),&tm.tm_year, &tm.tm_mon, &tm.tm_mday);
	tm.tm_year -= TM_YEAR_BASE;
	time = mktime (&tm);
	strftime (buffer, buff_len-1, "%x", gmtime (&time));
}

void calendar_set_signal_strings( char *sig_str,CalendarData *data)
{
	const gchar *prev_sig;
	prev_sig = gtk_label_get_text (GTK_LABEL (data->prev_sig));
	gtk_label_set_text (GTK_LABEL (data->prev2_sig), prev_sig);
	prev_sig = gtk_label_get_text (GTK_LABEL (data->last_sig));
	gtk_label_set_text (GTK_LABEL (data->prev_sig), prev_sig);
	gtk_label_set_text (GTK_LABEL (data->last_sig), sig_str);
}

void calendar_month_changed( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "month_changed: ";
	calendar_date_to_string (data, buffer+15, 256-15);
	calendar_set_signal_strings (buffer, data);
}

void calendar_day_selected( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "day_selected: ";
	calendar_date_to_string (data, buffer+14, 256-14);
	calendar_set_signal_strings (buffer, data);
}

void calendar_day_selected_double_click( GtkWidget *widget,CalendarData *data )
{
	struct tm tm;
	char buffer[256] = "day_selected_double_click: ";

	calendar_date_to_string (data, buffer+27, 256-27);
	calendar_set_signal_strings (buffer, data);
	memset (&tm, 0, sizeof (tm));
	gtk_calendar_get_date (GTK_CALENDAR (data->window),&tm.tm_year, &tm.tm_mon, &tm.tm_mday);
	tm.tm_year -= TM_YEAR_BASE;
	if (GTK_CALENDAR (data->window)->marked_date[tm.tm_mday-1] == 0)
	{
		gtk_calendar_mark_day (GTK_CALENDAR (data->window), tm.tm_mday);
	}
	else
	{
		gtk_calendar_unmark_day (GTK_CALENDAR (data->window), tm.tm_mday);
	}
}

void calendar_prev_month( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "prev_month: ";
	calendar_date_to_string (data, buffer+12, 256-12);
	calendar_set_signal_strings (buffer, data);
}

void calendar_next_month( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "next_month: ";
	calendar_date_to_string (data, buffer+12, 256-12);
	calendar_set_signal_strings (buffer, data);
}

void calendar_prev_year( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "prev_year: ";
	calendar_date_to_string (data, buffer+11, 256-11);
	calendar_set_signal_strings (buffer, data);
}

void calendar_next_year( GtkWidget *widget,CalendarData *data )
{
	char buffer[256] = "next_year: ";
	calendar_date_to_string (data, buffer+11, 256-11);
	calendar_set_signal_strings (buffer, data);
}

void calendar_set_flags( CalendarData *calendar )
{
	gint i;
	gint options = 0;

	for (i = 0; i < 5; i++)
	if (calendar->settings[i])
	{
		options=options + (1<<i);
	}
	if (calendar->window)
		gtk_calendar_display_options (GTK_CALENDAR (calendar->window), options);
}
void calendar_toggle_flag( GtkWidget *toggle,CalendarData *calendar )
{
	gint i;
	gint j;
	j = 0;

	for (i = 0; i < 5; i++)
	if (calendar->flag_checkboxes[i] == toggle)
		j = i;
	calendar->settings[j] = !calendar->settings[j];
	calendar_set_flags (calendar);
}

void calendar_font_selection_ok( GtkWidget *button,CalendarData *calendar )
{
	GtkStyle *style;
	PangoFontDescription *font_desc;
	calendar->font = gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG (calendar->font_dialog));
	if (calendar->window)
	{
		font_desc = pango_font_description_from_string (calendar->font);
		if (font_desc)
		{
			style = gtk_style_copy (gtk_widget_get_style (calendar->window));
			style->font_desc = font_desc;
			gtk_widget_set_style (calendar->window, style);
		}
	}
}

void calendar_select_font( GtkWidget *button,CalendarData *calendar )
{
	GtkWidget *window;

	if (!calendar->font_dialog) 
	{
		window = gtk_font_selection_dialog_new ("Font Selection Dialog");
		g_return_if_fail (GTK_IS_FONT_SELECTION_DIALOG (window));
		calendar->font_dialog = window;
		gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
		g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (gtk_widget_destroyed),&calendar->font_dialog);
		g_signal_connect (G_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->ok_button),"clicked", G_CALLBACK (calendar_font_selection_ok),calendar);
		g_signal_connect_swapped (G_OBJECT (GTK_FONT_SELECTION_DIALOG (window)->cancel_button),"clicked",G_CALLBACK (gtk_widget_destroy),
		calendar->font_dialog);
	}
	window=calendar->font_dialog;
	if (!GTK_WIDGET_VISIBLE (window))
		gtk_widget_show (window);
	else
		gtk_widget_destroy (window);
}

gint delete_event2( GtkWidget *widget,GdkEvent *event,gpointer data )
{
	gtk_widget_destroy(Main_window);
	displaycal = TRUE;

	return FALSE;
}

void create_calendar()
{
	GtkWidget *vbox, *vbox2, *vbox3;
	GtkWidget *hbox;
	GtkWidget *hbbox;
	GtkWidget *calendar;
	GtkWidget *toggle;
	GtkWidget *button;
	GtkWidget *frame;
	GtkWidget *separator;
	GtkWidget *label;
	GtkWidget *bbox;
	static CalendarData calendar_data;
	gint i;
	gchar* title;
	PangoFontDescription *desc;
	
	//颜色设置
	static GdkColor yellow = { 0, 0xffff , 0xffff, 0 };	//淡黄色
	static GdkColor pink = { 0,0xffff , 0,  0xffff };	//粉色
	static GdkColor littleviolet = { 0,0xffff , 0xaaaa,  0xffff };//淡紫色	
	static GdkColor white = { 0,0xffff , 0xffff,  0xffff };	//白色
	static GdkColor blue = { 0, 0, 0, 0xffff };		//蓝色...
	static GdkColor cyan = { 0, 0 , 0xffff, 0xffff };	//淡蓝
	static GdkColor red = { 0, 0xffff, 0x3333, 0x3333 };	//红色
	static GdkColor green = { 0, 0, 0xffff, 0 };	//绿色

	struct 
	{
		char *label;
	} flags[] = { { "Show Heading" },{ "Show Day Names" },{ "No Month Change" },{ "Show Week Numbers" },{ "Week Start Monday" } };

	calendar_data.window = NULL;
	calendar_data.font = NULL;
	calendar_data.font_dialog = NULL;	//字体对话框
	for (i = 0; i < 5; i++) 
	{
		calendar_data.settings[i] = 0;
	}

	//设置背景图片
	gtk_rc_parse("calendar.rc");//用RC文件
	Main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_name(Main_window, "newwindow");

	gtk_window_set_title (GTK_WINDOW (Main_window), "GtkCalendar Example");	//创建主窗口
	gtk_container_set_border_width (GTK_CONTAINER (Main_window), 5);
	g_signal_connect (G_OBJECT (Main_window), "delete_event",G_CALLBACK (delete_event2), NULL);
	gtk_window_set_resizable (GTK_WINDOW (Main_window), FALSE);		//设置窗口伸缩性
	vbox = gtk_vbox_new (FALSE, DEF_PAD);
	gtk_container_add (GTK_CONTAINER (Main_window), vbox);

	/*
	* 顶级窗口,其中包含日历构件,设置日历各参数的复选按钮和设置字体的按钮
	*/
	hbox = gtk_hbox_new (FALSE, DEF_PAD);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, DEF_PAD);
	hbbox = gtk_hbutton_box_new ();
	gtk_box_pack_start (GTK_BOX (hbox), hbbox, FALSE, FALSE, DEF_PAD);
	gtk_button_box_set_layout (GTK_BUTTON_BOX(hbbox), GTK_BUTTONBOX_SPREAD);	//设置控件排列方式
	gtk_box_set_spacing (GTK_BOX (hbbox), 5);	//设置空间间隙
	/* 日历构件 */
	frame = gtk_frame_new ("Calendar");
	gtk_box_pack_start (GTK_BOX (hbbox), frame, FALSE, TRUE, DEF_PAD);
	calendar=gtk_calendar_new ();
	calendar_data.window = calendar;
	calendar_set_flags (&calendar_data);
	gtk_calendar_mark_day (GTK_CALENDAR (calendar), 19);
	gtk_container_add( GTK_CONTAINER (frame), calendar);
	g_signal_connect (G_OBJECT (calendar), "month_changed",G_CALLBACK (calendar_month_changed),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "day_selected",G_CALLBACK (calendar_day_selected),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "day_selected_double_click",G_CALLBACK (calendar_day_selected_double_click),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "prev_month",G_CALLBACK (calendar_prev_month),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "next_month",G_CALLBACK (calendar_next_month),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "prev_year",G_CALLBACK (calendar_prev_year),&calendar_data);
	g_signal_connect (G_OBJECT (calendar), "next_year",G_CALLBACK (calendar_next_year),&calendar_data);
	separator = gtk_vseparator_new ();
	gtk_box_pack_start (GTK_BOX (hbox), separator, FALSE, TRUE, 0);
	vbox2 = gtk_vbox_new (FALSE, DEF_PAD);
	gtk_box_pack_start (GTK_BOX (hbox), vbox2, FALSE, FALSE, DEF_PAD);
	/* 创建一个框架,放入设置各种参数的复选按钮 */
	frame = gtk_frame_new ("Flags");
	gtk_box_pack_start (GTK_BOX (vbox2), frame, TRUE, TRUE, DEF_PAD);
	vbox3 = gtk_vbox_new (TRUE, DEF_PAD_SMALL);
	gtk_container_add (GTK_CONTAINER (frame), vbox3);
	for (i = 0; i < 5; i++)
	{
		toggle = gtk_check_button_new_with_label (flags[i].label);
		g_signal_connect (G_OBJECT (toggle),"toggled",G_CALLBACK (calendar_toggle_flag),&calendar_data);
		if( i == 0  )
		{
		 	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (toggle), TRUE);//使"Show Heading"默认为有效
		}

		gtk_box_pack_start (GTK_BOX (vbox3), toggle, TRUE, TRUE, 0);
		calendar_data.flag_checkboxes[i] = toggle;
	}

	/* 创建一个按钮,用于设置字体 */
	desc = pango_font_description_from_string("bold 11");
	button = gtk_button_new_with_label ("字体");
	gtk_widget_modify_fg(GTK_BIN(button)->child,GTK_STATE_NORMAL,&yellow);
	gtk_widget_modify_fg(GTK_BIN(button)->child,GTK_STATE_PRELIGHT,&green);
	g_signal_connect (G_OBJECT (button),"clicked",G_CALLBACK (calendar_select_font),&calendar_data);
	gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);

	/*
	* 创建“信号-事件”部分
	*/
	frame = gtk_frame_new ("Signal events");
	gtk_box_pack_start (GTK_BOX (vbox), frame, TRUE, TRUE, DEF_PAD);
	vbox2 = gtk_vbox_new (TRUE, DEF_PAD_SMALL);
	gtk_container_add (GTK_CONTAINER (frame), vbox2);
	hbox = gtk_hbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, TRUE, 0);

	desc = pango_font_description_from_string("ltalic 10");
	label = gtk_label_new ("Signal:");
	gtk_widget_modify_fg(label,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_font(label,desc);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	calendar_data.last_sig = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox), calendar_data.last_sig, FALSE, TRUE, 0);
	hbox = gtk_hbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, TRUE, 0);
	
	desc = pango_font_description_from_string("ltalic 10");
	label = gtk_label_new ("Previous signal:");
	gtk_widget_modify_fg(label,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_font(label,desc);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	calendar_data.prev_sig = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox), calendar_data.prev_sig, FALSE, TRUE, 0);
	hbox = gtk_hbox_new (FALSE, 3);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, TRUE, 0);
	
	desc = pango_font_description_from_string("ltalic 10");
	label = gtk_label_new ("Second previous signal:");
	gtk_widget_modify_fg(label,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_font(label,desc);

	gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, TRUE, 0);
	calendar_data.prev2_sig = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox), calendar_data.prev2_sig, FALSE, TRUE, 0);
	bbox = gtk_hbutton_box_new ();
	gtk_box_pack_start (GTK_BOX (vbox), bbox, FALSE, FALSE, 0);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_END);

	desc = pango_font_description_from_string("bold 11");
	button = gtk_button_new_with_label ("关闭");
	gtk_widget_modify_fg(GTK_BIN(button)->child,GTK_STATE_NORMAL,&red);
	gtk_widget_modify_fg(GTK_BIN(button)->child,GTK_STATE_PRELIGHT,&cyan);
	g_signal_connect (G_OBJECT (button), "clicked",G_CALLBACK (delete_event2),NULL);
	gtk_container_add (GTK_CONTAINER (bbox), button);
	GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);
	gtk_widget_grab_default (button);

	gtk_widget_show_all (Main_window);
	gtk_window_move(GTK_WINDOW(Main_window),300,100);//移动窗口
}


void on_show2 (GtkButton* button,gpointer data)
{
	//如果显示则消除
	if(displaycal == FALSE)
	{
		gtk_widget_destroy(Main_window);
		displaycal = TRUE;
	}
	else
	{
		//如果未显示则显示
		create_calendar();
		displaycal = FALSE;
	}
}




