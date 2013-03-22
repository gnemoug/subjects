main:main.o main_window.o sub_window.o calendar.o client.o calc.o fileselection.o
	gcc -o main main.o main_window.o sub_window.o calendar.o  client.o calc.o fileselection.o `pkg-config --cflags --libs gtk+-2.0`
main.o:main.c main_sub_windows.h
	gcc -c main.c `pkg-config --cflags --libs gtk+-2.0`
main_window.o:main_window.c main_sub_windows.h 
	gcc -c main_window.c `pkg-config --cflags --libs gtk+-2.0`
sub_window.o:sub_window.c main_sub_windows.h
	gcc -c sub_window.c `pkg-config --cflags --libs gtk+-2.0`
calendar.o:calendar.c main_sub_windows.h
	gcc -c calendar.c `pkg-config --cflags --libs gtk+-2.0`
client.o:client.c main_sub_windows.h
	gcc -c client.c `pkg-config --cflags --libs gtk+-2.0`
calc.o:calc.c main_sub_windows.h
	gcc -c calc.c `pkg-config --cflags --libs gtk+-2.0`
fileselection.o:fileselection.c main_sub_windows.h
	gcc -c fileselection.c `pkg-config --cflags --libs gtk+-2.0`
