TARGET := build/IntelHexTool


CXX      := g++
CXXFLAGS := -std=c++17 -g -Wall -pthread

BUILD_DIR := build/obj

# --- ImGui Source Files ---
# List the necessary ImGui implementation files from the 'imgui' directory
IMGUI_SRCS := \
	imgui/imgui.cpp \
	imgui/imgui_draw.cpp \
	imgui/imgui_tables.cpp \
	imgui/imgui_widgets.cpp \
	imgui/imgui_impl_sdl2.cpp \
	imgui/imgui_impl_sdlrenderer2.cpp

# --- All Source Files ---

APP_SRCS := \
	src/main.cpp \
	src/HexParser.cpp \
	src/Memory.cpp \
	src/i8080.cpp \
	src/Symbols.cpp \
	imgui/ImGuiFileDialog.cpp


# combine the application sources with the Imgui Sources
SRCS := $(APP_SRCS) $(IMGUI_SRCS)
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# --- Libratries and Includes ---
# Add paths for both SDL2 and ImGui headers
INCLUDES := -Iincludes -Iimgui -Iincludes/imgui -Iincludes/SDL2

# Point to the local library files in the "libs" folder
LIBS := -Llibs -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -mwindows

# -- Define DLLs to be copied
DLLs_TO_COPY := libs/SDL2.dll

# --- Rules ---
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking executable..."
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
	@echo "Copying required DLLs..."
	@cp $(DLLs_TO_COPY) $(dir $@) # This is the copy command
	@echo "Build finished successfully: $(TARGET)"

# Generic rule to compile any .cpp file, regardless of its folder
$(BUILD_DIR)/%.o: %.cpp
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@mkdir -p $(CURDIR)/build/tmp
	export TEMP=$(CURDIR)/build/tmp TMP=$(CURDIR)/build/tmp TMPDIR=$(CURDIR)/build/tmp && $(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	@echo "Cleaning build files..."
	rm -rf build

.PHONY: all clean
