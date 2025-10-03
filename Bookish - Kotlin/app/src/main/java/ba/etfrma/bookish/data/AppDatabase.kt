package ba.etfrma.bookish.data

import android.content.Context
import androidx.room.Database
import androidx.room.Room
import androidx.room.RoomDatabase
import ba.etfrma.bookish.model.Author
import ba.etfrma.bookish.model.BookAuthorCrossRef
import ba.etfrma.bookish.model.BookCategoryCrossRef
import ba.etfrma.bookish.model.BookEntity
import ba.etfrma.bookish.model.Category

@Database(
    entities = [
        BookEntity::class,
        Author::class,
        Category::class,
        BookAuthorCrossRef::class,
        BookCategoryCrossRef::class
    ],
    version = 1
)
abstract class AppDatabase : RoomDatabase() {
    abstract fun bookDao(): BookDao

    companion object {
        @Volatile private var INSTANCE: AppDatabase? = null

        fun getDatabase(context: Context): AppDatabase {
            return INSTANCE ?: synchronized(this) {
                Room.databaseBuilder(
                    context.applicationContext,
                    AppDatabase::class.java,
                    "bookish_database"
                ).build().also { INSTANCE = it }
            }
        }
    }
}