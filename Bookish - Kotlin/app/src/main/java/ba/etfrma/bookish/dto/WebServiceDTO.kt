package ba.etfrma.bookish.dto

import ba.etfrma.bookish.model.Author
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.model.BookEntity
import ba.etfrma.bookish.model.Category
import com.google.gson.annotations.SerializedName

data class BookResponse(
    @SerializedName("items") val items: List<BookItem>
)

data class BookItem(
    @SerializedName("id") val id: String,
    @SerializedName("volumeInfo") val volumeInfo: VolumeInfo
)

data class VolumeInfo(
    @SerializedName("title") val title: String?,
    @SerializedName("authors") val authors: List<String>?,
    @SerializedName("publisher") val publisher: String?,
    @SerializedName("categories") val categories: List<String>?,
    @SerializedName("infoLink") val infoLink: String?,
    @SerializedName("description") val description: String?,
    @SerializedName("imageLinks") val imageLinks: ImageLinks?
)

data class ImageLinks(
    @SerializedName("thumbnail") val thumbnail: String?
)

fun BookItem.toBook(): Book {
    val bookEntity = BookEntity(
        id = this.id,
        title = volumeInfo.title ?: "N/A",
        publisher = volumeInfo.publisher ?: "Unknown",
        infoLink = volumeInfo.infoLink ?: "",
        description = volumeInfo.description ?: "No description",
        thumbnail = volumeInfo.imageLinks?.thumbnail ?: ""
    )

    val authors = volumeInfo.authors?.map { Author(name = it) } ?: listOf(Author(name = "Unknown"))
    val categories = volumeInfo.categories?.map { Category(name = it) } ?: listOf(Category(name = "Uncategorized"))

    return Book(
        book = bookEntity,
        authors = authors,
        categories = categories
    )
}