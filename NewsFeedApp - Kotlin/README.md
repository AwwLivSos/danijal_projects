# NewsFeedApp - Kotlin

A modern Android news feed application built with Kotlin and Jetpack Compose, featuring offline support and real-time news updates.

## 📱 Features

- **Modern UI**: Built with Jetpack Compose and Material Design 3
- **News Feed**: Browse and filter news articles with categories
- **News Details**: View detailed news articles with full content
- **Filtering**: Filter news by categories and tags
- **Offline Support**: Local database caching with Room
- **Image Loading**: Efficient image loading with Coil
- **Navigation**: Smooth navigation between screens
- **Saved News**: Save articles for offline reading

## 🏗️ Architecture

- **MVVM Pattern**: Clean architecture with ViewModels
- **Jetpack Compose**: Modern declarative UI toolkit
- **Room Database**: Local data persistence
- **Retrofit**: Network API calls
- **Navigation Component**: Type-safe navigation
- **Coroutines**: Asynchronous programming

## 🛠️ Tech Stack

- **Language**: Kotlin
- **UI Framework**: Jetpack Compose
- **Database**: Room
- **Network**: Retrofit + OkHttp
- **Image Loading**: Coil
- **Navigation**: Navigation Compose
- **Async Programming**: Kotlin Coroutines
- **Build System**: Gradle with Kotlin DSL

## 📋 Requirements

- Android Studio Arctic Fox or later
- JDK 11 or later
- Android SDK 24+ (Android 7.0+)
- Target SDK 35

## 📊 Preview

![Home page preview](images/screenshot.jpg)  

## 🚀 Getting Started

### Prerequisites

1. Install [Android Studio](https://developer.android.com/studio)
2. Ensure you have the latest Android SDK installed
3. Clone this repository

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/AwwLivSos/danijal_projects
   cd NewsFeedApp-Kotlin
   ```

2. **Open in Android Studio**
   - Launch Android Studio
   - Select "Open an existing Android Studio project"
   - Navigate to the cloned directory and select it

3. **Sync Project**
   - Android Studio will automatically sync the project
   - Wait for the Gradle sync to complete

4. **Run the App**
   - Connect an Android device or start an emulator
   - Click the "Run" button or press `Ctrl+R`

## 📁 Project Structure

```
app/
├── src/main/java/etf/ri/rma/newsfeedapp/
│   ├── MainActivity.kt                 # Main activity with navigation
│   ├── model/                         # Data models
│   │   ├── News.kt                   # News entity
│   │   └── NewsItem.kt               # News item model
│   ├── data/                         # Data layer
│   │   ├── NewsData.kt              # Data source
│   │   ├── NewsDatabase.kt          # Room database
│   │   ├── dao/                     # Data Access Objects
│   │   └── network/                 # Network layer
│   ├── screen/                       # UI screens
│   │   ├── NewsFeedScreen.kt        # Main news feed
│   │   ├── NewsDetailsScreen.kt     # News details
│   │   ├── FilterScreen.kt          # Filter options
│   │   └── NewsCards.kt             # News card components
│   └── ui/theme/                     # UI theme and styling
├── build.gradle.kts                  # App-level build configuration
└── AndroidManifest.xml              # App manifest
```

## 🔧 Dependencies

### Core Android
- **AndroidX Core KTX**: 1.15.0
- **Activity Compose**: 1.10.1
- **Lifecycle Runtime**: 2.8.7

### UI & Compose
- **Compose BOM**: 2024.09.00
- **Material3**: Latest from BOM
- **Navigation Compose**: 2.7.7

### Network & Data
- **Retrofit**: 2.9.0
- **OkHttp**: 4.12.0
- **Room**: 2.6.1
- **Coil**: 2.5.0

### Async Programming
- **Kotlin Coroutines**: 1.7.3

The app includes a modern, clean interface with:
- News feed with card-based layout
- Detailed news view with full article content
- Filter screen for category-based browsing
- Smooth animations and transitions

## 🧪 Testing

The project includes comprehensive testing setup:

- **Unit Tests**: JUnit 4.13.2
- **UI Tests**: Espresso Core 3.6.1
- **Compose Testing**: UI Test JUnit4
- **Mock Web Server**: OkHttp MockWebServer for network testing

Run tests using:
```bash
./gradlew test          # Unit tests
./gradlew connectedAndroidTest  # Instrumented tests
```

## 📱 Permissions

The app requires the following permissions:
- `INTERNET`: For fetching news data from APIs

## 🚀 Build Variants

- **Debug**: Development build with debugging enabled
- **Release**: Production build with optimizations

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 👥 Authors

- Danijal Alibegović. Developed as coursework for "Mobile app development"
