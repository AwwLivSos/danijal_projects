<<<<<<< HEAD
# Bookish - Android Book Search App

Bookish is a modern Android application built with Kotlin and Jetpack Compose that allows users to search for books using the Google Books API and manage their personal book collection.

## 📱 Features

- **Book Search**: Search for books using the Google Books API by title
- **Book Details**: View detailed information about books including authors, categories, and descriptions
- **Local Storage**: Save books to local Room database for offline access
- **Content Provider**: Share book data with other applications
- **Modern UI**: Beautiful Material 3 design with Jetpack Compose
- **Navigation**: Seamless navigation between screens using Navigation Compose
- **Image Loading**: Efficient image loading with Coil library

## 🛠️ Tech Stack

- **Language**: Kotlin
- **UI Framework**: Jetpack Compose
- **Architecture**: MVVM with Repository pattern
- **Database**: Room (SQLite)
- **Networking**: Retrofit with Gson
- **Image Loading**: Coil
- **Navigation**: Navigation Compose
- **Dependency Injection**: Manual (Repository pattern)

## 📋 Prerequisites

- Android Studio Hedgehog or later
- Android SDK 24+ (Android 7.0)
- Kotlin 1.9+
- JDK 11+

## 📊 Preview

![Home page preview](images/screenshot.png)  

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone <repository-url>
cd "Bookish - Kotlin"
```

### 2. Open in Android Studio

1. Launch Android Studio
2. Select "Open an existing project"
3. Navigate to the project directory and select it

### 3. Sync Project

1. Android Studio will automatically sync the project
2. Wait for Gradle sync to complete
3. Ensure all dependencies are downloaded

### 4. Run the App

1. Connect an Android device or start an emulator
2. Click the "Run" button (green play icon) in Android Studio
3. Select your target device and click "OK"

## 🏗️ Project Structure

```
app/src/main/java/ba/etfrma/bookish/
├── data/                    # Data layer
│   ├── AppDatabase.kt      # Room database configuration
│   ├── BookDao.kt          # Database access objects
│   ├── BookStaticData.kt   # Static data utilities
│   ├── GoogleBooksApi.kt   # API interface
│   └── RetrofitInstance.kt # Retrofit configuration
├── dto/                    # Data transfer objects
│   └── WebServiceDTO.kt    # API response models
├── model/                  # Domain models
│   ├── Book.kt            # Main book model with relations
│   └── Entities.kt        # Room database entities
├── navigation/             # Navigation
│   └── BookishNavGraph.kt # Navigation graph
├── provider/              # Content provider
│   └── BookContentProvider.kt
├── repository/            # Repository pattern
│   └── BookRepository.kt  # Data repository
├── ui/                   # UI layer
│   ├── components/       # Reusable UI components
│   │   └── BookCard.kt   # Book card component
│   ├── screens/          # App screens
│   │   ├── HomeScreen.kt
│   │   ├── BookDetailScreen.kt
│   │   └── BookDetailsViewModel.kt
│   └── theme/           # UI theme
│       ├── Color.kt
│       ├── Theme.kt
│       └── Type.kt
└── MainActivity.kt       # Main activity
```

## 📚 Key Components

### Database Schema

The app uses Room database with the following entities:
- **BookEntity**: Main book information
- **Author**: Book authors
- **Category**: Book categories
- **CrossRef tables**: Many-to-many relationships

### API Integration

- Integrates with Google Books API
- Uses Retrofit for network calls
- Implements proper error handling

### Content Provider

- Allows sharing book data with other apps
- Authority: `ba.etfrma.bookish.provider`

## 🔧 Configuration

### Permissions

The app requires:
- `INTERNET` permission for API calls

### Network Security

- Uses custom network security configuration
- Allows cleartext traffic for development

## 🎨 UI/UX Features

- **Material 3 Design**: Modern Material Design 3 components
- **Responsive Grid**: Books displayed in a 2-column grid
- **Search Functionality**: Real-time book search
- **Loading States**: Proper loading indicators
- **Navigation**: Smooth screen transitions
- **Snackbar Feedback**: User action confirmations

## 🧪 Testing

The project includes:
- Unit tests (`ExampleUnitTest.kt`)
- Instrumented tests (`ExampleInstrumentedTest.kt`)

Run tests using:
```bash
./gradlew test          # Unit tests
./gradlew connectedAndroidTest  # Instrumented tests
```

## 📦 Dependencies

### Core Dependencies
- `androidx.core:core-ktx`
- `androidx.lifecycle:lifecycle-runtime-ktx`
- `androidx.activity:activity-compose`

### UI Dependencies
- `androidx.compose.bom`
- `androidx.compose.ui`
- `androidx.compose.material3`
- `androidx.navigation:navigation-compose`

### Database Dependencies
- `androidx.room:room-runtime`
- `androidx.room:room-compiler` (KSP)

### Network Dependencies
- `com.squareup.retrofit2:retrofit`
- `com.squareup.retrofit2:converter-gson`

### Utility Dependencies
- `io.coil-kt:coil-compose` (Image loading)
- `org.jetbrains.kotlinx:kotlinx-coroutines-android`

## 🚀 Build Variants

- **Debug**: Development build with debugging enabled
- **Release**: Production build with optimizations

## 📱 Supported Android Versions

- **Minimum SDK**: 24 (Android 7.0)
- **Target SDK**: 35 (Android 15)
- **Compile SDK**: 35

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## 📄 License

This project is part of an educational exercise and is available under the [MIT License](LICENSE).

## 👨‍💻 Author

Created as part of Android development coursework at ETF (Elektrotehnički fakultet) Sarajevo.

---

**Note**: This app is for educational purposes and demonstrates modern Android development practices using Kotlin, Jetpack Compose, Room database, and REST API integration.
=======
# Bookish - Android Book Search App

Bookish is a modern Android application built with Kotlin and Jetpack Compose that allows users to search for books using the Google Books API and manage their personal book collection.

## 📱 Features

- **Book Search**: Search for books using the Google Books API by title
- **Book Details**: View detailed information about books including authors, categories, and descriptions
- **Local Storage**: Save books to local Room database for offline access
- **Content Provider**: Share book data with other applications
- **Modern UI**: Beautiful Material 3 design with Jetpack Compose
- **Navigation**: Seamless navigation between screens using Navigation Compose
- **Image Loading**: Efficient image loading with Coil library

## 🛠️ Tech Stack

- **Language**: Kotlin
- **UI Framework**: Jetpack Compose
- **Architecture**: MVVM with Repository pattern
- **Database**: Room (SQLite)
- **Networking**: Retrofit with Gson
- **Image Loading**: Coil
- **Navigation**: Navigation Compose
- **Dependency Injection**: Manual (Repository pattern)

## 📋 Prerequisites

- Android Studio Hedgehog or later
- Android SDK 24+ (Android 7.0)
- Kotlin 1.9+
- JDK 11+

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone <repository-url>
cd "Bookish - Kotlin"
```

