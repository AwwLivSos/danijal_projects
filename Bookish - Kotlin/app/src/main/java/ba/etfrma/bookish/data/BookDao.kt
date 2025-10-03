package ba.etfrma.bookish.data

import androidx.room.Dao
import androidx.room.Delete
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import ba.etfrma.bookish.model.Author
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.model.BookAuthorCrossRef
import ba.etfrma.bookish.model.BookCategoryCrossRef
import ba.etfrma.bookish.model.BookEntity
import ba.etfrma.bookish.model.Category

@Dao
interface BookDao {
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertBook(book: BookEntity)

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertAuthors(authors: List<Author>): List<Long>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertCategories(categories: List<Category>): List<Long>

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertBookAuthorCrossRefs(refs: List<BookAuthorCrossRef>)

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertBookCategoryCrossRefs(refs: List<BookCategoryCrossRef>)

    @Transaction
    @Query("SELECT * FROM books WHERE id = :id")
    suspend fun getBookWithRelations(id: String): Book?

    @Transaction
    @Query("SELECT * FROM books")
    suspend fun getAllBooksWithRelations(): List<Book>

    @Delete
    suspend fun deleteBook(book: BookEntity)
}