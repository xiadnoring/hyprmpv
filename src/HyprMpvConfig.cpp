#include <unistd.h>
#include <iostream>

#include "HyprMpvConfig.h"
#include "HyprMpvUtils.h"

std::string HyprMpv::media_url;

QString HyprMpv::ConfigObject::get_media_url()
{
    return QString::fromStdString(media_url);
}

int HyprMpv::Config::setup (int argc, char *argv[])
{
    int opt;

    opterr = 0;

    while (opt = getopt (argc, argv, "v:h"))
    {
        if (opt == -1)
            break;

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

    return 0;
}