package ba.etfrma.bookish.data

import ba.etfrma.bookish.model.Author
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.model.BookEntity
import ba.etfrma.bookish.model.Category

object BookStaticData {

    fun getSampleBooks(): List<Book> {
        return listOf(
            Book(
                book = BookEntity(
                    id = "1",
                    title = "The Great Gatsby",
                    publisher = "Scribner",
                    description = "A novel set in the Jazz Age...",
                    infoLink = "https://books.google.ba/books/about/The_Great_Gatsby.html?id=iXn5U2IzVH0C&redir_esc=y",
                    thumbnail = "https://example.com/thumbnails/great_gatsby.jpg"
                ),
                authors = listOf(
                    Author(authorId = 1, name = "F. Scott Fitzgerald")
                ),
                categories = listOf(
                    Category(categoryId = 1, name = "Fiction"),
                    Category(categoryId = 2, name = "Classic")
                )
            )
        )
    }
}