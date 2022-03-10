//Buzdugan Mihaela 321CB
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

//functie care converteste un numar intreg intr-un string 
//in functie de baza specificata
char *converter(unsigned int nr, int base) 
{ 
	static char v[20]; 
	static char numere[] = "0123456789abcdef";
	char *p = &v[19]; 
	*p = '\0'; 
	
	while(nr != 0) { 
		p--;
		*p = numere[nr%base]; 
		nr /= base; 
	}

	return p; 
}

//functie pentru afisarea la stdout
static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

//functie care are comportamentul asemanator functiei printf;
int iocla_printf(char *format, ...)
{
	va_list arg; 
	const char *character, *s, *conv_nr;
	char c; 
	int ch, size_l; 

	int contor = 0; 
									
	va_start(arg, format); 

	for(character = format; *character != '\0'; character++) { 

		while(*character != '%' && *character != '\0') { 
			write_stdout(character, 1);
			contor++;
			character++; 
		} 

		if(*character == '\0'){
			break;
		}

		character++;
		switch(*character) {

			case 'd': 
				ch = va_arg(arg,int); 	

				if(ch < 0) { 
					ch = -ch;
					c = '-';
					write_stdout(&c, 1);
					contor += 1;
				} 

				conv_nr = converter(ch,10);
				write_stdout(conv_nr, strlen(conv_nr));
				size_l = strlen(conv_nr);
				contor += size_l;
				break; 

			case 'u': 
				ch = va_arg(arg,unsigned int); 
				conv_nr = converter(ch,10);
				write_stdout(conv_nr, strlen(conv_nr));
				size_l = strlen(conv_nr);
				contor += size_l;
				break; 

			case 'x': 
				ch = va_arg(arg,unsigned int);
				conv_nr = converter(ch,16);
				write_stdout(conv_nr, strlen(conv_nr));
				size_l = strlen(conv_nr);
				contor += size_l;
				break; 			

			case 'c': 
				ch = va_arg(arg,int);	
				c = ch;
				write_stdout(&c, 1);
				size_l = strlen(&c);
				contor = contor+1;
				break; 

			case 's': 
				s = va_arg(arg,char *); 	
				write_stdout(s, strlen(s));
				size_l = strlen(s);
				contor += size_l;
				break; 

			default:	
				character--;
				write_stdout(character, 1);
				contor++;
				character++;
				break;
		}	
	} 

	va_end(arg); 
	
	return contor;
} 
