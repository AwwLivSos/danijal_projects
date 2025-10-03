package etf.ri.rma.newsfeedapp.screen

import android.os.Build
import androidx.activity.compose.BackHandler
import androidx.annotation.RequiresApi
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.semantics.testTag
import androidx.compose.ui.unit.dp
import coil.compose.rememberAsyncImagePainter
import coil.request.ImageRequest
import etf.ri.rma.newsfeedapp.dao.NewsDAO
import etf.ri.rma.newsfeedapp.data.NewsData
import etf.ri.rma.newsfeedapp.model.NewsItem
import etf.ri.rma.newsfeedapp.model.Tag

@OptIn(ExperimentalLayoutApi::class)
@RequiresApi(Build.VERSION_CODES.O)
@Composable
fun NewsDetailsScreen(
    newsId: String,
    onNavigateBack: () -> Unit,
    onNewsClick: (String) -> Unit
) {
    BackHandler {
        onNavigateBack()
    }

    if (newsId.isEmpty()) {
        Text("Invalid news ID", modifier = Modifier.fillMaxSize())
        return
    }

    val newsItem = remember { NewsData.getAllNews().find { it.uuid == newsId } }
    val scope = rememberCoroutineScope()
    val newsDAO = remember { NewsDAO.getInstance() }
    var imageTags by remember { mutableStateOf<List<Tag>>(emptyList()) }
    var isLoadingTags by remember { mutableStateOf(false) }
    var tagError by remember { mutableStateOf<String?>(null) }
    var relatedNews by remember { mutableStateOf<List<NewsItem>>(emptyList()) }
    var isLoadingRelated by remember { mutableStateOf(false) }
    var relatedError by remember { mutableStateOf<String?>(null) }

    LaunchedEffect(newsItem) {
        if (newsItem != null) {
            // Load image tags
            if (newsItem.imageUrl != null) {
                isLoadingTags = true
                tagError = null
                try {
                    imageTags = newsDAO.getTags(newsItem.imageUrl)
                } catch (e: Exception) {
                    tagError = e.message
                } finally {
                    isLoadingTags = false
                }
            }

            // Load similar stories
            isLoadingRelated = true
            relatedError = null
            try {
                relatedNews = newsDAO.getSimilarStories(newsItem.uuid)
                if (relatedNews.isEmpty()) {
                    relatedError = "No similar news found."
                }
            } catch (e: Exception) {
                relatedError = "No similar news found."
            } finally {
                isLoadingRelated = false
            }
        }
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        Button(
            onClick = onNavigateBack,
            modifier = Modifier
                .align(Alignment.End)
                .semantics { testTag = "details_close_button" }
        ) {
            Text("Zatvori detalje")
        }

        if (newsItem != null) {
            LazyColumn(
                modifier = Modifier.fillMaxSize(),
                verticalArrangement = Arrangement.spacedBy(16.dp)
            ) {
                item {
                    Text(
                        text = newsItem.title,
                        style = MaterialTheme.typography.headlineMedium,
                        modifier = Modifier.semantics { testTag = "details_title" }
                    )
                }

                if (newsItem.imageUrl != null) {
                    item {
                        Image(
                            painter = rememberAsyncImagePainter(
                                ImageRequest.Builder(LocalContext.current)
                                    .data(newsItem.imageUrl)
                                    .crossfade(true)
                                    .build()
                            ),
                            contentDescription = newsItem.title,
                            modifier = Modifier
                                .fillMaxWidth()
                                .height(200.dp),
                            contentScale = ContentScale.Crop
                        )
                    }

                    item {
                        if (isLoadingTags) {
                            CircularProgressIndicator(
                                modifier = Modifier.align(Alignment.CenterHorizontally)
                            )
                        } else if (tagError != null) {
                            Text(
                                text = "Greška pri učitavanju tagova: $tagError",
                                color = MaterialTheme.colorScheme.error
                            )
                        } else if (imageTags.isNotEmpty()) {
                            Column {
                                Text(
                                    text = "Tagovi slike:",
                                    style = MaterialTheme.typography.titleMedium
                                )
                                FlowRow(
                                    modifier = Modifier.fillMaxWidth(),
                                    horizontalArrangement = Arrangement.spacedBy(8.dp)
                                ) {
                                    imageTags.forEach { tag ->
                                        AssistChip(
                                            onClick = { },
                                            label = { Text(tag.value) }
                                        )
                                    }
                                }
                            }
                        }
                    }
                }

                item {
                    Text(
                        text = newsItem.snippet,
                        style = MaterialTheme.typography.bodyLarge,
                        modifier = Modifier.semantics { testTag = "details_snippet" }
                    )
                }

                item {
                    Text(
                        text = "Kategorija: ${newsItem.category}",
                        style = MaterialTheme.typography.bodyMedium,
                        modifier = Modifier.semantics { testTag = "details_category" }
                    )
                }

                item {
                    Text(
                        text = "Izvor: ${newsItem.source}",
                        style = MaterialTheme.typography.bodyMedium,
                        modifier = Modifier.semantics { testTag = "details_source" }
                    )
                }

                item {
                    Text(
                        text = "Datum objave: ${newsItem.publishedDate}",
                        style = MaterialTheme.typography.bodyMedium,
                        modifier = Modifier.semantics { testTag = "details_date" }
                    )
                }

                if (isLoadingRelated) {
                    item {
                        Box(
                            modifier = Modifier.fillMaxWidth(),
                            contentAlignment = Alignment.Center
                        ) {
                            CircularProgressIndicator()
                        }
                    }
                } else if (relatedError != null) {
                    item {
                        Text(
                            text = relatedError ?: "No similar news found.",
                            color = MaterialTheme.colorScheme.error
                        )
                    }
                } else if (relatedNews.isNotEmpty()) {
                    item {
                        Text(
                            text = "Povezane vijesti iz iste kategorije",
                            style = MaterialTheme.typography.titleMedium,
                            modifier = Modifier.padding(vertical = 8.dp)
                        )
                    }

                    items(relatedNews) { relatedItem ->
                        TextButton(
                            onClick = { onNewsClick(relatedItem.uuid) },
                            modifier = Modifier
                                .fillMaxWidth()
                                .semantics {
                                    testTag = "related_news_${relatedItem.uuid}"
                                }
                        ) {
                            Text(
                                text = relatedItem.title,
                                style = MaterialTheme.typography.bodyMedium
                            )
                        }
                    }
                }
            }
        } else {
            Text("Vijest nije pronađena")
        }
    }
}