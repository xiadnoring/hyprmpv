#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <filesystem>
#include <regex>
#include <QProcessEnvironment>

#include "HyprMpvConfig.h"
#include "HyprMpvUtils.h"

namespace fs = std::filesystem;

std::string HyprMpv::Config::media_url;
int HyprMpv::Config::use_vulkan = 0;
int HyprMpv::Config::media_backend = HYPR_MPV_MEDIA_BACKEND_FFMPEG;
int HyprMpv::Config::custom_qml = 1;

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
        {"use-gstreamer", no_argument, &media_backend, HYPR_MPV_MEDIA_BACKEND_GSTREAMER},
        {"use-ffmpeg", no_argument, &media_backend, HYPR_MPV_MEDIA_BACKEND_FFMPEG},
        {"help", no_argument, nullptr, 'h'},
        {"video", required_argument, nullptr, 'v'},
        {"no-custom-qml", no_argument, &custom_qml, 0}
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
                printf("Usage: hyprmpv [options] [arguments]\n");

                printf("Options:\n");
                printf("    -h, --help            Show this help message and exit\n");
                printf("    -v, --video <file>    Specify video file\n");
                printf("    --use-vulkan          Say to use Vulkan Instance\n");
                printf("    --no-use-vulkan       Say to don't use Vulkan API\n");
                printf("    --use-gstreamer       Say to use GStreamer as Media Backend\n");
                printf("    --use-ffmpeg          Say to use FFmpeg as Media Backend\n");
                printf("    --no-custom-qml       Say to don't use custom qml scheme\n");

                printf("\nExamples:\n");
                printf("    hyprmpv --use-vulkan video.mp4\n");
                printf("    hyprmpv video.webm --use-gstreamer --no-use-vulkan\n");

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

    std::string media_backend_string;

    switch (media_backend) {
        case HYPR_MPV_MEDIA_BACKEND_GSTREAMER:
            media_backend_string = "gstreamer";
            break;
        case HYPR_MPV_MEDIA_BACKEND_FFMPEG:
            media_backend_string = "ffmpeg";
            break;
        default:
            exit (0);
    }

    qputenv("QT_MEDIA_BACKEND", media_backend_string);

    return 0;
}