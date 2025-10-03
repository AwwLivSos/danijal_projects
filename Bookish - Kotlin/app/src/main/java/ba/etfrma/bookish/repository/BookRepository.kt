import android.content.Context
import ba.etfrma.bookish.data.AppDatabase
import ba.etfrma.bookish.data.RetrofitInstance
import ba.etfrma.bookish.dto.toBook
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.model.BookAuthorCrossRef
import ba.etfrma.bookish.model.BookCategoryCrossRef

class BookRepository(private val context: Context)  {

    private val api = RetrofitInstance.api

    suspend fun searchBooks(query: String): List<Book> = withContext(Dispatchers.IO) {
        val response = api.searchBooks(query)
        response.items.map { it.toBook() }.take(10)
    }

    suspend fun getBookById(id: String): Book? = withContext(Dispatchers.IO) {
        try {
            val bookItem = api.getBookById(id)
            bookItem.toBook()
        } catch (e: Exception) {
            e.printStackTrace()
            null
        }
    }

    private val dao by lazy {
        AppDatabase.getDatabase(context).bookDao()
    }
    suspend fun getAllBooksFromDatabase(): List<Book> = withContext(Dispatchers.IO) {
        dao.getAllBooksWithRelations()
    }
    suspend fun getAllBooksFromDatabase(): List<Book> = withContext(Dispatchers.IO) {
        dao.getAllBooksWithRelations()
    }
    suspend fun getBookByIdDatabase(id: String): Book? = withContext(Dispatchers.IO) {
        dao.getBookWithRelations(id)
    }
    suspend fun saveToLocal(book: Book) = withContext(Dispatchers.IO) {
        dao.insertBook(book.book)
        val authorIds = dao.insertAuthors(book.authors)
        val categoryIds = dao.insertCategories(book.categories)

        val authorRefs = authorIds.map { BookAuthorCrossRef(book.book.id, it) }
        val categoryRefs = categoryIds.map { BookCategoryCrossRef(book.book.id, it) }

        dao.insertBookAuthorCrossRefs(authorRefs)
        dao.insertBookCategoryCrossRefs(categoryRefs)
    }
    suspend fun loadBooksFromContentProvider(context: Context): List<Book> = withContext(Dispatchers.IO) {

        val cursor = context.contentResolver.query(uri, null, null, null, null)
        val books = mutableListOf<Book>()

        cursor?.use {
            val idIndex = it.getColumnIndex("id")
            val titleIndex = it.getColumnIndex("title")
            val publisherIndex = it.getColumnIndex("publisher")
            val infoLinkIndex = it.getColumnIndex("infoLink")
            val descriptionIndex = it.getColumnIndex("description")
            val thumbnailIndex = it.getColumnIndex("thumbnail")
            val authorsIndex = it.getColumnIndex("authors") // očekuje se da je spojena kolona stringa
            val categoriesIndex = it.getColumnIndex("categories") // isto

            while (it.moveToNext()) {
                val id = it.getString(idIndex)
                val title = it.getString(titleIndex)
                val publisher = it.getString(publisherIndex)
                val infoLink = it.getString(infoLinkIndex)
                val description = it.getString(descriptionIndex)
                val thumbnail = it.getString(thumbnailIndex)
                val authors = it.getString(authorsIndex)
                val categories = it.getString(categoriesIndex)

                books.add(
                    Book(
                        book = BookEntity(id, title, publisher, infoLink, description, thumbnail),
                        authors = authors.split(",").map { Author(name = it.trim()) },
                        categories = categories.split(",").map { Category(name = it.trim()) }
                    )
                )
            }
        }

        books
    }
}