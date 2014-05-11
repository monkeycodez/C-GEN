CFLAGS = -g -Wall -std=c99
LIBS = -Iinclude -llua5.1

SRCS=$(wildcard src/**/*.c src/*.c)

OBJDIR=obj


OBJECTS=$(addprefix $(OBJDIR)/, $(patsubst src/%.c,%.o,$(SRCS)))

vpath %.c src
CC=gcc

TARGET=bin/cgen

all: $(TARGET)

$(TARGET):  $(OBJECTS) 
	@echo Linking:  $@
	@$(CC) -o $@ $(OBJECTS) $(LIBS) $(CFLAGS)


$(OBJDIR)/%.o:	%.c
	@echo Building:  $@
	@$(CC) $(CFLAGS) $(LIBS) -c  -o $@  $<

run:
	@./$(TARGET)

clean:
	@echo Deleting:  $(TARGET) $(OBJECTS)
	@rm $(OBJECTS) $(TARGET) 
