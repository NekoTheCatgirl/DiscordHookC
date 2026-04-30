/*
 * DiscordHookC
 * Author: Neko The Catgirl (thea@nekothecatgirl.com)
 * Copyright (c) 2026 Neko The Catgirl
 * Description: A C library for Discord Webhooks
 * License: CC Attribution no derivatives (CC BY-ND 4.0)
 */

#ifndef DISCORDHOOKC_DISHOOK_PRIVATE_H
#define DISCORDHOOKC_DISHOOK_PRIVATE_H

typedef struct EmbedFooterData {
    char *text;
    char *icon_url;
} EmbedFooterData;

typedef struct EmbedMultimediaData {
    char *url;
    int height;
    int width;
} EmbedMultimediaData;

typedef struct EmbedProviderData {
    char *name;
    char *url;
} EmbedProviderData;

typedef struct EmbedAuthorData {
    char *name;
    char *url;
    char *icon_url;
} EmbedAuthorData;

typedef struct EmbedFieldData {
    char *name;
    char *value;
    bool inl;
} EmbedFieldData;

#endif //DISCORDHOOKC_DISHOOK_PRIVATE_H
