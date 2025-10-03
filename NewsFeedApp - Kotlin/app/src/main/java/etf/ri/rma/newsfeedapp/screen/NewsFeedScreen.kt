package etf.ri.rma.newsfeedapp.screen

import android.os.Build
import android.util.Log
import androidx.annotation.RequiresApi
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.LazyRow
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.semantics.testTag
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import etf.ri.rma.newsfeedapp.dao.NewsDAO
import etf.ri.rma.newsfeedapp.data.NewsData
import etf.ri.rma.newsfeedapp.model.NewsItem
import kotlinx.coroutines.launch
import java.time.LocalDate
import java.time.format.DateTimeFormatter

@RequiresApi(Build.VERSION_CODES.O)
@Composable
fun NewsFeedScreen(
    navController: NavController,
    onNavigateToFilters: () -> Unit,
    onNavigateToDetails: (String) -> Unit
) {
    val currentBackStackEntry = navController.currentBackStackEntry
    val scope = rememberCoroutineScope()

    var selectedCategory by rememberSaveable {
        mutableStateOf(currentBackStackEntry?.savedStateHandle?.get<String>("category") ?: "All")
    }
    var dateRange by rememberSaveable { mutableStateOf<String?>(null) }
    var unwantedWords by rememberSaveable { mutableStateOf(emptyList<String>()) }
    var isLoading by remember { mutableStateOf(false) }
    var error by remember { mutableStateOf<String?>(null) }
    var newsUpdateTrigger by remember { mutableStateOf(0) }

    val newsDAO = remember { NewsDAO.getInstance() }

    LaunchedEffect(currentBackStackEntry) {
        currentBackStackEntry?.savedStateHandle?.apply {
            get<String>("category")?.let { category ->
                selectedCategory = category
                remove<String>("category")
            }
            get<String>("dateRange")?.let { range ->
                dateRange = range
                remove<String>("dateRange")
            }
            get<List<String>>("unwantedWords")?.let { words ->
                unwantedWords = words
                remove<List<String>>("unwantedWords")
            }
            Log.d("NewsFeedScreen", "LaunchedEffect(currentBackStackEntry): category=$selectedCategory, dateRange=$dateRange, unwantedWords=${unwantedWords.size} words")
        }
    }

    LaunchedEffect(selectedCategory) {
        Log.d("NewsFeedScreen", "LaunchedEffect(selectedCategory) triggered for: $selectedCategory")
        if (selectedCategory != "All") {
            isLoading = true
            error = null
            try {
                scope.launch {
                    val fetchedStories = newsDAO.getTopStoriesByCategory(selectedCategory)
                        .map { it.copy(isFeatured = true) }

                    val cachedStories = NewsData.getAllNews()
                        .filter { it.category == selectedCategory && it.uuid !in fetchedStories.map { f -> f.uuid } }
                        .map { it.copy(isFeatured = false) }

                    val allStories = (fetchedStories + cachedStories)
                        .sortedWith(
                            compareByDescending<NewsItem> { it.isFeatured }
                                .thenByDescending { it.publishedDate }
                        )

                    NewsData.clearCategory(selectedCategory) // implement this to clear only this category if needed
                    allStories.forEach { NewsData.addNews(it) }

                    newsUpdateTrigger++
                }
            } catch (e: Exception) {
                error = e.message
                Log.e("NewsFeedScreen", "Error fetching news for $selectedCategory: $error", e)
            } finally {
                isLoading = false
            }
        } else {
            isLoading = false
            error = null
            Log.d("NewsFeedScreen", "Selected category is 'All'. Displaying all available news from NewsData.")
        }
    }

    val allNews = remember(newsUpdateTrigger) {
        val currentAllNews = NewsData.getAllNews()
        Log.d("NewsFeedScreen", "Recomputing allNews. Total count: ${currentAllNews.size}")
        currentAllNews
    }

    val filteredNews = remember(selectedCategory, dateRange, unwantedWords, newsUpdateTrigger) {
        val filtered = allNews.filter { news ->
            val categoryMatch = if (selectedCategory == "All") {
                true
            } else {
                news.category == selectedCategory
            }

            val dateMatch = if (dateRange != null) {
                try {
                    val (startDateStr, endDateStr) = dateRange!!.split(";")
                    val publishedDate = LocalDate.parse(news.publishedDate, DateTimeFormatter.ISO_DATE)
                    val startDate = LocalDate.parse(startDateStr, DateTimeFormatter.ISO_DATE)
                    val endDate = LocalDate.parse(endDateStr, DateTimeFormatter.ISO_DATE)
                    !publishedDate.isBefore(startDate) && !publishedDate.isAfter(endDate)
                } catch (e: Exception) {
                    Log.e("NewsFeedScreen", "Date parsing error for ${news.publishedDate}: ${e.message}", e)
                    true
                }
            } else {
                true
            }

            val unwantedWordsMatch = unwantedWords.none { word ->
                news.title.lowercase().contains(word.lowercase()) ||
                        news.snippet.lowercase().contains(word.lowercase())
            }
            categoryMatch && dateMatch && unwantedWordsMatch
        }
        Log.d("NewsFeedScreen", "Recomputing filteredNews for category: $selectedCategory. Filtered count: ${filtered.size}")
        filtered
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(vertical = 16.dp)
    ) {
        LazyRow(
            modifier = Modifier
                .fillMaxWidth()
                .padding(horizontal = 16.dp)
                .semantics { testTag = "category_chips" },
            horizontalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            item {
                FilterChip(
                    selected = selectedCategory == "All",
                    onClick = { selectedCategory = "All" },
                    label = { Text("Sve") },
                    modifier = Modifier.semantics { testTag = "filter_chip_all" }
                )
            }

            item {
                FilterChip(
                    selected = selectedCategory == "politics",
                    onClick = { selectedCategory = "politics" },
                    label = { Text("Politika") },
                    modifier = Modifier.semantics { testTag = "filter_chip_politics" }
                )
            }

            item {
                FilterChip(
                    selected = selectedCategory == "sports",
                    onClick = { selectedCategory = "sports" },
                    label = { Text("Sport") },
                    modifier = Modifier.semantics { testTag = "filter_chip_sports" }
                )
            }

            item {
                FilterChip(
                    selected = selectedCategory == "tech",
                    onClick = { selectedCategory = "tech" },
                    label = { Text("Tehnologija") },
                    modifier = Modifier.semantics { testTag = "filter_chip_tech" }
                )
            }

            item {
                FilterChip(
                    selected = false,
                    onClick = {
                        navController.currentBackStackEntry?.savedStateHandle?.set("category", selectedCategory)
                        onNavigateToFilters()
                    },
                    label = { Text("Više filtera...") },
                    modifier = Modifier.semantics { testTag = "filter_chip_more" }
                )
            }
        }

        Spacer(modifier = Modifier.height(8.dp))

        if (isLoading) {
            Box(
                modifier = Modifier.fillMaxSize(),
                contentAlignment = Alignment.Center
            ) {
                CircularProgressIndicator()
            }
        } else if (error != null) {
            Box(
                modifier = Modifier.fillMaxSize(),
                contentAlignment = Alignment.Center
            ) {
                Text(
                    text = "Greška: $error",
                    color = MaterialTheme.colorScheme.error
                )
            }
        } else {
            LazyColumn(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(horizontal = 16.dp)
                    .semantics { testTag = "news_list" },
                verticalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                if (filteredNews.isEmpty()) {
                    item {
                        MessageCard("Nema pronađenih vijesti")
                    }
                } else {
                    items(filteredNews, key = { it.uuid }) { newsItem ->
                        if (newsItem.isFeatured) {
                            FeaturedNewsCard(
                                newsItem = newsItem,
                                onClick = { onNavigateToDetails(newsItem.uuid) }
                            )
                        } else {
                            StandardNewsCard(
                                newsItem = newsItem,
                                onClick = { onNavigateToDetails(newsItem.uuid) }
                            )
                        }
                    }
                }
            }
        }
    }
}