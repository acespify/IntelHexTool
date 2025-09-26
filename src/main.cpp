#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Vendor Libraries
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

// Local Libraries
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/ImGuiFileDialog.h"

// Our Application Code
#include "HexParser.h"
#include "Memory.h"
#include "i8080.h"
#include "Symbols.h"

int main(int, char**) {
    // --- 1. Initialize SDL (Same as before) ---
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) { /* ... error handling ... */ return -1; }

    // Initialize the SDL_image library for PNG loading
    if(!(IMG_Init(IMG_INIT_PNG))){
        std::cerr << "ERROR initializing SDL_image: " << IMG_GetError() << std::endl;
        return -1;
    }
    // Creating the window 
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Intel HEX Tool", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

    // Load and Set the Window Icon
    SDL_Surface* icon_surface = IMG_Load("assets/IHT.png");
    if (icon_surface == NULL){
        std::cerr << "Warning: Cound not load icon.png: " << IMG_GetError() << std::endl;
    } else {
        SDL_SetWindowIcon(window, icon_surface);
        SDL_FreeSurface(icon_surface); // The window makes its own copy, so we can free this surface.
    }
    // Creating the Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { /* ... error handling ... */ return -1; }

    // --- 2. Initialize ImGui (Same as before) ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // --- 3. Application State ---
    std::string current_filename = "No file loaded";
    std::vector<HexRecord> loaded_records;
    MemoryMap memory_map; // Add the memory map to our application's state
    std::vector<DissassembeledInstruction> disassembly;
    SymbolMap symbol_map;

    // --- 4. Main Application Loop ---
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // --- Our GUI code ---
        ImGui::Begin("Intel HEX Tool");

        

        // -- File Operations --
        if (ImGui::Button("Open Hex File")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("HexFileOpen", "Choose a HEX File", ".hex,.txt,*.*", config);
        }
        ImGui::SameLine();
        ImGui::Text("Current File: %s", current_filename.c_str());

        // -- File Dialog Logic --
        if (ImGuiFileDialog::Instance()->Display("HexFileOpen")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string file_path = ImGuiFileDialog::Instance()->GetFilePathName();
                current_filename = ImGuiFileDialog::Instance()->GetCurrentFileName();
                
                // Use our new parser to load the file!
                loaded_records = parse_hex_file(file_path);
                memory_map = build_memory_map(loaded_records);
                symbol_map = generate_symbols(memory_map);
                disassembly.clear();
                if(!memory_map.empty()){
                    uint32_t pc = memory_map.begin()->first;
                    uint32_t end_addr = memory_map.rbegin()->first;
                    while (pc <= end_addr){
                        DissassembeledInstruction instr = disassemble_8080_op(memory_map,pc, symbol_map);
                        disassembly.push_back(instr);
                        pc += instr.size;

                        // Check if the new PC is in a gap.
                        if (memory_map.find(pc) == memory_map.end()){
                            // The new PC is not a valid address. Find the next valid one.
                            auto it = memory_map.upper_bound(pc);
                            if (it == memory_map.end()){
                                // If there are no more data left in the map, we are now done.
                                break;
                            }
                            // Jump the PC to the next valid address.
                            pc = it->first;
                        }
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::Separator();

        // Add a new window for the Memory Viewer
        ImGui::Begin("Memory Viewer");
        if (memory_map.empty()) {
            ImGui::Text("No data loaded into memory.");
        } else {
            // A simple hex editor view
            // Note: For large files, a more advanced implementation would only render visible lines.
            for (auto it = memory_map.begin(); it != memory_map.end(); ) {
                // Address
                ImGui::Text("0x%04X: ", it->first);
                ImGui::SameLine();
                
                // Hex Bytes (up to 16 per line)
                std::stringstream hex_ss;
                for (int i = 0; i < 16; ++i) {
                    if (it != memory_map.end()) {
                        hex_ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(it->second) << " ";
                        ++it;
                    } else {
                        hex_ss << "   ";
                    }
                }
                ImGui::TextUnformatted(hex_ss.str().c_str());
            }
        }
        ImGui::End();

        ImGui::Begin("8080 Disassembly");
       if (ImGui::Button("Save Disassembly")) {
            if (!disassembly.empty()) {
                ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".asm,.txt");
            }
        }

        ImGui::Separator();

        ImGui::BeginChild("DisassemblyScrolling");
        if (disassembly.empty()) {
            ImGui::Text("No disassembly available. Load a file.");
        } else {
            for (const auto& instr : disassembly) {
                if (symbol_map.count(instr.address)) {
                    ImGui::Spacing();
                    ImGui::Text("%s:", symbol_map.at(instr.address).c_str());
                }
                ImGui::Text("  0x%04X:  %s", instr.address, instr.instruction_text.c_str());
            }
        }
        ImGui::EndChild();
        ImGui::End();


        // -- Parsed Data Display --
        ImGui::BeginChild("HexView");
        if (loaded_records.empty()) {
            ImGui::Text("No data loaded.");
        } else {
            for (const auto& record : loaded_records) {
                std::stringstream ss;
                ss << "T:" << std::hex << std::setw(2) << std::setfill('0') << (int)record.record_type
                   << "  ADDR: " << std::setw(4) << (int)record.address
                   << "  LEN: " << std::setw(2) << (int)record.byte_count << "  DATA: ";
                for (uint8_t byte : record.data) {
                    ss << std::setw(2) << static_cast<int>(byte) << " ";
                }
                ImGui::TextUnformatted(ss.str().c_str());
            }
        }
        ImGui::EndChild();
        ImGui::End();

        // File Dialog Logic for Saving file
        if(ImGuiFileDialog::Instance()->Display("SaveFileDlgKey")) {
            if(ImGuiFileDialog::Instance()->IsOk()) {
                std::string file_path = ImGuiFileDialog::Instance()->GetFilePathName();

                // the output file stream
                std::ofstream out_file(file_path);
                if (out_file.is_open()) {
                    // loop through the disassembly data and write it to the file
                    for (const auto& instr : disassembly) {
                        std::stringstream ss;
                        if (symbol_map.count(instr.address)) {
                            ss << "\n" << symbol_map.at(instr.address) << ":\n";
                        }
                        ss << "  0x" << std::hex << std::uppercase << std::setw(4) << std::setfill('0')
                           << instr.address << ":  " << instr.instruction_text << "\n";
                        
                        out_file << ss.str();
                    }
                    out_file.close();
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }

        // --- 5. Render the frame ---
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 45, 55, 60, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // --- 6. Cleanup ---
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
