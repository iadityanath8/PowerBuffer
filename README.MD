# PowerBuffer

PowerBuffer is a production-grade implementation of the gap buffer data structure in C. It provides efficient insertion, deletion, and manipulation of character sequences with low overhead.

## Features

- Efficient insertion and deletion of characters at any position in the buffer.
- Fast cursor movement operations.
- Memory-efficient implementation using a gap buffer approach.
- Supports dynamic resizing of the buffer as needed.
- Designed for production use with robust error handling and testing capabilities.

## Usage

To use the PowerBuffer library in your C project, follow these steps:

1. Include the `powerbuffer.h` header file in your source files.
2. Use the provided functions to initialize, manipulate, and free the buffer as needed.

## Contributing

Contributions to the PowerBuffer project are welcome! Feel free to submit bug reports, feature requests, or pull requests on the GitHub repository.


Example usage:

```c
#include "powerbuffer.h"

int main() {
    // Initialize a PowerBuffer with an initial size
    PowerBuffer buffer = power_init(10);

    // Insert characters into the buffer
    p_append_char(&buffer, 'H');
    p_append_char(&buffer, 'e');
    p_append_char(&buffer, 'l');
    p_append_char(&buffer, 'l');
    p_append_char(&buffer, 'o');

    // Insert a string into the buffer
    p_append_string(&buffer, " World!");

    // Print the contents of the buffer
    printf("%s\n", buffer.buffer);

    // Free the memory allocated for the buffer
    free(buffer.buffer);

    return 0;
}