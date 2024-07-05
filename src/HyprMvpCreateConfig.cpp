//
// Created by lenarus on 7/4/24.
//

#include <filesystem>
#include <string>

#include "HyprMvpCreateConfig.h"

std::string HyprMpv::homedir = getenv("HOME");

void HyprMpv::createConfig() {
    std::filesystem::create_directory(HyprMpv::homedir + "/.config/hyprmpv");
    if (!std::filesystem::exists(HyprMpv::homedir + "/.config/hyprmpv/theme.qml")) {
//        std::filesystem::remove(HyprMpv::homedir + "/.config/hyprmpv/theme.qml");
        std::filesystem::copy("../tests/theme.qml", HyprMpv::homedir + "/.config/hyprmpv");
    }
}