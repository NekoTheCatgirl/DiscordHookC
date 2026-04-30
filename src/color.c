/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <color.h>
#include <ctype.h>
#include <string.h>

int from_hex(const char* hex_color) {
    const size_t len = strlen(hex_color);
    int base = 1;
    int dec_val = 0;

    for (int i = (int)len - 1; i >= 0; i--) {
        const char c = (char)toupper((unsigned char)hex_color[i]);
        if (isdigit(c)) {
            dec_val += ((int)c - (int)'0') * base;
            base *= 16;
        } else if (isascii(c) && isupper(c)) {
            dec_val += ((int)c - (int)'A' + 10) * base;
            base *= 16;
        }
    }

    return dec_val;
}
