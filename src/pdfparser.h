#include <stdio.h>
#include <stdlib.h>
#include "/opt/homebrew/Cellar/poppler/24.11.0/include/poppler/glib/poppler.h"
#include <cairo.h>
#include <cairo-pdf.h>

char* uri;
int num_pages ;




void read_pdf(const char* filename);
void read_write_export_txts(const char* filename, char *file_exported); //this implimentaion failed to create a complete pdf object, 
char* get_absolute_path(char* relative_path);
void export_txt_to_pdf(const char* txt_filename, const char* pdf_filename);
