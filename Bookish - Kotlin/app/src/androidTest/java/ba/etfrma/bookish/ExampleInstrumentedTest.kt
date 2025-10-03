package ba.etfrma.bookish

import android.content.Context
import android.content.Intent
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import androidx.test.espresso.intent.Intents.init
import androidx.test.espresso.intent.Intents.intended
import androidx.test.espresso.intent.Intents.release
import androidx.test.espresso.intent.matcher.IntentMatchers
import androidx.test.espresso.intent.matcher.IntentMatchers.hasExtra
import androidx.test.platform.app.InstrumentationRegistry
import androidx.test.ext.junit.runners.AndroidJUnit4
import ba.etfrma.bookish.data.AppDatabase
import ba.etfrma.bookish.data.BookDao
import ba.etfrma.bookish.data.BookStaticData
import org.hamcrest.CoreMatchers.`is`

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import org.junit.Rule
import org.hamcrest.Matcher
import org.hamcrest.CoreMatchers.allOf
import org.hamcrest.CoreMatchers.equalTo
import org.junit.After
import org.junit.Before

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class BookishInstrumentedTests {

    private fun hasSendIntentInsideChooser(bookDescription: String): Matcher<Intent> {
        return allOf(
            IntentMatchers.hasAction(Intent.ACTION_CHOOSER),
            hasExtra(`is`(Intent.EXTRA_INTENT), allOf(
                IntentMatchers.hasAction(Intent.ACTION_SEND),
                hasExtra(Intent.EXTRA_TEXT, bookDescription),
                IntentMatchers.hasType("text/plain")
            ))
        )
    }

    @get:Rule
    val composeTestRule = createAndroidComposeRule<MainActivity>()

    @Test
    fun allBooksAreDisplayedOnHomeScreen() {
        val books = BookStaticData.getSampleBooks()

        books.forEach { book ->
            composeTestRule.onNodeWithText(book.book.title, substring = true)
                .assertExists("Book '${book.book.title}' is not displayed")
        }
    }

    @Test
    fun insertAndReadBook() {
        val context = ApplicationProvider.getApplicationContext<Context>()
        val contentResolver = context.contentResolver
        val uri = Uri.parse("content://ba.etfrma.bookish.provider/books")

        // Prepare book data
        val values = ContentValues().apply {
            put("id", "test_id_1")
            put("title", "Test Book")
            put("publisher", "Test Publisher")
            put("infoLink", "http://test.info")
            put("description", "Test Description")
            put("thumbnail", "http://test.thumb")
            put("authors", "Author1,Author2")
            put("categories", "Category1,Category2")
        }

        // Insert book
        val insertedUri = contentResolver.insert(uri, values)
        assertNotNull(insertedUri)

        // Query book
        val cursor = contentResolver.query(uri, null, "id=?", arrayOf("test_id_1"), null)
        assertNotNull(cursor)
        cursor?.use {
            assertTrue(it.moveToFirst())
            val title = it.getString(it.getColumnIndex("title"))
            val publisher = it.getString(it.getColumnIndex("publisher"))
            val authors = it.getString(it.getColumnIndex("authors"))
            val categories = it.getString(it.getColumnIndex("categories"))
            assertEquals("Test Book", title)
            assertEquals("Test Publisher", publisher)
            assertEquals("Author1,Author2", authors)
            assertEquals("Category1,Category2", categories)
        }
    }

    private lateinit var db: AppDatabase
    private lateinit var dao: BookDao

    @Before
    fun setUp() {
        val context = ApplicationProvider.getApplicationContext<Context>()
        db = Room.inMemoryDatabaseBuilder(context, AppDatabase::class.java).build()
        dao = db.bookDao()
    }

    @After
    fun tearDown() {
        db.close()
    }

    @Test
    fun insertAndGetBookWithRelations() = runBlocking {
        val book = BookEntity("1", "Title", "Publisher", "Link", "Desc", "thumb")
        val authors = listOf(Author(1000000,"Author1"))
        val categories = listOf(Category(2000000,"Category1"))

        dao.insertBook(book)
        dao.insertAuthors(authors)
        dao.insertCategories(categories)
        dao.insertBookAuthorCrossRefs(listOf(BookAuthorCrossRef("1", 1000000)))
        dao.insertBookCategoryCrossRefs(listOf(BookCategoryCrossRef("1", 2000000)))

        val result = dao.getBookWithRelations("1")
        Assert.assertEquals("Title", result?.book?.title)
        Assert.assertEquals("Author1", result?.authors?.first()?.name)
        Assert.assertEquals("Category1", result?.categories?.first()?.name)
    }

    @Test
    fun clickingBookNavigatesToDetailsAndDisplaysCorrectInfo() {
        val book = BookStaticData.getSampleBooks().first()

        composeTestRule.onNodeWithText(book.title, substring = true)
            .performClick()

        composeTestRule.onNodeWithText(book.title, substring = true).assertExists()
        composeTestRule.onNodeWithText("Authors: ${book.authors.joinToString(", ")}").assertExists()
        composeTestRule.onNodeWithText("Publisher: ${book.publisher}").assertExists()
        composeTestRule.onNodeWithText("Categories: ${book.categories.joinToString(", ")}").assertExists()
        composeTestRule.onNodeWithText(book.description, substring = true).assertExists()
    }

    @Test
    fun shareIntentIsTriggeredOnDescriptionClick() {
        val book = BookStaticData.getSampleBooks().first()

        init()

        composeTestRule.onNodeWithText(book.title, substring = true).performClick()
        composeTestRule.onNodeWithText(book.description, substring = true).performClick()

        intended(hasSendIntentInsideChooser(book.description))

        release()
    }

    @Test
    fun clickingBookTitleOpensWebSearch() {
        val book = BookStaticData.getSampleBooks().first()

        init()

        composeTestRule.onNodeWithText(book.title, substring = true).performClick()
        composeTestRule.onNodeWithText(book.title, substring = true).performClick()

        intended(allOf(
            IntentMatchers.hasAction(Intent.ACTION_WEB_SEARCH),
            hasExtra(`is`("query"), equalTo(book.title))
        ))

        release()
    }

    @Test
    fun clickingBookLinkOpensWebPage() {
        val book = BookStaticData.getSampleBooks().first()
        val bookLink = book.infoLink

        init()

        composeTestRule.onNodeWithText(book.title, substring = true).performClick()
        composeTestRule.onNodeWithText(bookLink, substring = true).performClick()

        intended(allOf(
            IntentMatchers.hasAction(Intent.ACTION_VIEW),
            IntentMatchers.hasData(bookLink)
        ))

        release()
    }
}