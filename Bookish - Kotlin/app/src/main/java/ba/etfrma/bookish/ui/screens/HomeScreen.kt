package ba.etfrma.bookish.ui.screens

import BookRepository
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.material3.Button
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.LineHeightStyle
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import ba.etfrma.bookish.data.BookStaticData
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.ui.components.BookCard
import kotlinx.coroutines.launch
import android.net.Uri
import androidx.compose.material3.SnackbarHostState
import androidx.compose.ui.platform.LocalContext

@Composable
fun HomeScreen(navController: NavController, initialSearch: String = "") {
    val booksList = remember { mutableStateListOf<Book>() }
    var searchQuery by remember { mutableStateOf(initialSearch) }
    var isLoading by remember { mutableStateOf(false) }
    val scope = rememberCoroutineScope()
    val context = LocalContext.current
    val repo = BookRepository(context)
    val snackbarHostState = remember { SnackbarHostState() }
    val repository = BookRepository(context)
    val localBooks = repository.loadBooksFromContentProvider(context)

    // Automatically trigger search if initialSearch is provided
    LaunchedEffect(initialSearch) {
        isLoading = true
        val result = if (initialSearch.isNotBlank()) {
            repo.searchBooks(initialSearch)
        } else {
            repository.getAllBooksFromDatabase()
        }
        booksList.clear()
        booksList.addAll(result)
        isLoading = false
    }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        // Search input and button
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            TextField(
                value = searchQuery,
                onValueChange = { searchQuery = it },
                label = { Text("Search by title") },
                modifier = Modifier.weight(1f)
            )
            Button(onClick = {
                isLoading = true
                booksList.clear()
                scope.launch {
                    val result = if (searchQuery.isNotBlank()) {
                        repo.searchBooks(searchQuery)
                    } else {
                        repository.getAllBooksFromDatabase()
                    }
                    booksList.addAll(result)
                    isLoading = false
                }
            }) {
                Text("Search")
            }
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Loading indicator
        if (isLoading) {
            CircularProgressIndicator(modifier = Modifier.align(Alignment.CenterHorizontally))
        }

        // Display search results
        LazyVerticalGrid(columns = GridCells.Fixed(2)) {
            items(booksList) { book ->
                BookCard(book = book) {
                    scope.launch {
                        val localBook = repository.getBookByIdDatabase(book.book.id)
                        if (localBook == null) {
                            repository.saveToLocal(book)
                            snackbarHostState.showSnackbar("The book is saved to database.")
                        }
                        navController.navigate("details/${book.book.id}")
                    }
                }
            }
        }
    }
}