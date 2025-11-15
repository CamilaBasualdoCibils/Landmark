#!/usr/bin/env bash

set -e

SCRIPT_DIR="$(cd -- "$(dirname -- "$(readlink -f -- "$0")")")" && pwd
echo "Script directory: $SCRIPT_DIR"

DEB_NAME="NVIDIA_Nsight_Graphics_2025.4.1.25250.deb"
DEB_PATH="$SCRIPT_DIR/$DEB_NAME"
DOWNLOAD_URL="https://developer.nvidia.com/downloads/assets/tools/secure/nsight-graphics/2025_4_1/linux/$DEB_NAME"

# ------------------------------------------
# If ngfx exists, skip everything
# ------------------------------------------
if command -v ngfx-ui-for-linux &> /dev/null; then
    echo "ngfx command already available. Nothing to do."
    ngfx-ui-for-linux
    exit 0
fi

echo "ngfx-ui-for-linux not found. Installing Nsight Graphics..."

# ------------------------------------------
# Ensure wget
# ------------------------------------------
if ! command -v wget &> /dev/null; then
    echo "wget missing. Installing..."
    apt-get update -y
    DEBIAN_FRONTEND=noninteractive apt-get install -y wget
fi

# ------------------------------------------
# Download .deb if missing
# ------------------------------------------
if [ ! -f "$DEB_PATH" ]; then
    echo "Downloading Nsight Graphics..."
    wget -O "$DEB_PATH" "$DOWNLOAD_URL"
else
    echo ".deb already downloaded."
fi

# ------------------------------------------
# Install the .deb
# ------------------------------------------
echo "Installing .deb..."
dpkg -i "$DEB_PATH" || true

echo "Fixing dependencies..."
DEBIAN_FRONTEND=noninteractive apt-get install -f -y

# ------------------------------------------
# Remove .deb after installation
# ------------------------------------------
echo "Removing installer..."
rm -f "$DEB_PATH"

echo "Installation complete."

# ------------------------------------------
# Recheck ngfx availability
# ------------------------------------------
if command -v ngfx-ui-for-linux &> /dev/null; then
    echo "ngfx-ui-for-linux installed successfully."
else
    echo "ngfx-ui-for-linux still not found — installation may have failed."
fi

ngfx-ui-for-linux
