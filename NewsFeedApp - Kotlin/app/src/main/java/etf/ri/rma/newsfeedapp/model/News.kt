package etf.ri.rma.newsfeedapp.model

import androidx.room.ColumnInfo
import androidx.room.Entity
import androidx.room.Ignore
import androidx.room.Index
import androidx.room.PrimaryKey

@Entity(tableName = "News", indices = [Index(value = ["title"], unique = true)])
data class News(
    @PrimaryKey(autoGenerate = true)
    var id: Long = 0,
    val uuid: String,
    val title: String,
    val snippet: String,
    val category: String,
    val imageUrl: String?,
    val isFeatured: Boolean,
    val source: String,
    val publishedDate: String
) {
    @Ignore
    var tags: List<Tag> = emptyList()
}

@Entity(tableName = "Tags", indices = [Index(value = ["value"], unique = true)])
data class Tag(
    @PrimaryKey(autoGenerate = true)
    var id: Long = 0,
    val value: String
)

@Entity(tableName = "NewsTags")
data class NewsTag(
    @PrimaryKey(autoGenerate = true)
    var id: Long = 0,
    @ColumnInfo(index = true)
    val newsId: Long,
    @ColumnInfo(index = true)
    val tagsId: Long
) 