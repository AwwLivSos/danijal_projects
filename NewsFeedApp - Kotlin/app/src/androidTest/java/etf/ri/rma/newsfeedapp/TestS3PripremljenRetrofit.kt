// File: TestS3PripremljenRetrofit.kt
package etf.ri.rma.newsfeedapp

import etf.ri.rma.newsfeedapp.dao.NewsDAO
import etf.ri.rma.newsfeedapp.dao.ImagaDAO
import etf.ri.rma.newsfeedapp.model.NewsItem
import okhttp3.OkHttpClient
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.GET
import retrofit2.http.Query
import retrofit2.http.Header
import java.lang.reflect.Field
import java.lang.reflect.Proxy

// Define the test interfaces that match the expected API structure
interface NewsApiService {
    @GET("v1/news/top")
    suspend fun getTopHeadlines(
        @Query("category") category: String,
        @Query("locale") locale: String = "us",
        @Query("limit") limit: Int = 3,
        @Query("api_token") apiKey: String
    ): NewsResponse

    @GET("v1/news/similar")
    suspend fun getSimilarStories(
        @Query("uuid") uuid: String,
        @Query("api_token") apiKey: String
    ): NewsResponse
}

interface ImagaApiService {
    @GET("v2/tags")
    suspend fun getTags(
        @Query("image_url") imageUrl: String,
        @Header("Authorization") authorization: String
    ): TaggingResponse
}

// Response data classes for the test interfaces
data class NewsResponse(
    val meta: Meta,
    val data: List<Article>
)

data class Meta(
    val found: Int,
    val returned: Int,
    val limit: Int,
    val page: Int
)

data class Article(
    val uuid: String,
    val title: String,
    val description: String?,
    val image_url: String?,
    val published_at: String,
    val source: String,
    val categories: List<String>
)

data class TaggingResponse(
    val result: Result,
    val status: Status
)

data class Result(
    val tags: List<Tag>
)

data class Tag(
    val confidence: Double,
    val tag: TagInfo
)

data class TagInfo(
    val en: String
)

data class Status(
    val text: String,
    val type: String
)

class TestS3PripremljenRetrofit {
    fun getNewsDAOwithBaseURL(baseURL: String, httpClient: OkHttpClient): NewsDAO {
        val retrofit = Retrofit.Builder()
            .baseUrl(baseURL)
            .client(httpClient)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
        
        // Get the NewsApiService interface from NewsDAO using reflection
        val newsApiServiceClass = NewsDAO::class.java.getDeclaredClasses()
            .find { it.simpleName == "NewsApiService" }
            ?: throw RuntimeException("Could not find NewsApiService interface in NewsDAO")
        
        // Create a proxy that implements the exact same interface
        val newsApiService = retrofit.create(newsApiServiceClass)
        
        // Create a test instance of NewsDAO
        val newsDAO = NewsDAO.createTestInstance(baseURL, "test_api_key")
        
        // Use reflection to set the service
        try {
            val field = NewsDAO::class.java.getDeclaredField("newsApiService")
            field.isAccessible = true
            field.set(newsDAO, newsApiService)
        } catch (e: Exception) {
            throw RuntimeException("Failed to set NewsApiService in NewsDAO", e)
        }
        
        return newsDAO
    }

    fun getImaggaDAOwithBaseURL(baseURL: String, httpClient: OkHttpClient): ImagaDAO {
        val retrofit = Retrofit.Builder()
            .baseUrl(baseURL)
            .client(httpClient)
            .addConverterFactory(GsonConverterFactory.create())
            .build()
        
        // Get the ImaggaService interface from ImagaDAO using reflection
        val imaggaServiceClass = ImagaDAO::class.java.getDeclaredClasses()
            .find { it.simpleName == "ImaggaService" }
            ?: throw RuntimeException("Could not find ImaggaService interface in ImagaDAO")
        
        // Create a proxy that implements the exact same interface
        val imaggaService = retrofit.create(imaggaServiceClass)
        
        // Create a new instance of ImagaDAO since it's a class
        val imaggaDAO = ImagaDAO()
        
        // Use reflection to set the service since it's private
        try {
            val field = ImagaDAO::class.java.getDeclaredField("imaggaService")
            field.isAccessible = true
            field.set(imaggaDAO, imaggaService)
        } catch (e: Exception) {
            throw RuntimeException("Failed to set ImaggaService in ImagaDAO", e)
        }
        
        return imaggaDAO
    }
}