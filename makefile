CC:= gcc
CFLAGS:= -std=c11 -O0 -Wall -g
OBJECTS_PATH:= ./objects
SOURCE_PATH:= ./sources

myls: $(OBJECTS_PATH)/list_files.o $(OBJECTS_PATH)/list_files_long_format.o $(OBJECTS_PATH)/list_recursive_files.o $(OBJECTS_PATH)/myls.o
	$(CC)  $(OBJECTS_PATH)/list_files.o $(OBJECTS_PATH)/list_files_long_format.o $(OBJECTS_PATH)/list_recursive_files.o $(OBJECTS_PATH)/myls.o  -o myls

$(OBJECTS_PATH)/list_files.o: ./objects $(SOURCE_PATH)/list_files.c
	@$(CC) -c $(SOURCE_PATH)/list_files.c
	@mv list_files.o $(OBJECTS_PATH)

$(OBJECTS_PATH)/list_files_long_format.o: ./objects $(SOURCE_PATH)/list_files_long_format.c
	@$(CC) -c $(SOURCE_PATH)/list_files_long_format.c
	@mv list_files_long_format.o $(OBJECTS_PATH)
	
$(OBJECTS_PATH)/list_recursive_files.o: ./objects $(SOURCE_PATH)/list_recursive_files.c
	@$(CC) -c $(SOURCE_PATH)/list_recursive_files.c
	@mv list_recursive_files.o $(OBJECTS_PATH)
	
$(OBJECTS_PATH)/myls.o: objects ./myls.c
	@$(CC) -c ./myls.c
	@mv myls.o $(OBJECTS_PATH)
	
objects:
	mkdir -p ./objects
