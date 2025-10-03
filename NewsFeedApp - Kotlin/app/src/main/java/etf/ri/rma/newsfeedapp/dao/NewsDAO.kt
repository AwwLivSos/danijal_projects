package etf.ri.rma.newsfeedapp.dao

import android.os.Build
import android.util.Log // Add this import for logging
import androidx.annotation.RequiresApi
import etf.ri.rma.newsfeedapp.data.NewsData // Assuming NewsData is an object too
import etf.ri.rma.newsfeedapp.data.network.exception.InvalidUUIDException
import etf.ri.rma.newsfeedapp.model.NewsItem
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.GET
import retrofit2.http.Query
import java.util.*
import kotlin.time.Duration.Companion.seconds
import java.time.LocalDate
import java.time.format.DateTimeFormatter
import java.time.temporal.ChronoUnit
import kotlin.math.abs
import etf.ri.rma.newsfeedapp.model.Tag

@RequiresApi(Build.VERSION_CODES.O)
class NewsDAO private constructor(
    private val baseUrl: String = "https://api.thenewsapi.com/",
    private val apiKey: String = "qlvys8k3aMHylkvFylSGrCtPNBxxvzoIe443oqm7"
) {
    companion object {
        private val validCategories = setOf(
            "business", "tech", "sports", "entertainment",
            "health", "science", "world", "politics"
        )

        // Default instance for backward compatibility
        private var defaultInstance: NewsDAO? = null

        fun getInstance(): NewsDAO {
            return defaultInstance ?: NewsDAO().also { defaultInstance = it }
        }

        // For testing purposes
        fun createTestInstance(baseUrl: String, apiKey: String): NewsDAO {
            return NewsDAO(baseUrl, apiKey)
        }
    }

    // Instance-specific caches
    private val allStories = mutableListOf<NewsItem>()
    private val categoryLastFetchTime = mutableMapOf<String, Long>()
    private val categoryStories = mutableMapOf<String, List<NewsItem>>()
    private val similarStoriesCache = mutableMapOf<String, List<NewsItem>>()
    private val imageTagsCache = mutableMapOf<String, List<String>>()

    init {
        allStories.addAll(NewsData.getAllNews())
        Log.d("NewsDAO", "init: Initial allStories count: ${allStories.size}")
    }

    // Retrofit interface for TheNewsAPI
    private interface NewsApiService {
        @GET("v1/news/top")
        suspend fun getTopHeadlines(
            @Query("categories") category: String,
            @Query("locale") locale: String = "us",
            @Query("limit") limit: Int = 3, // Fetch 3 news items
            @Query("api_token") apiKey: String
        ): NewsResponse

        @GET("v1/news/similar")
        suspend fun getSimilarStories(
            @Query("uuid") uuid: String,
            @Query("api_token") apiKey: String
        ): NewsResponse
    }

    private data class NewsResponse(
        val data: List<Article>
    )

    private data class Article(
        val uuid: String,
        val title: String,
        val description: String?,
        val image_url: String?,
        val published_at: String,
        val source: String,
        val categories: List<String> // Original categories from API
    )

    private val retrofit = Retrofit.Builder()
        .baseUrl(baseUrl)
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    private val newsApiService = retrofit.create(NewsApiService::class.java)

    suspend fun getTopStoriesByCategory(category: String): List<NewsItem> = withContext(Dispatchers.IO) {
        if (category !in validCategories) {
            Log.e("NewsDAO", "Invalid category: $category")
            throw IllegalArgumentException("Invalid category. Must be one of: ${validCategories.joinToString()}")
        }

        val currentTime = System.currentTimeMillis()
        val lastFetchTime = categoryLastFetchTime[category] ?: 0L
        val timeSinceLastFetch = currentTime - lastFetchTime

        Log.d("NewsDAO", "getTopStoriesByCategory($category): timeSinceLastFetch = ${timeSinceLastFetch / 1000}s")
        Log.d("NewsDAO", "getTopStoriesByCategory($category): categoryStories hasKey = ${categoryStories.containsKey(category)}")

        // Check cache first (30-second window). If cached data is available and fresh, return it.
        if (timeSinceLastFetch < 30.seconds.inWholeMilliseconds && categoryStories.containsKey(category)) {
            Log.d("NewsDAO", "getTopStoriesByCategory($category): Returning cached data.")
            return@withContext categoryStories[category] ?: emptyList()
        }

        Log.d("NewsDAO", "getTopStoriesByCategory($category): Fetching from network.")
        try {
            val response = newsApiService.getTopHeadlines(category = category, apiKey = apiKey)
            Log.d("NewsDAO", "API call for category: $category. Fetched articles count: ${response.data.size}")
            response.data.forEachIndexed { index, article ->
                Log.d("NewsDAO", "  API Article $index: UUID: ${article.uuid}, Title: ${article.title}, Original Categories: ${article.categories.joinToString()}")
            }

            val fetchedNewsItems = response.data.map { article ->
                NewsItem(
                    uuid = article.uuid,
                    title = article.title,
                    snippet = article.description ?: "",
                    imageUrl = article.image_url,
                    category = category, // <-- CRITICAL FIX: Use the requested category
                    isFeatured = true, // These are the newly fetched, so mark as featured
                    source = article.source,
                    publishedDate = article.published_at,
                    imageTags = ArrayList() // No tags from API
                )
            }
            Log.d("NewsDAO", "Mapped ${fetchedNewsItems.size} NewsItems for category $category:")
            fetchedNewsItems.forEachIndexed { index, item ->
                Log.d("NewsDAO", "  Mapped NewsItem $index: UUID: ${item.uuid}, Title: ${item.title}, Assigned Category: ${item.category}, IsFeatured: ${item.isFeatured}")
            }

            val oldCategoryStories = categoryStories[category]?.map { it.copy(isFeatured = false) } ?: emptyList()
            Log.d("NewsDAO", "Old cached stories for $category count: ${oldCategoryStories.size}")

            // Combine new and old. Add new items first, then add old items that are not duplicates of new ones.
            val combinedForCategoryCache = mutableListOf<NewsItem>()
            val seenUuids = mutableSetOf<String>()

            fetchedNewsItems.forEach { item ->
                if (seenUuids.add(item.uuid)) { // Add if not already processed (shouldn't be for new ones)
                    combinedForCategoryCache.add(item)
                }
            }

            // Add old stories that haven't been seen in the new fetch
            oldCategoryStories.forEach { item ->
                if (seenUuids.add(item.uuid)) { // Add if UUID is not already in the combined list
                    combinedForCategoryCache.add(item)
                }
            }

            // Sort to ensure featured items are at the top
            combinedForCategoryCache.sortByDescending { it.isFeatured }

            // 3. Update the in-memory cache for THIS category
            categoryStories[category] = combinedForCategoryCache
            categoryLastFetchTime[category] = currentTime
            Log.d("NewsDAO", "Updated categoryStories[$category] with ${combinedForCategoryCache.size} items.")

            fetchedNewsItems.forEach { newStory ->
                // Only add to global stores if it's genuinely new globally (by UUID)
                if (!allStories.any { it.uuid == newStory.uuid }) {
                    allStories.add(newStory)
                    NewsData.addNews(newStory) // Assuming NewsData is also a singleton object
                    Log.d("NewsDAO", "Added new unique story to allStories/NewsData: ${newStory.title} (${newStory.uuid})")
                } else {
                    Log.d("NewsDAO", "Story ${newStory.title} (${newStory.uuid}) already exists in allStories/NewsData. Not re-adding.")
                }
            }
            Log.d("NewsDAO", "Current allStories count: ${allStories.size}")
            Log.d("NewsDAO", "Current NewsData.getAllNews() count: ${NewsData.getAllNews().size}")


            // 5. Return the specific combined list for this category to the caller (NewsFeedScreen)
            return@withContext combinedForCategoryCache

        } catch (e: retrofit2.HttpException) {
            val errorMessage = "HTTP Error ${e.code()}: ${e.message()}"
            Log.e("NewsDAO", "API call failed for category $category: $errorMessage", e)
            // If API fails, try to return cached data as a fallback
            if (categoryStories.containsKey(category)) {
                Log.d("NewsDAO", "Returning cached data due to API HTTP error.")
                return@withContext categoryStories[category] ?: emptyList()
            }
            throw Exception(errorMessage)
        } catch (e: Exception) {
            val errorMessage = "General error fetching news for category $category: ${e.message}"
            Log.e("NewsDAO", errorMessage, e)
            // If general error, try to return cached data as a fallback
            if (categoryStories.containsKey(category)) {
                Log.d("NewsDAO", "Returning cached data due to general error.")
                return@withContext categoryStories[category] ?: emptyList()
            }
            throw Exception(errorMessage)
        }
    }

    fun getAllStories(): List<NewsItem> {
        return allStories.toList() // Return a copy to prevent external modification
    }

    @RequiresApi(Build.VERSION_CODES.O)
    suspend fun getSimilarStories(uuid: String): List<NewsItem> = withContext(Dispatchers.IO) {
        try {
            UUID.fromString(uuid) // Validate UUID format

            // Check cache first
            similarStoriesCache[uuid]?.let {
                Log.d("NewsDAO", "Returning cached similar stories for UUID: $uuid")
                return@withContext it
            }

            // Call the API to get similar stories
            val response = newsApiService.getSimilarStories(uuid = uuid, apiKey = apiKey)
            
            val similarStories = response.data.map { article ->
                NewsItem(
                    uuid = article.uuid,
                    title = article.title,
                    snippet = article.description ?: "",
                    imageUrl = article.image_url,
                    category = article.categories.firstOrNull() ?: "general",
                    isFeatured = false,
                    source = article.source,
                    publishedDate = article.published_at,
                    imageTags = ArrayList() // No tags from API
                )
            }

            // Cache the result
            similarStoriesCache[uuid] = similarStories
            Log.d("NewsDAO", "Cached ${similarStories.size} similar stories for UUID: $uuid")
            similarStories

        } catch (e: IllegalArgumentException) {
            Log.e("NewsDAO", "Invalid UUID format: $uuid", e)
            throw InvalidUUIDException()
        } catch (e: Exception) {
            Log.e("NewsDAO", "Failed to get similar stories for UUID $uuid: ${e.message}", e)
            throw e
        }
    }

    suspend fun getTags(imageUrl: String): List<Tag> = withContext(Dispatchers.IO) {
        imageTagsCache[imageUrl]?.let {
            return@withContext it.map { tagStr -> Tag(value = tagStr) }
        }
        try {
            val tags = ImagaDAO().getTags(imageUrl)
            imageTagsCache[imageUrl] = tags
            return@withContext tags.map { Tag(value = it) }
        } catch (e: Exception) {
            throw Exception("Failed to fetch image tags: ${e.message}")
        }
    }
}