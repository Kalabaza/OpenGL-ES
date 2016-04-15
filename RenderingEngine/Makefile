# Input and output directories
INDIR=RenderingEngine
OUTDIR=bin

# Using the C++ compiler
CC=g++
# Directory for the include files (header.h)
INC_DIR=$(INDIR)/include
# Directory of the OpenGL ES shared objects
GLES_DIR=$(INDIR)/so
# Libraries used on the linking phase
LDLIBS=-lX11 -lGLESv2 -lEGL -lm
# Compilation flags to generate the object files
CFLAGS=-Wall -std=c++1y -D__cdecl=""

vpath %.cpp $(INDIR)
vpath %.h $(INDIR)

OBJECTS = $(addprefix $(OUTDIR)/, main.o mesh.o object.o openGLES.o scenemanager.o shader.o texture.o linux.o)

# Compile and link all the files
all: $(OUTDIR)/renderingengine
	@echo "All compiled"
	@cp $(INDIR)/shader/* $(OUTDIR)
	@cp $(INDIR)/texture/* $(OUTDIR)
	@cp $(INDIR)/obj/* $(OUTDIR)
	@cp $(INDIR)/resources.txt $(OUTDIR)

# Link all the object files
$(OUTDIR)/renderingengine: $(OBJECTS)
	@$(CC) -o $@ $(OBJECTS) -I$(INC_DIR) -L$(GLES_DIR) $(LDLIBS)

# Generate object files
$(OUTDIR)/%.o: %.cpp logger.h sceneobject.h stb_image.h variable.h
	@mkdir -p $(OUTDIR)
	@$(CC) -c $(CFLAGS) -I$(INC_DIR) -L$(GLES_DIR) $(LDLIBS) -o $@ $<

# Delete the object files and the executable
.PHONY: clean
clean:
	@rm -rf $(OUTDIR)
	@echo "All cleaned"
