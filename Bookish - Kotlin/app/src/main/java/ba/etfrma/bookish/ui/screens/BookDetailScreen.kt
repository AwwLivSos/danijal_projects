package ba.etfrma.bookish.ui.screens

import android.content.ActivityNotFoundException
import android.content.Intent
import android.net.Uri
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import ba.etfrma.bookish.model.Book
import androidx.compose.ui.platform.LocalContext
import ba.etfrma.bookish.R
import androidx.core.net.toUri
import coil.compose.AsyncImage


@Composable
fun BookDetailsScreen(bookId: String, onBack: () -> Unit, viewModel: BookDetailsViewModel = androidx.lifecycle.viewmodel.compose.viewModel()) {
    val bookDetails by viewModel.bookDetails.collectAsState()

    LaunchedEffect(bookId) {
        viewModel.fetchBookDetails(bookId)
    }

    if (bookDetails != null) {
        val book = bookDetails!!
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            AsyncImage(
                model = book.book.thumbnail,
                contentDescription = "Book cover",
                modifier = Modifier
                    .height(200.dp)
                    .fillMaxWidth()
            )

            Text(
                text = book.book.title,
                style = MaterialTheme.typography.headlineMedium,
                modifier = Modifier.fillMaxWidth(),
                textAlign = TextAlign.Center
            )

            Text("Authors: ${book.authors.joinToString(", ")}", style = MaterialTheme.typography.bodyMedium)
            Text("Publisher: ${book.book.publisher}", style = MaterialTheme.typography.bodyMedium)
            Text("Categories: ${book.categories.joinToString(", ")}", style = MaterialTheme.typography.bodyMedium)

            val context = LocalContext.current
            Text(
                text = book.book.infoLink,
                color = Color.Blue,
                style = MaterialTheme.typography.bodyMedium,
                modifier = Modifier.clickable {
                    val intent = Intent(Intent.ACTION_VIEW, book.book.infoLink.toUri())
                    context.startActivity(intent)
                }
            )

            Text(
                text = book.book.description,
                style = MaterialTheme.typography.bodyLarge
            )
            Button(onClick = {
                scope.launch {
                    repository.deleteBook(book)
                    snackbarHostState.showSnackbar("Book removed from database.")
                }
            }) {
                Text("Remove from database")
            }
        }
    } else {
        Text("Loading...", modifier = Modifier.fillMaxSize(), textAlign = TextAlign.Center)
    }
}