package etf.ri.rma.newsfeedapp.data

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query
import androidx.room.Transaction
import etf.ri.rma.newsfeedapp.model.News
import etf.ri.rma.newsfeedapp.model.NewsItem
import etf.ri.rma.newsfeedapp.model.NewsTag
import etf.ri.rma.newsfeedapp.model.Tag

@Dao
abstract class SavedNewsDAO {

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    abstract suspend fun insertNews(news: News): Long

    @Transaction
    open suspend fun saveNews(news: NewsItem): Boolean {
        return insertNews(news.toNewsEntity()) != -1L
    }

    @Query("SELECT * FROM News")
    abstract suspend fun getAllNewsEntities(): List<News>

    @Query("SELECT * FROM Tags WHERE id IN (SELECT tagsId FROM NewsTags WHERE newsId = :newsId)")
    abstract suspend fun getTagsForNews(newsId: Long): List<Tag>

    @Transaction
    open suspend fun allNews(): List<NewsItem> {
        return getAllNewsEntities().map { news ->
            news.toNewsItem(getTagsForNews(news.id))
        }
    }

    @Query("SELECT * FROM News WHERE category = :category")
    abstract suspend fun getNewsEntitiesWithCategory(category: String): List<News>

    @Transaction
    open suspend fun getNewsWithCategory(category: String): List<NewsItem> {
        return getNewsEntitiesWithCategory(category).map { news ->
            news.toNewsItem(getTagsForNews(news.id))
        }
    }

    @Insert(onConflict = OnConflictStrategy.IGNORE)
    abstract suspend fun insertTag(tag: Tag): Long

    @Query("SELECT id FROM Tags WHERE value = :tagValue")
    abstract suspend fun getTagIdByValue(tagValue: String): Long?

    @Insert
    abstract suspend fun insertNewsTag(newsTag: NewsTag)

    @Transaction
    open suspend fun addTags(tags: List<String>, newsId: Int): Int {
        var newTagsCount = 0
        for (tagValue in tags) {
            var tagId = getTagIdByValue(tagValue)
            if (tagId == null) {
                tagId = insertTag(Tag(value = tagValue))
                if (tagId != -1L) {
                    newTagsCount++
                }
            }
            if (tagId != null && tagId != -1L) {
                insertNewsTag(NewsTag(newsId = newsId.toLong(), tagsId = tagId))
            }
        }
        return newTagsCount
    }

    @Query("SELECT T.value FROM Tags T JOIN NewsTags NT ON T.id = NT.tagsId WHERE NT.newsId = :newsId")
    abstract suspend fun getTags(newsId: Int): List<String>
    
    @Query("""
        SELECT DISTINCT N.* FROM News N
        JOIN NewsTags NT ON N.id = NT.newsId
        JOIN Tags T ON NT.tagsId = T.id
        WHERE T.value IN (:tagValues)
        ORDER BY SUBSTR(N.publishedDate, 7, 4) DESC, SUBSTR(N.publishedDate, 4, 2) DESC, SUBSTR(N.publishedDate, 1, 2) DESC
    """)
    abstract suspend fun getNewsEntitiesWithTagsSorted(tagValues: List<String>): List<News>

    @Transaction
    open suspend fun getSimilarNews(tags: List<String>): List<NewsItem> {
        if (tags.isEmpty()) return emptyList()
        return getNewsEntitiesWithTagsSorted(tags).map { news ->
            news.toNewsItem(getTagsForNews(news.id))
        }
    }

    @Query("SELECT * FROM Tags")
    abstract suspend fun getAllTags(): List<Tag>

    // Conversion helpers
    private fun News.toNewsItem(tags: List<Tag>): NewsItem = NewsItem(
        uuid = this.uuid,
        title = this.title,
        snippet = this.snippet,
        imageUrl = this.imageUrl,
        category = this.category,
        isFeatured = this.isFeatured,
        source = this.source,
        publishedDate = this.publishedDate,
        imageTags = ArrayList(tags)
    )
    private fun NewsItem.toNewsEntity(): News = News(
        uuid = this.uuid,
        title = this.title,
        snippet = this.snippet,
        imageUrl = this.imageUrl,
        category = this.category,
        isFeatured = this.isFeatured,
        source = this.source,
        publishedDate = this.publishedDate
    )
} 