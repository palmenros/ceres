#include <cstdint>
#include <iostream>
#include <string>

extern "C" {
int32_t testMain();
}

// Signal a fail message and bail (exit)
static void failed(std::string const& msg)
{
    std::cerr << "Test failed: " << msg << std::endl;
    exit(1);
}

static void passed() { std::cout << "Test passed!" << std::endl; }

extern "C" {
// Define library functions for the test program

void assert(bool cond)
{
    if (!cond) {
        failed("Assertion failed");
    }
}

// Printing ints
void printI8(int8_t x)
{
    /* Print as number instead of char */
    std::cout << +x << std::endl;
}
void printI16(int16_t x) { std::cout << x << std::endl; }
void printI32(int32_t x) { std::cout << x << std::endl; }
void printI64(int64_t x) { std::cout << x << std::endl; }
void printU8(uint8_t x) { std::cout << x << std::endl; }
void printU16(uint16_t x) { std::cout << x << std::endl; }
void printU32(uint32_t x) { std::cout << x << std::endl; }
void printU64(uint64_t x) { std::cout << x << std::endl; }

// Printing floats
void printF32(float x) { std::cout << x << std::endl; }
void printF64(double x) { std::cout << x << std::endl; }
}

int main(int argc, char** argv)
{
    int32_t res = testMain();
    if (res != 0) {
        failed("testMain() returned non-zero value '" + std::to_string(res) + "'");
    }

    // If control reaches here, we have passed
    passed();
    return 0;
}