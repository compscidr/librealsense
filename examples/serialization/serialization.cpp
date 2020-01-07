#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering

#include <imgui.h>
#include "imgui_impl_glfw.h"

int main(int argc, char * argv[]) {
    window app(1280, 720, "Serialization Example");
    ImGui_ImplGlfw_Init(app, false);

    rs2::pipeline pipe;
    rs2::config cfg;
    rs2::colorizer c;                     // Helper to colorize depth images
    texture depth_image, color_image;     // Helpers for renderig images
    float       alpha = 0.5f;               // Transparancy coefficient
    cfg.enable_stream(RS2_STREAM_DEPTH);
    cfg.enable_stream(RS2_STREAM_COLOR);
    pipe.start(cfg);

    while(app) {
        // Flags for displaying ImGui window
        static const int flags = ImGuiWindowFlags_NoCollapse
                                 | ImGuiWindowFlags_NoScrollbar
                                 | ImGuiWindowFlags_NoSavedSettings
                                 | ImGuiWindowFlags_NoTitleBar
                                 | ImGuiWindowFlags_NoResize
                                 | ImGuiWindowFlags_NoMove;

        ImGui_ImplGlfw_NewFrame(1);
        ImGui::SetNextWindowSize({app.width(), app.height()});
        ImGui::Begin("app", nullptr, flags);

        rs2::frameset frameset = pipe.wait_for_frames();

        auto depth = frameset.get_depth_frame();
        auto color = frameset.get_color_frame();
        auto colorized_depth = c.colorize(depth);

        glColor4f(1.f, 1.f, 1.f, 1.f);
        glDisable(GL_BLEND);

        // color_image.render(color, { 0, 0, app.width(), app.height() });
        depth_image.render(colorized_depth, { 0, 0, app.width(), app.height() }, 1 - alpha);

        ImGui_ImplGlfw_NewFrame(1);

        //ImGui::End();
        ImGui::Render();
    }

    return 0;
}