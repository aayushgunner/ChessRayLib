df!/bin/bash

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <source_file>"
    exit 1
fi

# Input filename
SRC="$1"

# Output executable
OUTPUT="${SRC%.*}"
# Compile source file with Raylib libraries linked
gcc -ggdb $SRC -o $OUTPUT -lraylib -lm -lpthread -ldl -lrt -lX11

# Run the resulting executable
./$OUTPUT

