#include "ViewTestFixture.h"

class ViewApiTest : public ViewTestBase {
};

TEST_F(ViewApiTest, createContentView_InvalidInput)
{
    PContentView pContentView;

    EXPECT_TRUE(STATUS_FAILED(createContentView(MIN_CONTENT_VIEW_ITEMS, MIN_CONTENT_VIEW_BUFFER_DURATION + 1, NULL, 0, &pContentView)));
    EXPECT_TRUE(STATUS_FAILED(createContentView(MIN_CONTENT_VIEW_ITEMS + 1, MIN_CONTENT_VIEW_BUFFER_DURATION, NULL, 0, &pContentView)));
    EXPECT_TRUE(STATUS_FAILED(createContentView(MIN_CONTENT_VIEW_ITEMS, MIN_CONTENT_VIEW_BUFFER_DURATION, NULL, 0, &pContentView)));
    EXPECT_TRUE(STATUS_FAILED(createContentView(MIN_CONTENT_VIEW_ITEMS + 1, MIN_CONTENT_VIEW_BUFFER_DURATION + 1, NULL, 0, NULL)));
}

TEST_F(ViewApiTest, freeContentView_NullPointer)
{
    PContentView pContentView;

    // Create a valid view
    EXPECT_TRUE(STATUS_SUCCEEDED(createContentView(MIN_CONTENT_VIEW_ITEMS + 1, MIN_CONTENT_VIEW_BUFFER_DURATION + 1, NULL, 0, &pContentView)));

    // Free the view
    EXPECT_TRUE(STATUS_SUCCEEDED(freeContentView(pContentView)));

    // The call is idempotent
    EXPECT_TRUE(STATUS_SUCCEEDED(freeContentView(NULL)));
}

TEST_F(ViewApiTest, contentViewItemExists_NullPointer)
{
    BOOL exists;

    EXPECT_TRUE(STATUS_FAILED(contentViewItemExists(NULL, 0, &exists)));
    EXPECT_TRUE(STATUS_FAILED(contentViewItemExists(NULL, 0, NULL)));
    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewItemExists(mContentView, 0, NULL)));

    // Shouldn't exist
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewItemExists(mContentView, 0, &exists)));
    EXPECT_FALSE(exists);
}

TEST_F(ViewApiTest, contentViewRemoveAll_NullPointer)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewRemoveAll(NULL)));
}

TEST_F(ViewApiTest, contentViewGetItemAt_NullPointer)
{
    PViewItem pViewItem;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemAt(NULL, 0, &pViewItem)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemAt(NULL, 0, NULL)));
    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemAt(mContentView, 0, NULL)));

    // Shouldn't exist
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemAt(mContentView, 0, &pViewItem)));
}

TEST_F(ViewApiTest, contentViewGetItemWithTimestamp_NullPointer)
{
    PViewItem pViewItem;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemWithTimestamp(NULL, 0, &pViewItem)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemWithTimestamp(NULL, 0, NULL)));
    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemWithTimestamp(mContentView, 0, NULL)));

    // Shouldn't exist
    EXPECT_TRUE(STATUS_FAILED(contentViewGetItemWithTimestamp(mContentView, 0, &pViewItem)));
}

TEST_F(ViewApiTest, contentViewTrimTail_NullPointer)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewTrimTail(NULL, 0)));
    CreateContentView();
}

TEST_F(ViewApiTest, contentViewTimestampInRange_NullPointer)
{
    BOOL inRange;

    EXPECT_TRUE(STATUS_FAILED(contentViewTimestampInRange(NULL, 0, &inRange)));
    EXPECT_TRUE(STATUS_FAILED(contentViewTimestampInRange(NULL, 0, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewTimestampInRange(mContentView, 0, NULL)));

    // Shouldn't be in range
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewTimestampInRange(mContentView, 0, &inRange)));
    EXPECT_FALSE(inRange);
}

TEST_F(ViewApiTest, contentViewGetNext_NullPointer)
{
    PViewItem pViewItem;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetNext(NULL, &pViewItem)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetNext(NULL, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetNext(mContentView, NULL)));

    // Shouldn't have any items
    EXPECT_EQ(STATUS_CONTENT_VIEW_NO_MORE_ITEMS, contentViewGetNext(mContentView, &pViewItem));
}

TEST_F(ViewApiTest, contentViewGetCurrent_NullPointer)
{
    UINT32 index;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetCurrentIndex(NULL, &index)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetCurrentIndex(NULL, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetCurrentIndex(mContentView, NULL)));
}

TEST_F(ViewApiTest, contentViewSetCurrent_NullPointerInvalid)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewSetCurrentIndex(NULL, 0)));
    EXPECT_TRUE(STATUS_FAILED(contentViewSetCurrentIndex(NULL, 1)));

    CreateContentView();
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 0));
    EXPECT_EQ(STATUS_CONTENT_VIEW_INVALID_INDEX, contentViewSetCurrentIndex(mContentView, 1));

    // add one
    EXPECT_EQ(STATUS_SUCCESS, contentViewAddItem(mContentView, 0, 10, 1, 0, 10, ITEM_FLAG_FRAGMENT_START));
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 0));
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 1));
    EXPECT_EQ(STATUS_CONTENT_VIEW_INVALID_INDEX, contentViewSetCurrentIndex(mContentView, 2));

    // Add another
    EXPECT_EQ(STATUS_SUCCESS, contentViewAddItem(mContentView, 10, 20, 1, 0, 10, ITEM_FLAG_FRAGMENT_START));
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 0));
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 1));
    EXPECT_EQ(STATUS_SUCCESS, contentViewSetCurrentIndex(mContentView, 2));
    EXPECT_EQ(STATUS_CONTENT_VIEW_INVALID_INDEX, contentViewSetCurrentIndex(mContentView, 3));
}

