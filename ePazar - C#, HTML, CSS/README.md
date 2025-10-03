# ePazar - Online Marketplace Platform

A comprehensive ASP.NET Core 9.0 web application for an online marketplace platform (ePazar) built with modern web technologies. This platform allows users to buy and sell items with integrated delivery services, user management, and notification systems.

## 🚀 Features

### Core Functionality
- **Product Management**: Users can create, edit, and manage product listings
- **Order System**: Complete order processing with delivery tracking
- **User Management**: Role-based authentication (Admin, Courier Service, User)
- **Search & Filter**: Advanced search with category filtering and sorting
- **Notifications**: Real-time notifications and email alerts
- **Following System**: Users can follow other users to get notified about new products

### User Roles
- **Admin**: Full system access, user management, order oversight
- **Kurirska_Sluzba (Courier Service)**: Handles order delivery and status updates
- **Korisnik (User)**: Can create listings, place orders, and follow other users

### Product Categories
- Elektronika (Electronics)
- Odjeca (Clothing)
- Obuca (Footwear)
- Kuca_i_Vrt (Home & Garden)
- Sport_i_Rekreacija (Sports & Recreation)
- Automobili (Cars)
- Nekretnine (Real Estate)
- Knjige (Books)
- Igracke (Toys)
- Ljepota_i_Zdravlje (Beauty & Health)
- Usluge (Services)
- Umjetnost_i_Kolekcije (Art & Collectibles)
- Hrana_i_Pice (Food & Beverages)
- Posao_i_Industrija (Business & Industrial)
- Kucni_Ljubimci (Pet Supplies)
- Ostalo (Other)

## 🛠️ Technology Stack

### Backend
- **ASP.NET Core 9.0** - Web framework
- **Entity Framework Core 9.0** - ORM with SQL Server
- **ASP.NET Core Identity** - Authentication and authorization
- **Bootstrap 5** - Frontend framework
- **Markdig** - Markdown processing
- **iText7** - PDF generation

### Database
- **Microsoft SQL Server** - Primary database
- **Entity Framework Migrations** - Database versioning

### Frontend
- **Razor Views** - Server-side rendering
- **Bootstrap Icons** - Icon library
- **Custom CSS** - Responsive design

## 📁 Project Structure

```
ooadepazar/
├── Controllers/          # MVC Controllers
│   ├── ArtikalController.cs      # Product management
│   ├── NarudzbaController.cs     # Order processing
│   ├── HomeController.cs         # Main page and search
│   ├── KorisnikController.cs     # User management
│   └── NotifikacijaController.cs # Notification system
├── Models/               # Data models
│   ├── Artikal.cs               # Product entity
│   ├── Narudzba.cs              # Order entity
│   ├── ApplicationUser.cs       # User entity
│   ├── Notifikacija.cs          # Notification entity
│   └── ViewModels/              # View-specific models
├── Views/                # Razor views
│   ├── Home/                    # Main page views
│   ├── Artikal/                 # Product views
│   ├── Narudzba/                # Order views
│   └── Shared/                  # Layout and partial views
├── Data/                 # Database context
├── Services/             # Business logic services
├── Areas/                # Identity area
└── wwwroot/              # Static files
```

## 🚀 Getting Started

### Prerequisites
- .NET 9.0 SDK
- Microsoft SQL Server
- Visual Studio 2022 or VS Code

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/AwwLivSos/danijal_projects
   cd ePazar-C#-HTML-CSS
   ```

2. **Configure the database**
   - Update the connection string in `appsettings.json`
   - Run the following commands:
   ```bash
   cd ooadepazar
   dotnet ef database update
   ```

3. **Install dependencies**
   ```bash
   dotnet restore
   ```

4. **Run the application**
   ```bash
   dotnet run
   ```

5. **Access the application**
   - Navigate to `https://localhost:5001` or `http://localhost:5000`

## 🔧 Configuration

### Database Connection
Update the connection string in `appsettings.json`:
```json
{
  "ConnectionStrings": {
    "DefaultConnection": "Server=(localdb)\\mssqllocaldb;Database=ePazarDB;Trusted_Connection=true;MultipleActiveResultSets=true"
  }
}
```

### Email Configuration
Configure email settings for notifications in `appsettings.json`:
```json
{
  "EmailSettings": {
    "SmtpServer": "your-smtp-server",
    "Port": 587,
    "Username": "your-email",
    "Password": "your-password"
  }
}
```

## 📱 Features Overview

### Product Management
- Create product listings with images, descriptions, and pricing
- Categorize products using predefined categories
- Set product condition (New/Used)
- Location-based product filtering

### Order Processing
- Place orders for available products
- Select courier service for delivery
- Track order status (Created → In Progress → Delivered)
- Email notifications for status updates

### User Experience
- Responsive design for all devices
- Advanced search with multiple filters
- User profiles and following system
- Real-time notifications

### Admin Features
- User role management
- System-wide oversight
- Access to all orders and products

## 🔐 Security Features

- **Authentication**: ASP.NET Core Identity
- **Authorization**: Role-based access control
- **CSRF Protection**: Anti-forgery tokens
- **Input Validation**: Server-side validation
- **SQL Injection Protection**: Entity Framework

## 📊 Database Schema

### Key Entities
- **Artikal**: Products with details, pricing, and availability
- **Narudzba**: Orders with status tracking and delivery information
- **ApplicationUser**: Extended user model with roles and profile information
- **Notifikacija**: User notifications and alerts
- **Pracenje**: User following relationships

## 🎨 UI/UX Features

- **Responsive Design**: Mobile-first approach
- **Modern Interface**: Clean, intuitive design
- **Visual Feedback**: Hover effects and transitions
- **Accessibility**: ARIA labels and semantic HTML
- **Performance**: Optimized images and lazy loading

## 🚀 Deployment

### Production Deployment
1. Configure production connection string
2. Set up email service
3. Configure HTTPS certificates
4. Deploy to Azure, IIS, or Docker

### Docker Support
```dockerfile
FROM mcr.microsoft.com/dotnet/aspnet:9.0
COPY . /app
WORKDIR /app
EXPOSE 80
ENTRYPOINT ["dotnet", "ooadepazar.dll"]
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 👥 Authors

- [Bakir Činjarević](bcinjarevi1@etf.unsa.ba) - Team lead
- [Enis Adilović](eadilovic1@etf.unsa.ba) - Database 
- [Amar Handanagić](ahandanagi1@etf.unsa.ba) - Optimization
- Danijal Alibegović - Frontend

---

**Note**: This is a university project (OOAD - Object-Oriented Analysis and Design) demonstrating modern web development practices with ASP.NET Core.