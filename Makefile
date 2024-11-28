CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic \
         -I/opt/homebrew/Cellar/poppler/24.11.0/include \
         -I/opt/homebrew/include/glib-2.0 \
		 -I/opt/homebrew/lib/glib-2.0/include \
		 -I/opt/homebrew/include/cairo
LIBS = -L/opt/homebrew/Cellar/poppler/24.11.0/lib -lpoppler-glib \
       -L/opt/homebrew/lib -lgobject-2.0  -lglib-2.0 -lcairo 
TARGET = pdfparser
OUTPUT = output.pdf
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
FILES = /Users/imarao/Downloads/sense.mb.txt
#/Users/imarao/Desktop/Algorithms/pdfParser/phys_model.pdf
#/Users/imarao/Downloads/resume.pdf

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(FILES)

clean:
	rm -f $(OBJ_FILES) $(TARGET) $(OUTPUT)
