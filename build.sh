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

if [ "$DEV_MODE" = true ]; then
    echo "Building in DEV mode..."
    gcc ./test/test_syscall.c \
      ./memory/heap.c \
      ./memory/constants.c \
      ./fibre/fibre.c \
      ./vminit/start/runner.c ./engine/engine.c \
      ./logger/logger.c \
      ./vminit/parser/parser.c \
      ./syscall/loader.c \
      -o ./build/bliss   # Example: dev flags
    gcc -fPIC -shared -o ./build/println_dev.so ./syscall/libs/stdio.c
else
    echo "Building for PRODUCTION..."
    gcc main.c -O2 -o ./build/bliss       # Production flags
    gcc -fPIC -shared -o ./build/println.so ./syscall/libs/stdio.c
fi