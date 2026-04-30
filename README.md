# DiscordHookC

[![License](https://img.shields.io/badge/License-CC%20BY--ND%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nd/4.0/)
[![Version](https://img.shields.io/badge/Version-1.0.0.0-blue.svg)](#)
[![CMake on a single platform](https://github.com/NekoTheCatgirl/DiscordHookC/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/NekoTheCatgirl/DiscordHookC/actions/workflows/cmake-single-platform.yml)

A modern, lightweight C library for sending and managing Discord Webhooks with ease. Supporting everything from simple text messages to complex multi-embed payloads.

---

## Table of Contents
- [Features](#features)
- [Project Metadata](#project-metadata)
- [Dependencies](#dependencies)
  - [Installing Dependencies](#installing-dependencies)
- [Building the Library](#building-the-library)
  - [CMake Options](#cmake-options)
- [Quick Start](#quick-start)
  - [Simple Usage](#simple-usage)
  - [Embed Webhook Example](#embed-webhook-example)
- [Documentation](#documentation)
- [License](#license)

---

## Features
- **Easy-to-use API**: Simple functions to create and manage webhooks.
- **Full Embed Support**: Complete control over titles, descriptions, fields, colors, images, thumbnails, footers, and authors.
- **Webhook Customization**: Override the default webhook username and avatar.
- **Environment Support**: Automatic `.env` file loading for secure token management during development.
- **Memory Safe**: Designed with careful memory management (always remember to call `webhook_destroy`).
- **Doxygen Documented**: Clean and well-documented public headers.

## Project Metadata
- **Author**: Neko The Catgirl
- **Email**: [thea@nekothecatgirl.com](mailto:thea@nekothecatgirl.com)
- **Version**: 1.0.0.0
- **Description**: A robust C library for Discord Webhooks integration.

---

## Dependencies
The library relies on two main external dependencies:
1.  **libcurl**: For handling HTTP POST requests to Discord's API.
2.  **json-c**: For serializing webhook data into JSON format.

### Installing Dependencies

#### Ubuntu / Debian (apt)
```bash
sudo apt update
sudo apt install libcurl4-openssl-dev libjson-c-dev
```

#### Arch Linux (pacman)
```bash
sudo pacman -S curl json-c
```

#### Fedora / RHEL (dnf)
```bash
sudo dnf install libcurl-devel json-c-devel
```

#### macOS (Homebrew)
```bash
brew install curl json-c
```

#### Windows (vcpkg)
```bash
vcpkg install curl json-c
```

---

## Building the Library
This project uses CMake (minimum version 4.2).

```bash
# Clone the repository (if applicable)
# git clone https://github.com/NekoTheCatgirl/DiscordHookC.git
# cd DiscordHookC

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .
```

### CMake Options
You can customize the build by passing flags to `cmake`:

| Option | Description | Default |
| :--- | :--- | :--- |
| `DISHOOK_BUILD_EXAMPLES` | Build the example applications | `OFF` |
| `DISHOOK_BUILD_TESTS` | Build the test suite | `OFF` |

Example:
```bash
cmake -DDISHOOK_BUILD_EXAMPLES=ON -DDISHOOK_BUILD_TESTS=ON ..
```

---

## Quick Start

### Simple Usage
Send a simple text message with a few lines of code:

```c
#include <dishook.h>

int main() {
    const char* webhook_url = "YOUR_WEBHOOK_URL_HERE";
    
    // Create webhook data
    WebhookData *data = webhook_create();
    
    // Set message content
    set_webhook_content(data, "Hello from DiscordHookC!");
    
    // Send it!
    send_webhook(webhook_url, data);
    
    // Cleanup
    webhook_destroy(data);
    
    return 0;
}
```

### Embed Webhook Example
Leverage embeds and customization:

```c
#include <dishook.h>
#include <color.h> // For predefined color constants

int main() {
    WebhookData *data = webhook_create();
    set_webhook_username(data, "Junie Bot");
    set_webhook_avatar_url(data, "https://example.com/avatar.png");

    EmbedData *embed = embed_create();
    set_embed_title(embed, "System Status");
    set_embed_description(embed, "All systems are operational.");
    set_embed_color(embed, GREEN); // Using constants from color.h
    
    try_add_embed_field(embed, "CPU Usage", "15%", true);
    try_add_embed_field(embed, "Memory", "2.4GB / 16GB", true);
    
    try_add_webhook_embed(data, embed);
    
    send_webhook("YOUR_WEBHOOK_URL", data);
    webhook_destroy(data); // Also destroys the attached embed
    
    return 0;
}
```

---

## Documentation
The library's public API is fully documented using Doxygen. You can generate the HTML documentation by running:

```bash
# Using CMake (recommended)
cmake --build build --target doc

# Or manually
cd doc
doxygen Doxyfile
```

---

## License
This project is licensed under the **Creative Commons Attribution-NoDerivatives 4.0 International (CC BY-ND 4.0)**.
See the [LICENSE](LICENSE) file for more details.
