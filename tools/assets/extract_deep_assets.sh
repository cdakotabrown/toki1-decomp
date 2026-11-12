#!/bin/bash
# Extract all PS1 assets from Tokimeki Memorial disc
# Uses jPSXdec to scan unreferenced CD sectors
#
# WORKING COMMAND LINE SYNTAX (January 2026):
# - Index: java -jar jpsxdec.jar -f <disc> -x <index>
# - Extract by type: java -jar jpsxdec.jar -x <index> -a <type> -dir <output>
#   where <type> is: video, audio, image, or file
#
# Results (PlayStation the Best version - SLPM_86.053):
# - 14,558 XA audio files (~1.9 GB as WAV)
# - 2 video files (KONAMIC3.STR Konami logo, OPENXA.STR opening movie)
# - 16 PNG images from 1 TIM file (256x256 font atlas)
# - 4 file entries (SYSTEM.CNF, executable, 2 STR raw files)

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$(dirname "$SCRIPT_DIR")")"
DISC_PATH="${DISC_PATH:-$PROJECT_ROOT/disks/Tokimeki Memorial - Forever with You (Japan) (Rev 1) (Track 1).bin}"
OUTPUT_DIR="$PROJECT_ROOT/assets/deep_scan"
INDEX_FILE="$PROJECT_ROOT/assets/jpsxdec_index.idx"
JPSXDEC_JAR="${JPSXDEC_JAR:-}"

echo "========================================"
echo "   Deep Asset Extraction with jPSXdec"
echo "========================================"
echo ""

# Check if Java is installed
if ! command -v java &> /dev/null; then
    echo "ERROR: Java is not installed!"
    echo ""
    echo "Please install Java first:"
    echo "  sudo apt update"
    echo "  sudo apt install -y default-jre"
    echo ""
    echo "Or see docs/JAVA-INSTALLATION-GUIDE.md for other methods"
    echo ""
    exit 1
fi

# Check Java version
JAVA_VERSION=$(java -version 2>&1 | head -n 1 | cut -d'"' -f2 | cut -d'.' -f1)
if [ "$JAVA_VERSION" -lt 8 ] 2>/dev/null; then
    echo "ERROR: Java 8 or higher is required (you have version $JAVA_VERSION)"
    echo ""
    echo "Please upgrade Java:"
    echo "  sudo apt update"
    echo "  sudo apt install -y default-jre"
    exit 1
fi

if [ -z "$JPSXDEC_JAR" ] || [ ! -f "$JPSXDEC_JAR" ]; then
    echo "ERROR: jPSXdec is not installed."
    echo ""
    echo "Download it from https://github.com/m35/jpsxdec/releases, then set:"
    echo "  export JPSXDEC_JAR=/path/to/jpsxdec.jar"
    exit 1
fi

echo "[OK] Java version: $(java -version 2>&1 | head -n 1)"
echo ""

# Check if disc image exists
if [ ! -f "$DISC_PATH" ]; then
    echo "ERROR: Disc image not found!"
    echo "Expected: $DISC_PATH"
    exit 1
fi

echo "[OK] Disc image: $(basename "$DISC_PATH")"
echo "  Size: $(du -h "$DISC_PATH" | cut -f1)"
echo ""

# Create output directories
mkdir -p "$OUTPUT_DIR/audio"
mkdir -p "$OUTPUT_DIR/video"
mkdir -p "$OUTPUT_DIR/images"
mkdir -p "$OUTPUT_DIR/files"

# Step 1: Create index if it doesn't exist
if [ ! -f "$INDEX_FILE" ]; then
    echo "Step 1: Indexing disc for all assets..."
    echo "  This may take a few minutes..."
    echo ""

    java -jar "$JPSXDEC_JAR" \
        -f "$DISC_PATH" \
        -x "$INDEX_FILE"

    echo ""
    echo "[OK] Disc indexed: $INDEX_FILE"
else
    echo "Step 1: Using existing index: $INDEX_FILE"
fi
echo ""

# Count items in index
TOTAL_COUNT=$(grep -c "^#:" "$INDEX_FILE" 2>/dev/null || echo "0")
echo "Found $TOTAL_COUNT indexed items"
echo ""

# Step 2: Extract by type
echo "Step 2: Extracting assets..."
echo ""

# Extract video files (STR)
echo "  [2a] Extracting video files..."
java -jar "$JPSXDEC_JAR" \
    -x "$INDEX_FILE" \
    -a video \
    -dir "$OUTPUT_DIR/video" \
    -vidfmt avi:mjpg
echo ""

# Extract image files (TIM)
echo "  [2b] Extracting image files..."
java -jar "$JPSXDEC_JAR" \
    -x "$INDEX_FILE" \
    -a image \
    -dir "$OUTPUT_DIR/images"
echo ""

# Extract file entries (SYSTEM.CNF, executable, etc.)
echo "  [2c] Extracting file entries..."
java -jar "$JPSXDEC_JAR" \
    -x "$INDEX_FILE" \
    -a file \
    -dir "$OUTPUT_DIR/files"
echo ""

# Extract audio files (XA) - this takes the longest
echo "  [2d] Extracting audio files (this may take several minutes)..."
java -jar "$JPSXDEC_JAR" \
    -x "$INDEX_FILE" \
    -a audio \
    -dir "$OUTPUT_DIR/audio"
echo ""

echo "========================================"
echo "   Extraction Complete!"
echo "========================================"
echo ""
echo "Assets extracted to: $OUTPUT_DIR"
echo ""
echo "Summary:"
echo "  Audio:  $(ls "$OUTPUT_DIR/audio/" 2>/dev/null | wc -l) files ($(du -sh "$OUTPUT_DIR/audio/" 2>/dev/null | cut -f1))"
echo "  Video:  $(ls "$OUTPUT_DIR/video/" 2>/dev/null | wc -l) files ($(du -sh "$OUTPUT_DIR/video/" 2>/dev/null | cut -f1))"
echo "  Images: $(ls "$OUTPUT_DIR/images/" 2>/dev/null | wc -l) files ($(du -sh "$OUTPUT_DIR/images/" 2>/dev/null | cut -f1))"
echo "  Files:  $(ls "$OUTPUT_DIR/files/" 2>/dev/null | wc -l) files ($(du -sh "$OUTPUT_DIR/files/" 2>/dev/null | cut -f1))"
echo ""
echo "  Total:  $(du -sh "$OUTPUT_DIR/" | cut -f1)"
echo ""
echo "For catalog details, see: docs/EXTRACTED-ASSETS-CATALOG.md"
echo ""
