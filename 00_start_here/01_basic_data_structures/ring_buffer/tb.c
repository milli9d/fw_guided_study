#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "ring_buffer.h"

#define TEST_PASS printf("  ✓ PASS\n")
#define TEST_FAIL printf("  ✗ FAIL\n")
#define TEST(name) printf("\n%s\n", name)

int test_count = 0;
int pass_count = 0;

void assert_equal_int(int expected, int actual, const char* msg) {
    test_count++;
    if (expected == actual) {
        printf("    %s: %d == %d ", msg, expected, actual);
        TEST_PASS;
        pass_count++;
    } else {
        printf("    %s: %d != %d ", msg, expected, actual);
        TEST_FAIL;
    }
}

void assert_equal_size(size_t expected, size_t actual, const char* msg) {
    test_count++;
    if (expected == actual) {
        printf("    %s: %zu == %zu ", msg, expected, actual);
        TEST_PASS;
        pass_count++;
    } else {
        printf("    %s: %zu != %zu ", msg, expected, actual);
        TEST_FAIL;
    }
}

void assert_true(int condition, const char* msg) {
    test_count++;
    if (condition) {
        printf("    %s: true ", msg);
        TEST_PASS;
        pass_count++;
    } else {
        printf("    %s: false ", msg);
        TEST_FAIL;
    }
}

void assert_mem_equal(uint8_t* expected, uint8_t* actual, size_t len, const char* msg) {
    test_count++;
    if (memcmp(expected, actual, len) == 0) {
        printf("    %s: memory match ", msg);
        TEST_PASS;
        pass_count++;
    } else {
        printf("    %s: memory mismatch ", msg);
        TEST_FAIL;
    }
}

// ============================================================================
// INITIALIZATION TESTS
// ============================================================================

void test_init_valid() {
    TEST("TC1.1: Valid Initialization");
    ring_buffer_t rb;
    int rc = init_ring_buffer(&rb, 32);
    
    assert_equal_int(0, rc, "init returns 0");
    assert_equal_size(32, rb.cap, "capacity is 32");
    assert_equal_size(0, rb.size, "size is 0");
    assert_equal_size(0, rb.in, "in pointer is 0");
    assert_equal_size(0, rb.out, "out pointer is 0");
    assert_true(rb.data != NULL, "data pointer allocated");
    
    free(rb.data);
}

void test_init_large_size() {
    TEST("TC1.3: Large Size Buffer");
    ring_buffer_t rb;
    int rc = init_ring_buffer(&rb, 65536);
    
    assert_equal_int(0, rc, "init returns 0");
    assert_equal_size(65536, rb.cap, "capacity is 65536");
    assert_true(rb.data != NULL, "data pointer allocated");
    
    free(rb.data);
}

// ============================================================================
// FREE SPACE TESTS
// ============================================================================

void test_free_space_empty() {
    TEST("TC2.1: Free Space in Empty Buffer");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    size_t free_space = ring_buffer_free_space(&rb);
    assert_equal_size(32, free_space, "free space is 32");
    
    free(rb.data);
}

void test_free_space_after_put() {
    TEST("TC2.2: Free Space After Put");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    ring_buffer_put(&rb, data, 10);
    
    size_t free_space = ring_buffer_free_space(&rb);
    assert_equal_size(22, free_space, "free space is 22");
    
    free(rb.data);
}

void test_free_space_full() {
    TEST("TC2.3: Free Space in Full Buffer");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[32] = {0};
    for (int i = 0; i < 32; i++) data[i] = i;
    ring_buffer_put(&rb, data, 32);
    
    size_t free_space = ring_buffer_free_space(&rb);
    assert_equal_size(0, free_space, "free space is 0");
    
    free(rb.data);
}

void test_free_space_after_put_get() {
    TEST("TC2.4: Free Space After Put and Get");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    ring_buffer_put(&rb, data, 10);
    
    uint8_t out[5] = {0};
    ring_buffer_get(&rb, out, 5);
    
    size_t free_space = ring_buffer_free_space(&rb);
    assert_equal_size(27, free_space, "free space is 27");
    
    free(rb.data);
}

// ============================================================================
// PUT TESTS
// ============================================================================

void test_put_single_byte() {
    TEST("TC3.1: Put Single Byte");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[1] = {0x42};
    int rc = ring_buffer_put(&rb, data, 1);
    
    assert_equal_int(0, rc, "put returns 0");
    assert_equal_size(1, rb.size, "size is 1");
    assert_equal_size(1, rb.in, "in pointer is 1");
    
    free(rb.data);
}

void test_put_multiple_bytes() {
    TEST("TC3.2: Put Multiple Bytes");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[3] = {0x01, 0x02, 0x03};
    int rc = ring_buffer_put(&rb, data, 3);
    
    assert_equal_int(0, rc, "put returns 0");
    assert_equal_size(3, rb.size, "size is 3");
    assert_equal_size(3, rb.in, "in pointer is 3");
    
    free(rb.data);
}