### 2. Open in Android Studio

1. Launch Android Studio
2. Select "Open an existing project"
3. Navigate to the project directory and select it

### 3. Sync Project

1. Android Studio will automatically sync the project
2. Wait for Gradle sync to complete
3. Ensure all dependencies are downloaded

### 4. Run the App

1. Connect an Android device or start an emulator
2. Click the "Run" button (green play icon) in Android Studio
3. Select your target device and click "OK"

## 🏗️ Project Structure

```
app/src/main/java/ba/etfrma/bookish/
├── data/                    # Data layer
│   ├── AppDatabase.kt      # Room database configuration
│   ├── BookDao.kt          # Database access objects
│   ├── BookStaticData.kt   # Static data utilities
│   ├── GoogleBooksApi.kt   # API interface
│   └── RetrofitInstance.kt # Retrofit configuration
├── dto/                    # Data transfer objects
│   └── WebServiceDTO.kt    # API response models
├── model/                  # Domain models
│   ├── Book.kt            # Main book model with relations
│   └── Entities.kt        # Room database entities
├── navigation/             # Navigation
│   └── BookishNavGraph.kt # Navigation graph
├── provider/              # Content provider
│   └── BookContentProvider.kt
├── repository/            # Repository pattern
│   └── BookRepository.kt  # Data repository
├── ui/                   # UI layer
│   ├── components/       # Reusable UI components
│   │   └── BookCard.kt   # Book card component
│   ├── screens/          # App screens
│   │   ├── HomeScreen.kt
│   │   ├── BookDetailScreen.kt
│   │   └── BookDetailsViewModel.kt
│   └── theme/           # UI theme
│       ├── Color.kt
│       ├── Theme.kt
│       └── Type.kt
└── MainActivity.kt       # Main activity
```

## 📚 Key Components

### Database Schema

The app uses Room database with the following entities:
- **BookEntity**: Main book information
- **Author**: Book authors
- **Category**: Book categories
- **CrossRef tables**: Many-to-many relationships

### API Integration

- Integrates with Google Books API
- Uses Retrofit for network calls
- Implements proper error handling

### Content Provider

- Allows sharing book data with other apps
- Authority: `ba.etfrma.bookish.provider`

## 🔧 Configuration

### Permissions

The app requires:
- `INTERNET` permission for API calls

### Network Security

- Uses custom network security configuration
- Allows cleartext traffic for development

## 🎨 UI/UX Features

- **Material 3 Design**: Modern Material Design 3 components
- **Responsive Grid**: Books displayed in a 2-column grid
- **Search Functionality**: Real-time book search
- **Loading States**: Proper loading indicators
- **Navigation**: Smooth screen transitions
- **Snackbar Feedback**: User action confirmations

## 🧪 Testing

The project includes:
- Unit tests (`ExampleUnitTest.kt`)
- Instrumented tests (`ExampleInstrumentedTest.kt`)

Run tests using:
```bash
./gradlew test          # Unit tests
./gradlew connectedAndroidTest  # Instrumented tests
```

## 📦 Dependencies

### Core Dependencies
- `androidx.core:core-ktx`
- `androidx.lifecycle:lifecycle-runtime-ktx`
- `androidx.activity:activity-compose`

### UI Dependencies
- `androidx.compose.bom`
- `androidx.compose.ui`
- `androidx.compose.material3`
- `androidx.navigation:navigation-compose`

### Database Dependencies
- `androidx.room:room-runtime`
- `androidx.room:room-compiler` (KSP)

### Network Dependencies
- `com.squareup.retrofit2:retrofit`
- `com.squareup.retrofit2:converter-gson`

### Utility Dependencies
- `io.coil-kt:coil-compose` (Image loading)
- `org.jetbrains.kotlinx:kotlinx-coroutines-android`

## 🚀 Build Variants

- **Debug**: Development build with debugging enabled
- **Release**: Production build with optimizations

## 📱 Supported Android Versions

- **Minimum SDK**: 24 (Android 7.0)
- **Target SDK**: 35 (Android 15)
- **Compile SDK**: 35

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## 📄 License

This project is part of an educational exercise.

## 👨‍💻 Author

Created as part of Android development coursework at ETF (Elektrotehnički fakultet) Sarajevo for the course "Mobile app development". 
Developed by Danijal Alibegović

---

**Note**: This app is for educational purposes and demonstrates modern Android development practices using Kotlin, Jetpack Compose, Room database, and REST API integration.

