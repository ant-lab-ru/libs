#include <gtest/gtest.h>

#include "libs/static-queue.h"

#define QUEUE_TEST_MEMORY_SIZE 10
uint8_t test_uint8_t_buff[QUEUE_TEST_MEMORY_SIZE] = {0};

uint8_t test_var_for_get_ptr = 0;

int init_teat_queue() {


    StaticQueue test_queue;
    test_queue.init(sizeof(uint8_t), QUEUE_TEST_MEMORY_SIZE, test_uint8_t_buff);

    EXPECT_EQ(test_queue.get_capacity(), QUEUE_TEST_MEMORY_SIZE);
    EXPECT_EQ(test_queue.get_first_elem(), 0);
    EXPECT_EQ(test_queue.get_last_elem(), 0);
    EXPECT_EQ(test_queue.get_number_of_elem(), 0);

    return 0;
}

TEST(StaticQueueInit, base)
{
    init_teat_queue();
}