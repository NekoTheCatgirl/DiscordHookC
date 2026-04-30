/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#include <dishook.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
#ifdef WEBHOOK_URL
    const char* webhook_url = WEBHOOK_URL;
#else
    const char* webhook_url = getenv("WEBHOOK_URL");
#endif
    if (webhook_url == nullptr) {
        fprintf(stderr, "Error: WEBHOOK_URL environment variable is not set\n");
        return 1;
    }

    WebhookData *data = webhook_create();
    set_webhook_username(data, "Junie Bot");
    set_webhook_content(data, "This is a complex webhook message with an embed!");

    EmbedData *embed = embed_create();
    set_embed_title(embed, "Embed Title");
    set_embed_description(embed, "This is the description of the embed.");
    set_embed_color(embed, 0x00FF00); // Green
    set_embed_footer(embed, "Footer Text", nullptr);
    try_add_embed_field(embed, "Field 1", "Value 1", true);
    try_add_embed_field(embed, "Field 2", "Value 2", true);

    if (!try_add_webhook_embed(data, embed)) {
        fprintf(stderr, "Failed to add embed to webhook\n");
        embed_destroy(embed);
    }

    printf("Sending complex webhook...\n");
    send_webhook(webhook_url, data);

    webhook_destroy(data);
    printf("Done.\n");

    return 0;
}
