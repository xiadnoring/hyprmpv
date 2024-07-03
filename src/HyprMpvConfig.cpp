#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <filesystem>
#include <regex>
#include "HyprMpvConfig.h"
#include "HyprMpvUtils.h"

namespace fs = std::filesystem;

std::string HyprMpv::Config::media_url;
int HyprMpv::Config::use_vulkan = 0;

QString HyprMpv::ConfigObject::get_media_url()
{
    return QString::fromStdString(Config::media_url);
}

bool HyprMpv::Config::resolve_url(std::string &url)
{
    std::regex url_regex (R"(^(https?|ftp|file)://[-\w0-9+&@#/%?=~_|!:,.;]*[-\w0-9+&@#/%=~_|]$)");

    if (std::regex_match(url, url_regex))
        return true;

    fs::path p (url);

    p = fs::absolute(p);

    url = p.string();

    return fs::exists(p) && !fs::is_directory(p);
}

int HyprMpv::Config::setup (int argc, char *argv[])
{
    opterr = 0;

    static struct option long_options[] = {
        {"use-vulkan", no_argument, &use_vulkan, 1},
        {"no-use-vulkan", no_argument, &use_vulkan, 0},
        {"help", no_argument, nullptr, 'h'},
        {"video", required_argument, nullptr, 'v'}
    };

    int prev_index = 1;

    while (true)
    {
        int opt = getopt_long (argc, argv, "v:h", long_options, nullptr);

        if (opt == -1)
            break;

        // detect no option
        if (optind - prev_index > 1)
        {
            media_url = argv[optind - 2];
        }

        prev_index = optind;

        switch ((char) opt)
        {
            case '?':
            case 'h':
                printf("Hyprland Video Player\n");
                printf(" -h - Help\n");
                printf(" -v [URL] - Video URL\n");

                return -1;
            case 'v':
                media_url = optarg;

                debug ("Video: %s", media_url.data());

                continue;
        }
    }

    // detect no option
    if (prev_index != argc)
    {
        media_url = argv[argc - 1];
    }

    if (!resolve_url(media_url))
    {
        std::cerr << "Invalid Media URL" << '\n';

        return -1;
    }

    debug ("Media: %s", media_url.data());

    return 0;
}