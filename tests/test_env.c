/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
#ifdef WEBHOOK_URL
    const char* webhook_url = WEBHOOK_URL;
    printf("WEBHOOK_URL is defined: %s\n", webhook_url);
    assert(webhook_url != NULL);
    assert(strlen(webhook_url) > 0);
#else
    printf("WEBHOOK_URL is NOT defined\n");
    assert(0 && "WEBHOOK_URL should be defined via CMake");
#endif
    printf("Env test passed!\n");
    return 0;
}
