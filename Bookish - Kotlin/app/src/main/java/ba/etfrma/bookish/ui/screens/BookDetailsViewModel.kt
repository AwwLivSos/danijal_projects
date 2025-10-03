package ba.etfrma.bookish.ui.screens

import BookRepository
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import ba.etfrma.bookish.model.Book
import ba.etfrma.bookish.data.BookStaticData

class BookDetailsViewModel(private val repository: BookRepository) : ViewModel() {
    private val _bookDetails = MutableStateFlow<Book?>(null)
    val bookDetails: StateFlow<Book?> = _bookDetails

    fun fetchBookDetails(id: String) {
        viewModelScope.launch {
            try {
                val book = repository.getBookById(id)
                _bookDetails.value = book
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}