void test_put_multiple_times() {
    TEST("TC3.3: Put Multiple Times");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data1[2] = {0x01, 0x02};
    uint8_t data2[2] = {0x03, 0x04};
    
    int rc1 = ring_buffer_put(&rb, data1, 2);
    int rc2 = ring_buffer_put(&rb, data2, 2);
    
    assert_equal_int(0, rc1, "first put returns 0");
    assert_equal_int(0, rc2, "second put returns 0");
    assert_equal_size(4, rb.size, "size is 4");
    
    free(rb.data);
}

void test_put_exceeds_capacity() {
    TEST("TC3.4: Put Exceeds Capacity");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data1[20];
    for (int i = 0; i < 20; i++) data1[i] = i;
    ring_buffer_put(&rb, data1, 20);
    
    uint8_t data2[15];
    for (int i = 0; i < 15; i++) data2[i] = i;
    int rc = ring_buffer_put(&rb, data2, 15);
    
    assert_equal_int(-12, rc, "put returns -ENOMEM");
    assert_equal_size(20, rb.size, "size unchanged at 20");
    
    free(rb.data);
}

void test_put_exactly_fills_buffer() {
    TEST("TC3.5: Put Exactly Fills Buffer");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[32];
    for (int i = 0; i < 32; i++) data[i] = i;
    int rc = ring_buffer_put(&rb, data, 32);
    
    assert_equal_int(0, rc, "put returns 0");
    assert_equal_size(32, rb.size, "size is 32");
    assert_equal_size(0, ring_buffer_free_space(&rb), "free space is 0");
    
    free(rb.data);
}

void test_put_zero_bytes() {
    TEST("TC3.7: Put Zero Bytes");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    int rc = ring_buffer_put(&rb, NULL, 0);
    
    assert_equal_int(0, rc, "put returns 0");
    assert_equal_size(0, rb.size, "size is 0");
    
    free(rb.data);
}

// ============================================================================
// GET TESTS
// ============================================================================

void test_get_single_byte() {
    TEST("TC4.1: Get Single Byte");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_in[1] = {0x42};
    ring_buffer_put(&rb, data_in, 1);
    
    uint8_t data_out[1] = {0};
    int rc = ring_buffer_get(&rb, data_out, 1);
    
    assert_equal_int(1, rc, "get returns 1");
    assert_equal_int(0x42, data_out[0], "retrieved byte is 0x42");
    assert_equal_size(0, rb.size, "size is 0");
    
    free(rb.data);
}

void test_get_multiple_bytes() {
    TEST("TC4.2: Get Multiple Bytes");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_in[3] = {0x01, 0x02, 0x03};
    ring_buffer_put(&rb, data_in, 3);
    
    uint8_t data_out[3] = {0};
    int rc = ring_buffer_get(&rb, data_out, 3);
    
    assert_equal_int(3, rc, "get returns 3");
    assert_mem_equal(data_in, data_out, 3, "retrieved bytes match");
    assert_equal_size(0, rb.size, "size is 0");
    
    free(rb.data);
}

void test_get_more_than_available() {
    TEST("TC4.3: Get More Bytes Than Available");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_in[2] = {0x01, 0x02};
    ring_buffer_put(&rb, data_in, 2);
    
    uint8_t data_out[10] = {0};
    int rc = ring_buffer_get(&rb, data_out, 10);
    
    assert_equal_int(2, rc, "get returns 2");
    assert_equal_size(0, rb.size, "size is 0");
    
    free(rb.data);
}

void test_get_from_empty() {
    TEST("TC4.4: Get From Empty Buffer");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_out[5] = {0};
    int rc = ring_buffer_get(&rb, data_out, 5);
    
    assert_equal_int(0, rc, "get returns 0");
    
    free(rb.data);
}

