package etf.ri.rma.newsfeedapp

import android.os.Build
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.annotation.RequiresApi
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.ui.Modifier
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import etf.ri.rma.newsfeedapp.screen.FilterScreen
import etf.ri.rma.newsfeedapp.screen.NewsDetailsScreen
import etf.ri.rma.newsfeedapp.screen.NewsFeedScreen
import etf.ri.rma.newsfeedapp.ui.theme.NewsFeedAppTheme

class MainActivity : ComponentActivity() {
    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            NewsFeedAppTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    val navController = rememberNavController()

                    NavHost(
                        navController = navController,
                        startDestination = "home"
                    ) {
                        composable("home") {
                            NewsFeedScreen(
                                navController = navController,
                                onNavigateToFilters = {
                                    navController.navigate("filters")
                                },
                                onNavigateToDetails = { newsId ->
                                    navController.navigate("details/$newsId")
                                }
                            )
                        }
                        composable("filters") {
                            FilterScreen(
                                navController = navController,
                                onNavigateBack = {
                                    navController.popBackStack()
                                }
                            )
                        }
                        composable("details/{newsId}") { backStackEntry ->
                            val newsId = backStackEntry.arguments?.getString("newsId") ?: ""
                            NewsDetailsScreen(
                                newsId = newsId,
                                onNavigateBack = {
                                    navController.navigate("home") {
                                        popUpTo("home") { inclusive = true }
                                    }
                                },
                                onNewsClick = { newNewsId ->
                                    navController.navigate("details/$newNewsId")
                                }
                            )
                        }
                    }
                }
            }
        }
    }
}