#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/arch/i386/asm.h>

#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;


void terminal_setcursor()
{
   // The screen is 80 characters wide...
   uint16_t cursorLocation = terminal_row * 80 + terminal_column;
   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
} 

void terminal_scroll()
{

	// Row 25 is the end, this means we need to scroll up
	if(terminal_row > (VGA_HEIGHT-1))
	{
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		uint32_t i;
		for (i =0; i < (VGA_HEIGHT-1)*VGA_WIDTH; i++)
			VGA_MEMORY[i] = VGA_MEMORY[i+VGA_WIDTH];
	

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for (i = (VGA_HEIGHT-1)*VGA_WIDTH; i < (VGA_HEIGHT)*VGA_WIDTH; i++)
			VGA_MEMORY[i] = vga_entry(' ', terminal_color);

		// The cursor should now be on the last line.
		terminal_row = (VGA_HEIGHT-1);
		terminal_column=0;
	
	}
} 

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
	unsigned char uc = c;
	switch(uc)
	{
		case 0x09:
			terminal_column = (terminal_column+8) & ~(8-1);
		break;
		case '\r':
			terminal_column = 0;
		break;
		case '\n':
			terminal_column = 0;
			terminal_row++;
		break;
		default:
			terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
       			
       			terminal_column++;
			if (terminal_column >=VGA_WIDTH)
			{
				terminal_column= 0;
				terminal_row ++;
			}
		break;
	}
	terminal_setcursor();
	terminal_scroll();	
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}
