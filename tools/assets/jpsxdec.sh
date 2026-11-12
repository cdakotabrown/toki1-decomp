#!/bin/bash
# Wrapper for an externally installed jPSXdec.
# Requires Java 8 or higher. Set JPSXDEC_JAR to the release's jpsxdec.jar.

JAR_PATH="${JPSXDEC_JAR:-}"

if [ -z "$JAR_PATH" ] || [ ! -f "$JAR_PATH" ]; then
    echo "ERROR: jPSXdec is not installed."
    echo ""
    echo "Download it from https://github.com/m35/jpsxdec/releases, then set:"
    echo "  export JPSXDEC_JAR=/path/to/jpsxdec.jar"
    exit 1
fi

if ! command -v java &> /dev/null; then
    echo "ERROR: Java is not installed!"
    echo ""
    echo "Please install Java 8 or higher:"
    echo "  sudo apt update"
    echo "  sudo apt install -y default-jre"
    echo ""
    echo "Or download from: https://adoptium.net/temurin/releases/"
    exit 1
fi

exec java -jar "$JAR_PATH" "$@"
