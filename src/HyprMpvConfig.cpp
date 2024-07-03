#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include "HyprMpvConfig.h"
#include "HyprMpvUtils.h"

std::string HyprMpv::Config::media_url;
int HyprMpv::Config::use_vulkan = 0;

QString HyprMpv::ConfigObject::get_media_url()
{
    return QString::fromStdString(Config::media_url);
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

    int option_index = 0;
    int prev_index = 1;

    while (true)
    {
        int opt = getopt_long (argc, argv, "v:h", long_options, &option_index);

        std::cout << opt << " " << optind << "\n";

        if (opt == -1)
            break;

        if (optind - prev_index > 1) {
            media_url = argv[optind - 2];

            debug ("Media: %s", media_url.data());
        }

        prev_index = optind;

        switch (opt)
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

    if (prev_index != argc) {
        media_url = argv[argc - 1];

        debug ("Media: %s", media_url.data());
    }

    return 0;
}