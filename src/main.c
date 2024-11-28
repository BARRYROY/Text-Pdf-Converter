#include "pdfparser.h"

int main(int argc, char *argv[]) {

    printf("To run this program, provide text file path, you wish to convert to pdf as a command line argument argument");
    char pdf_file_export[] = "output.pdf";

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PDF file path>\n", argv[0]);
        return EXIT_FAILURE;
    }
    export_txt_to_pdf(argv[1], pdf_file_export);

    char *absolute_path = get_absolute_path(pdf_file_export);
    if(absolute_path){
        printf("%s is the absolute path of the PDF file.\n", absolute_path);
        read_pdf(absolute_path);
        free(absolute_path);
    }
    else{
        fprintf(stderr, "Failed to resolve the absolute path of the PDF file.\n");
        return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}



void read_pdf(const char *filename) {
    GError *error = NULL;
    PopplerDocument *document;

    uri = g_filename_to_uri(filename, NULL, &error);
    if (!uri) {
        fprintf(stderr, "Error converting file path to URI: %s\n", error->message);
        g_error_free(error);
        return;
    }

    document = poppler_document_new_from_file(uri, NULL, &error);
    g_free(uri);

    if (error) {
        fprintf(stderr, "Error opening PDF file: %s\n", error->message);
        g_error_free(error);
        return;
    }

    num_pages = poppler_document_get_n_pages(document);
    printf("Number of pages: %d\n", num_pages);

    for (int i = 0; i < num_pages; i++) {
        PopplerPage *page = poppler_document_get_page(document, i);
        if (!page) {
            fprintf(stderr, "Failed to retrieve page %d\n", i + 1);
            continue;
        }

        char *text = poppler_page_get_text(page);
        if (text) {
            printf("\n--- Page %d ---\n", i + 1);
            puts(text);
            g_free(text);
        } else {
            printf("Page %d contains no extractable text.\n", i + 1);
        }

        g_object_unref(page);
    }

    g_object_unref(document);
}

void read_write_export_txts(const char* filename, char *ret_pdf){
    FILE *txt_file;
    FILE *pdf_file;
    txt_file = fopen(filename, "r");

    if(!txt_file){
        printf("The file failed to open!\n");
        exit(1);
    }

    
    pdf_file =fopen(ret_pdf, "w");

    char buffer[1024];
    char pdf_content[100000]="";

    while(fgets(buffer, sizeof(buffer), txt_file)){
        strcat(pdf_content, buffer);
        //strcat(pdf_content, "\\n");
    }

    fclose(txt_file);

    fprintf(pdf_file, "4 0 obj\n<< /Length %ld >>\nstream\n", strlen(pdf_content));
    fprintf(pdf_file, "%s", pdf_content);
    fprintf(pdf_file, "\nendstream\nendobj\n");

    // Write cross-reference table and trailer
    fprintf(pdf_file, "xref\n0 5\n0000000000 65535 f \n0000000010 00000 n \n0000000053 00000 n \n0000000114 00000 n \n");
    fprintf(pdf_file, "0000000171 00000 n \ntrailer\n<< /Root 1 0 R /Size 5 >>\nstartxref\n241\n%%EOF\n");

    fclose(pdf_file);
    printf("PDF exported successfully to '%s'.\n", ret_pdf);

}

char* get_absolute_path(char* relative_path){
    char* absolute_path = malloc(1024);
    if(!absolute_path){
    printf("Error allocating memory to absolute path\n");
    return NULL; 
    }

    if(realpath(relative_path, absolute_path) != NULL){
        return absolute_path;
    }else {
        printf("Failed to get the absolute path to %s \n.", relative_path);
        free(absolute_path);
        return NULL;
    }
}

void export_txt_to_pdf(const char* txt_filename, const char* pdf_filename) {
    FILE *txt_file = fopen(txt_filename, "r");
    if (!txt_file) {
        printf("The text file failed to open!\n");
        exit(1);
    }

    // Create a PDF surface and context using Cairo
    cairo_surface_t *surface = cairo_pdf_surface_create(pdf_filename, 595, 842); // A4 size
    if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS) {
        printf("Error creating Cairo PDF surface\n");
        fclose(txt_file);
        return;
    }

    cairo_t *cr = cairo_create(surface);

    // Set up the font for text output
    cairo_select_font_face(cr, "Helvetica", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    // Define starting point for text
    double x = 10, y = 20;

    // Read the file and write it to the PDF
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), txt_file)) {
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, buffer);
        y += 14;  // Move the cursor down for the next line
    }

    // Clean up
    fclose(txt_file);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    printf("PDF exported successfully to '%s'.\n", pdf_filename);
}
