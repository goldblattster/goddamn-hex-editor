#include "main.h"

int main(int argc, char* argv[])
{
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  clear();
  refresh();
  if(argv[1] == NULL)
  {
    mvprintw(0,0, "No argument found. Press a key to continue.");
    getch();
  }
  else {
    parse_file(argv[1]);
  }
  echo();
  clear();
  endwin();
  
  return 0;
}

void parse_file(char* file_name)
{
  FILE *f_ptr;
  int c;
  char write_mode = 0;
  unsigned int i = 0;
  int x;
  unsigned int row = 0;
  char x_cursor = 11;
  char y_cursor = 1;
  char at_cursor;
  unsigned int sz;
  char* hex_buff;
  f_ptr = fopen(file_name, "r+b");
  if(f_ptr == NULL)
  {
    printw("\n The file you were trying to open cannot be opened.");
    return;
  }
  fseek(f_ptr, 0, SEEK_END);
  sz = ftell(f_ptr);
  fseek(f_ptr, 0, SEEK_SET);
  hex_buff = (char*) malloc(sz * sizeof(char));
  do
  {
    c = fgetc(f_ptr);
    hex_buff[i] = c;
    i++;
  }while(c != EOF);
  fclose(f_ptr);
  for(;;)
  {
    clear();
    dump_hex(hex_buff, row, 24, 0, sz);
    at_cursor = mvinch(y_cursor, x_cursor);
    //mvprintw(0, 0, "Goddamn Hex Editor 0.2 Write Enable:%i\n", write_mode);
    move(y_cursor, x_cursor);
    c = getch();
    switch(c) //Scroll up / down
    {
      case KEY_PPAGE:
	if(row != 0)
	  row--;
	break;
      case KEY_NPAGE:
	if(row != sz / 16)
	  row++;
	break;
      case 27: //27 == ESCAPE key
	free(hex_buff);
	return;
      case KEY_UP:
	if(y_cursor != 0)
	  y_cursor--;
	break;
      case KEY_DOWN:
	if(y_cursor != 23) 
	  y_cursor++;
	break;
      case KEY_LEFT:
	if(x_cursor != 11)
	  x_cursor--;
	break;
      case KEY_RIGHT:
	if(x_cursor != 57)
	  x_cursor++;
	break;
      case KEY_F(2):
	if(write_mode == 1)
	  write_mode = 0;
	else
	  write_mode = 1;
	break;
      default:
	if(write_mode != 0) //Unstable, don't write to files
	{
	  if(c >= 48 && c <= 57)//Is c a number character
	    write_hex(hex_buff, row + y_cursor, y_cursor, x_cursor, c);
	  if(c >= 97 && c <= 102)//Is a hex character
	    write_hex(hex_buff, row + y_cursor, y_cursor, x_cursor, c);
	}
	break;
    }
  }
}

void dump_hex(char* buff,unsigned int row, unsigned int line_dump, int y, unsigned int size)
{
  char hex_buff[17];
  char c;
  int x;
  unsigned int z;
  unsigned int s_z;
  
  for(; y < line_dump; y++)
  {
    z = (row + y) * 16;
    s_z = z;
    for(x = 0; x < 16; x++)
    {
      hex_buff[x] = buff[z++];
      if(hex_buff[x] < 32)
	hex_buff[x] = '.';
    }
    hex_buff[16] = '\0';
    mvprintw(y, 0, "%08X: ", (row + y) * 16);
    move(y, 11);
    for(x = 0; x < 16; x++)
    {
	if(buff[s_z + x] == EOF && s_z + x >= size)
	{
	  hex_buff[x + 1] = '\0';
	  mvprintw(y, 60, " %s\n", hex_buff);
	  return;
	}
	else
	   printw("%02X ", buff[s_z + x] & 0xFF);
    }
    printw(" %s\n", hex_buff);
    refresh();
  }
}

void write_hex(char* buff, unsigned int row, char y, char x, char byte) //Commented out due to being unstable
{
  return;
}