TEST_F(ViewApiTest, contentViewRollbackCurrent_NullPointerInvalid)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 0, TRUE, TRUE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 0, TRUE, FALSE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 0, FALSE, TRUE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 0, FALSE, FALSE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 1000000, TRUE, TRUE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 1000000, TRUE, FALSE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 1000000, FALSE, TRUE)));
    EXPECT_TRUE(STATUS_FAILED(contentViewRollbackCurrent(NULL, 1000000, FALSE, FALSE)));
}

TEST_F(ViewApiTest, contentViewResetCurrent_NullPointer)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewResetCurrent(NULL)));
}

TEST_F(ViewApiTest, contentViewGetTail_NullPointer)
{
    PViewItem pViewItem;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetTail(NULL, &pViewItem)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetTail(NULL, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetTail(mContentView, NULL)));

    // Shouldn't be any tail
    EXPECT_EQ(STATUS_CONTENT_VIEW_NO_MORE_ITEMS, contentViewGetTail(mContentView, &pViewItem));
}

TEST_F(ViewApiTest, contentViewGetHead_NullPointer)
{
    PViewItem pViewItem;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetHead(NULL, &pViewItem)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetHead(NULL, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetHead(mContentView, NULL)));

    // Shouldn't be any tail
    EXPECT_EQ(STATUS_CONTENT_VIEW_NO_MORE_ITEMS, contentViewGetHead(mContentView, &pViewItem));
}

TEST_F(ViewApiTest, contentViewGetAllocationSize_NullPointer)
{
    PViewItem pViewItem;
    UINT32 allocationSize;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetAllocationSize(NULL, &allocationSize)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetAllocationSize(mContentView, NULL)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetAllocationSize(NULL, NULL)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetAllocationSize(mContentView, NULL)));

    // Should succeed
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewGetAllocationSize(mContentView, &allocationSize)));
    EXPECT_EQ(SIZEOF(RollingContentView) + SIZEOF(ViewItem) * MAX_VIEW_ITEM_COUNT, allocationSize);
}

TEST_F(ViewApiTest, contentViewAddItem_InvalidTime)
{
    EXPECT_TRUE(STATUS_FAILED(contentViewAddItem(NULL, 5, 5, INVALID_ALLOCATION_HANDLE_VALUE, 0, 1, ITEM_FLAG_NONE)));

    CreateContentView();
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewAddItem(mContentView, 5, 10, INVALID_ALLOCATION_HANDLE_VALUE, 0, 1, ITEM_FLAG_NONE)));
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewAddItem(mContentView, 15, 10, INVALID_ALLOCATION_HANDLE_VALUE, 0, 1, ITEM_FLAG_NONE)));
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewAddItem(mContentView, 40, 10, INVALID_ALLOCATION_HANDLE_VALUE, 0, 1, ITEM_FLAG_NONE)));

    // Add with older timestamp
    EXPECT_EQ(STATUS_CONTENT_VIEW_INVALID_TIMESTAMP, contentViewAddItem(mContentView, 49, 10, INVALID_ALLOCATION_HANDLE_VALUE, 0, 1, ITEM_FLAG_NONE));

    // Add a 0-length item
    EXPECT_EQ(STATUS_INVALID_CONTENT_VIEW_LENGTH, contentViewAddItem(mContentView, 60, 10, INVALID_ALLOCATION_HANDLE_VALUE, 0, 0, ITEM_FLAG_NONE));
}

TEST_F(ViewApiTest, contentViewGetWindowDuration_NullPointer)
{
    UINT64 currentDuration, windowDuration;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowDuration(NULL, &currentDuration, &windowDuration)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowDuration(NULL, NULL, &windowDuration)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowDuration(mContentView, NULL, &windowDuration)));
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewGetWindowDuration(mContentView, &currentDuration, NULL)));
}

TEST_F(ViewApiTest, contentViewGetWindowItemCount_NullPointer)
{
    UINT32 currentSize, windowSize;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowItemCount(NULL, &currentSize, &windowSize)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowItemCount(NULL, NULL, &windowSize)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowItemCount(mContentView, NULL, &windowSize)));
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewGetWindowItemCount(mContentView, &currentSize, NULL)));
}

TEST_F(ViewApiTest, contentViewGetWindowAllocationSize_NullPointer)
{
    UINT64 currentSize, windowSize;

    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowAllocationSize(NULL, &currentSize, &windowSize)));
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowAllocationSize(NULL, NULL, &windowSize)));

    CreateContentView();
    EXPECT_TRUE(STATUS_FAILED(contentViewGetWindowAllocationSize(mContentView, NULL, &windowSize)));
    EXPECT_TRUE(STATUS_SUCCEEDED(contentViewGetWindowAllocationSize(mContentView, &currentSize, NULL)));
}