void test_get_zero_bytes() {
    TEST("TC4.5: Get Zero Bytes");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_in[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    ring_buffer_put(&rb, data_in, 5);
    
    uint8_t data_out[5] = {0};
    int rc = ring_buffer_get(&rb, data_out, 0);
    
    assert_equal_int(0, rc, "get returns 0");
    assert_equal_size(5, rb.size, "size unchanged at 5");
    
    free(rb.data);
}

void test_get_null_pointer() {
    TEST("TC4.8: NULL Pointer Checks");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data[1] = {0x01};
    ring_buffer_put(&rb, data, 1);
    
    int rc1 = ring_buffer_get(NULL, data, 1);
    int rc2 = ring_buffer_get(&rb, NULL, 1);
    
    assert_equal_int(-22, rc1, "get(NULL, ...) returns -EINVAL");
    assert_equal_int(-22, rc2, "get(..., NULL) returns -EINVAL");
    
    free(rb.data);
}

// ============================================================================
// FIFO ORDERING TESTS
// ============================================================================

void test_fifo_ordering() {
    TEST("TC5.1: FIFO Ordering");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data_in[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    ring_buffer_put(&rb, data_in, 5);
    
    uint8_t data_out[5] = {0};
    ring_buffer_get(&rb, data_out, 5);
    
    assert_mem_equal(data_in, data_out, 5, "FIFO order preserved");
    
    free(rb.data);
}

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

void test_put_get_put_cycle() {
    TEST("TC7.1: Put-Get-Put Cycle");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    uint8_t data1[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    ring_buffer_put(&rb, data1, 5);
    
    uint8_t out1[3] = {0};
    ring_buffer_get(&rb, out1, 3);
    assert_equal_size(2, rb.size, "size is 2 after partial get");
    
    uint8_t data2[2] = {0x06, 0x07};
    ring_buffer_put(&rb, data2, 2);
    assert_equal_size(4, rb.size, "size is 4 after second put");
    
    uint8_t out2[4] = {0};
    int rc = ring_buffer_get(&rb, out2, 4);
    assert_equal_int(4, rc, "get returns 4");
    
    free(rb.data);
}

void test_fill_empty_refill() {
    TEST("TC7.4: Fill, Empty, Refill Cycle");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    // Fill completely
    uint8_t data_full[32];
    for (int i = 0; i < 32; i++) data_full[i] = i;
    ring_buffer_put(&rb, data_full, 32);
    assert_equal_size(32, rb.size, "size is 32 after fill");
    
    // Empty completely
    uint8_t out_full[32] = {0};
    ring_buffer_get(&rb, out_full, 32);
    assert_equal_size(0, rb.size, "size is 0 after empty");
    
    // Refill to half capacity
    uint8_t data_half[16];
    for (int i = 0; i < 16; i++) data_half[i] = i + 100;
    ring_buffer_put(&rb, data_half, 16);
    assert_equal_size(16, rb.size, "size is 16 after refill");
    assert_equal_size(16, ring_buffer_free_space(&rb), "free space is 16");
    
    free(rb.data);
}

void test_alternating_put_get() {
    TEST("TC7.3: Alternating Put-Get");
    ring_buffer_t rb;
    init_ring_buffer(&rb, 32);
    
    for (int i = 0; i < 10; i++) {
        uint8_t data_in[1] = {(uint8_t)i};
        ring_buffer_put(&rb, data_in, 1);
        
        uint8_t data_out[1] = {0};
        int rc = ring_buffer_get(&rb, data_out, 1);
        
        if (rc != 1 || data_out[0] != i) {
            printf("    Cycle %d failed\n", i);
            free(rb.data);
            return;
        }
    }
    
    assert_equal_size(0, rb.size, "size is 0 after cycles");
    printf("    Alternating put-get successful ");
    TEST_PASS;
    pass_count++;
    test_count++;
    
    free(rb.data);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================

int main() {
    printf("=====================================\n");
    printf("   RING BUFFER TEST SUITE\n");
    printf("=====================================\n");

    // Initialization Tests
    printf("\n--- INITIALIZATION TESTS ---\n");
    test_init_valid();
    test_init_large_size();

    // Free Space Tests
    printf("\n--- FREE SPACE TESTS ---\n");
    test_free_space_empty();
    test_free_space_after_put();
    test_free_space_full();
    test_free_space_after_put_get();

    // Put Tests
    printf("\n--- PUT TESTS ---\n");
    test_put_single_byte();
    test_put_multiple_bytes();
    test_put_multiple_times();
    test_put_exceeds_capacity();
    test_put_exactly_fills_buffer();
    test_put_zero_bytes();

    // Get Tests
    printf("\n--- GET TESTS ---\n");
    test_get_single_byte();
    test_get_multiple_bytes();
    test_get_more_than_available();
    test_get_from_empty();
    test_get_zero_bytes();
    test_get_null_pointer();

    // FIFO Tests
    printf("\n--- FIFO ORDERING TESTS ---\n");
    test_fifo_ordering();

    // Integration Tests
    printf("\n--- INTEGRATION TESTS ---\n");
    test_put_get_put_cycle();
    test_fill_empty_refill();
    test_alternating_put_get();

    // Summary
    printf("\n=====================================\n");
    printf("   TEST SUMMARY\n");
    printf("=====================================\n");
    printf("Total Tests: %d\n", test_count);
    printf("Passed:      %d\n", pass_count);
    printf("Failed:      %d\n", test_count - pass_count);
    printf("Success:     %.1f%%\n", (float)pass_count / test_count * 100);
    printf("=====================================\n\n");

    return (test_count == pass_count) ? 0 : 1;
}