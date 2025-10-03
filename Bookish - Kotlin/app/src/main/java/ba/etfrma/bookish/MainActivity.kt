package ba.etfrma.bookish

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import android.content.Intent
import ba.etfrma.bookish.navigation.BookishNavGraph
import ba.etfrma.bookish.ui.theme.BookishTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val sharedText = intent?.getStringExtra(Intent.EXTRA_TEXT)

        setContent {
            BookishTheme  {
                BookishNavGraph(startText = sharedText)
            }
        }
    }
}