package ba.etfrma.bookish.navigation

import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.navigation.NavBackStackEntry
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.rememberNavController
import androidx.navigation.compose.composable
import androidx.navigation.navArgument
import ba.etfrma.bookish.data.BookStaticData
import ba.etfrma.bookish.ui.screens.BookDetailsScreen
import ba.etfrma.bookish.ui.screens.HomeScreen
import android.net.Uri

@Composable
fun BookishNavGraph(startText: String?) {
    val navController = rememberNavController()

    NavHost(navController = navController, startDestination = "home") {
        composable("home") {
            HomeScreen(navController = navController)
        }
        composable(
            "details/{title}",
            arguments = listOf(navArgument("title") { type = NavType.StringType })
        ) { backStackEntry: NavBackStackEntry ->
            val title = backStackEntry.arguments?.getString("title")?.let { Uri.decode(it) } ?: ""

            val book = BookStaticData.getSampleBooks()
                .firstOrNull { it.book.title.equals(title, ignoreCase = true) }

            if (book != null) {
                BookDetailsScreen(bookId = book.book.id, onBack = { navController.popBackStack() })
            } else {
                Text(text = "Book not found")
            }
        }
    }
}