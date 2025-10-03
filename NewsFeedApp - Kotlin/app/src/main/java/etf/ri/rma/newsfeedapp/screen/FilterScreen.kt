package etf.ri.rma.newsfeedapp.screen

import android.app.DatePickerDialog
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.semantics.semantics
import androidx.compose.ui.semantics.testTag
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import java.util.Calendar

@OptIn(ExperimentalLayoutApi::class)
@Composable
fun FilterScreen(
    navController: NavController,
    onNavigateBack: () -> Unit
) {
    val currentBackStackEntry = navController.previousBackStackEntry
    val initialCategory = navController.previousBackStackEntry?.savedStateHandle?.get<String>("category") ?: "All"

    var selectedCategory by rememberSaveable { mutableStateOf(initialCategory) }
    var dateRange by rememberSaveable { mutableStateOf<String?>(null) }
    var unwantedWord by rememberSaveable { mutableStateOf("") }
    var unwantedWords by rememberSaveable { mutableStateOf(emptyList<String>()) }
    var showDatePicker by remember { mutableStateOf(false) }
    var isSelectingStartDate by remember { mutableStateOf(true) }
    var startDate by remember { mutableStateOf("") }
    var endDate by remember { mutableStateOf("") }

    val categoryDisplayToApi = mapOf(
        "All" to "All",
        "Politika" to "politics",
        "Sport" to "sports",
        "Filmovi" to "entertainment",
        "Lokalno" to "world", // or your intended mapping
        "Nauka/tehnologija" to "science" // or "tech" if you want
    )

    Box(modifier = Modifier.fillMaxSize()) {
        IconButton(
            onClick = onNavigateBack,
            modifier = Modifier
                .padding(16.dp)
                .semantics { testTag = "filter_back_button" }
        ) {
            Text("←")
        }

        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp)
        ) {
            Text(
                text = "Kategorije",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.padding(top = 48.dp, bottom = 8.dp)
            )
            FlowRow(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(bottom = 16.dp),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                FilterChip(
                    selected = selectedCategory == "All",
                    onClick = { selectedCategory = "All" },
                    label = { Text("All") },
                    modifier = Modifier.semantics { testTag = "filter_chip_all" }
                )
                FilterChip(
                    selected = selectedCategory == "Politika",
                    onClick = { selectedCategory = "Politika" },
                    label = { Text("Politika") },
                    modifier = Modifier.semantics { testTag = "filter_chip_pol" }
                )
                FilterChip(
                    selected = selectedCategory == "Sport",
                    onClick = { selectedCategory = "Sport" },
                    label = { Text("Sport") },
                    modifier = Modifier.semantics { testTag = "filter_chip_spo" }
                )
                FilterChip(
                    selected = selectedCategory == "Filmovi",
                    onClick = { selectedCategory = "Filmovi" },
                    label = { Text("Filmovi") },
                    modifier = Modifier.semantics { testTag = "filter_chip_fil" }
                )
                FilterChip(
                    selected = selectedCategory == "Lokalno",
                    onClick = { selectedCategory = "Lokalno" },
                    label = { Text("Lokalno") },
                    modifier = Modifier.semantics { testTag = "filter_chip_loc" }
                )
                FilterChip(
                    selected = selectedCategory == "Nauka/tehnologija",
                    onClick = { selectedCategory = "Nauka/tehnologija" },
                    label = { Text("Nauka/tehnologija") },
                    modifier = Modifier.semantics { testTag = "filter_chip_sci" }
                )
            }

            Text(
                text = "Opseg datuma",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.padding(bottom = 8.dp)
            )

            if (showDatePicker) {
                val calendar = Calendar.getInstance()
                val title = if (isSelectingStartDate) "Odaberite početni datum" else "Odaberite krajnji datum"
                
                DatePickerDialog(
                    LocalContext.current,
                    { _, year, month, dayOfMonth ->
                        val selectedDate = String.format("%04d-%02d-%02d", year, month + 1, dayOfMonth)
                        if (isSelectingStartDate) {
                            startDate = selectedDate
                            isSelectingStartDate = false
                            showDatePicker = true
                        } else {
                            if (selectedDate < startDate) {
                                return@DatePickerDialog
                            }
                            endDate = selectedDate
                            dateRange = "$startDate;$endDate"
                            showDatePicker = false
                        }
                    },
                    calendar.get(Calendar.YEAR),
                    calendar.get(Calendar.MONTH),
                    calendar.get(Calendar.DAY_OF_MONTH)
                ).apply {
                    setTitle(title)
                    show()
                }
            }

            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(bottom = 16.dp),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Text(
                    text = dateRange ?: "Odaberite opseg datuma",
                    modifier = Modifier
                        .weight(1f)
                        .semantics { testTag = "filter_daterange_display" }
                )
                Button(
                    onClick = {
                        isSelectingStartDate = true
                        showDatePicker = true
                    },
                    modifier = Modifier.semantics { testTag = "filter_daterange_button" }
                ) {
                    Text("Odaberi")
                }
            }

            Text(
                text = "Nepoželjne riječi",
                style = MaterialTheme.typography.titleMedium,
                modifier = Modifier.padding(bottom = 8.dp)
            )
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(bottom = 8.dp),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                TextField(
                    value = unwantedWord,
                    onValueChange = { unwantedWord = it },
                    modifier = Modifier
                        .weight(1f)
                        .semantics { testTag = "filter_unwanted_input" },
                    placeholder = { Text("Unesite riječ") }
                )
                Button(
                    onClick = {
                        if (unwantedWord.isNotBlank() && !unwantedWords.map { it.lowercase() }.contains(unwantedWord.lowercase())) {
                            unwantedWords = unwantedWords + unwantedWord
                            unwantedWord = ""
                        }
                    },
                    modifier = Modifier.semantics { testTag = "filter_unwanted_add_button" }
                ) {
                    Text("Dodaj")
                }
            }

            LazyColumn(
                modifier = Modifier
                    .fillMaxWidth()
                    .height(100.dp)
                    .semantics { testTag = "filter_unwanted_list" }
            ) {
                items(unwantedWords) { word ->
                    Row(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(vertical = 4.dp),
                        horizontalArrangement = Arrangement.SpaceBetween
                    ) {
                        Text(word)
                        IconButton(
                            onClick = { unwantedWords = unwantedWords - word }
                        ) {
                            Text("×")
                        }
                    }
                }
            }

            Spacer(modifier = Modifier.weight(1f))

            Button(
                onClick = {
                    navController.previousBackStackEntry?.savedStateHandle?.apply {
                        remove<String>("category")
                        remove<String>("dateRange")
                        remove<List<String>>("unwantedWords")

                        set("category", categoryDisplayToApi[selectedCategory] ?: "All")
                        set("dateRange", dateRange)
                        set("unwantedWords", unwantedWords)
                    }
                    onNavigateBack()
                },
                modifier = Modifier
                    .fillMaxWidth()
                    .semantics { testTag = "filter_apply_button" }
            ) {
                Text("Primijeni")
            }
        }
    }
}