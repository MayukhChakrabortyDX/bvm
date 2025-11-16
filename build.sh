rm -rf ./build
mkdir ./build

DEV_MODE=false

# Check for --dev flag
for arg in "$@"; do
  if [ "$arg" == "--dev" ]; then
    DEV_MODE=true
    break
  fi
done

# Detect platform for shared library extension
UNAME_OUT="$(uname -s 2>/dev/null || echo Unknown)"
case "${UNAME_OUT}" in
    Linux*)     SHARED_EXT="so";   SHARED_FLAGS="-fPIC -shared" ;;
    Darwin*)    SHARED_EXT="dylib"; SHARED_FLAGS="-dynamiclib" ;;
    MINGW*|MSYS*|CYGWIN*)
                SHARED_EXT="dll";
                SHARED_FLAGS="-shared" ;;  # PIC not required on Windows
    *)
                echo "Unknown platform. Defaulting to Linux settings."
                SHARED_EXT="so"
                SHARED_FLAGS="-fPIC -shared"
                ;;
esac

echo "Detected platform: $UNAME_OUT"
echo "Shared library extension: .$SHARED_EXT"

if [ "$DEV_MODE" = true ]; then
    echo "Building in DEV mode..."
    gcc ./test/test_syscall.c \
      ./memory/heap.c \
      ./memory/constants.c \
      ./fibre/fibre.c \
      ./vminit/start/runner.c \
      ./engine/engine.c \
      ./logger/logger.c \
      ./vminit/parser/parser.c \
      ./syscall/loader.c \
      -o ./build/bliss   # Example: dev flags

    # --- PLATFORM-AWARE SHARED LIBRARY ---
    gcc $SHARED_FLAGS -o ./build/println_dev.$SHARED_EXT ./syscall/libs/stdio.c

else
    echo "Building for PRODUCTION..."
    gcc main.c -O2 -o ./build/bliss       # Production flags

    # --- PLATFORM-AWARE SHARED LIBRARY ---
    gcc $SHARED_FLAGS -o ./build/println.$SHARED_EXT ./syscall/libs/stdio.c
fi
