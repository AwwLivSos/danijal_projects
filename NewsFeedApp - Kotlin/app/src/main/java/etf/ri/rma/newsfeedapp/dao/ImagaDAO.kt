package etf.ri.rma.newsfeedapp.dao

import etf.ri.rma.newsfeedapp.data.network.exception.InvalidImageURLException
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory
import retrofit2.http.GET
import retrofit2.http.Header
import retrofit2.http.Query
import java.net.MalformedURLException
import java.net.URL
import java.net.URISyntaxException

class ImagaDAO {
    // --- Response Data Classes for Imagga API ---
    private data class TaggingResponse(
        val result: Result,
        val status: Status
    )
    private data class Result(val tags: List<Tag>)
    private data class Tag(val confidence: Double, val tag: TagInfo)
    private data class TagInfo(val en: String)
    private data class Status(val text: String, val type: String)

    // --- Retrofit Interface ---
    private interface ImaggaService {
        @GET("v2/tags")
        suspend fun getTags(
            @Query("image_url") imageUrl: String,
            @Header("Authorization") authorization: String
        ): TaggingResponse
    }

    // --- OkHttp Client with Logging ---
    private val loggingInterceptor = HttpLoggingInterceptor().apply {
        level = HttpLoggingInterceptor.Level.BODY
    }

    private val client = OkHttpClient.Builder()
        .addInterceptor(loggingInterceptor)
        .build()

    // --- Retrofit Setup ---
    private val retrofit = Retrofit.Builder()
        .baseUrl("https://api.imagga.com/")
        .client(client)
        .addConverterFactory(GsonConverterFactory.create())
        .build()

    private val imaggaService = retrofit.create(ImaggaService::class.java)

    // --- Imagga API Credentials ---
    private val apiKey = "acc_5df7273a73f30f5"
    private val apiSecret = "218c2be05585d762c56ea1cee3ccdec0"
    private val authorization = "Basic ${android.util.Base64.encodeToString(
        "$apiKey:$apiSecret".toByteArray(),
        android.util.Base64.NO_WRAP
    )}"

    // --- Optional Tag Cache ---
    private val tagCache = mutableMapOf<String, List<String>>()

    // --- Public Function ---
    suspend fun getTags(imageURL: String): List<String> = withContext(Dispatchers.IO) {
        // Validate URL format early
        try {
            URL(imageURL).toURI()
        } catch (e: URISyntaxException) {
            throw InvalidImageURLException()
        } catch (e: IllegalArgumentException) {
            throw InvalidImageURLException()
        } catch (e: MalformedURLException) {
            throw InvalidImageURLException()
        }

        // Check cache first
        tagCache[imageURL]?.let {
            return@withContext it
        }

        try {
            val response = imaggaService.getTags(imageURL, authorization)

            val filteredTags = response.result.tags
                .filter { it.confidence >= 30.0 }
                .map { it.tag.en }

            // Save to cache
            tagCache[imageURL] = filteredTags

            return@withContext filteredTags
        } catch (e: retrofit2.HttpException) {
            throw Exception("HTTP error: ${e.code()} ${e.message()}")
        } catch (e: MalformedURLException) {
            throw InvalidImageURLException("Invalid image URL: $imageURL")
        } catch (e: IllegalArgumentException) {
            throw InvalidImageURLException("Invalid image URL: $imageURL")
        }
        catch (e: Exception) {
            throw Exception("Failed to get image tags: ${e.message}")
        }

    }
}
