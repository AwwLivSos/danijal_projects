package Model;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.sql.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.Date;

public class OsobaModel {
    private ObservableList<Osoba> osobe;            //sad je ovo kao kontejnerksa kolekcija za klasu
    private static final String DATABASE_URL = "jdbc:sqlite:baza.db";

    private static Connection connect() throws SQLException {
        return DriverManager.getConnection(DATABASE_URL);
    }

    private static OsobaModel instance = null;
    public static OsobaModel getInstance() {
        if (instance == null) {
            instance = new OsobaModel();
        }
        return instance;
    }
    public static void removeInstance() {
        instance = null;
    }

    public OsobaModel() {
        osobe = FXCollections.observableArrayList();
    }

    public String dodajOsobu(Integer id, String ime, String prezime, String adresa, Date datumRodjenja, String maticniBroj, Uloga uloga){
        try{
            Osoba newOsoba = new Osoba(id, ime, prezime, adresa, datumRodjenja, maticniBroj, uloga);
            osobe.add(newOsoba);
            return "Osoba je uspjesno dodana!";
        }
        catch(IllegalArgumentException e){
            return e.getMessage();              //uhvacen izuzetak i vracena njegova poruka kao string
        }
    }

    public ObservableList<Osoba> dajSviju() {
        return osobe;
    }

    public static Osoba dajOsobuPoId(Integer id) {
        Osoba osoba = null;
        String upit = "SELECT * FROM Osoba WHERE id = ?";
        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(upit)) {
            pstmt.setInt(1, id);
            ResultSet rs = pstmt.executeQuery();
            if (rs.next()) {
                osoba = new Osoba(
                        rs.getInt("id"),
                        rs.getString("ime"),
                        rs.getString("prezime"),
                        rs.getString("adresa"),
                        OsobaModel.dateFormat.parse(rs.getString("datumRodjenja")),
                        rs.getString("maticniBroj"),
                        Uloga.valueOf(rs.getString("uloga"))
                );
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        } catch (ParseException e) {
            System.out.println(e.getMessage());
        }
        return osoba;
    }

    public boolean provjeriMaticniBroj(String maticniBroj, Date datumRodjenja) {
        boolean danIsti = datumRodjenja.getDate() == Integer.parseInt(maticniBroj.substring(0, 2)),
                mjesecIsti = datumRodjenja.getMonth()+1 == Integer.parseInt(maticniBroj.substring(2, 4)),
                godinaIsta = (datumRodjenja.getYear()+900 > 1000 ? datumRodjenja.getYear()+900-1000 : datumRodjenja.getYear()+900)  == Integer.parseInt(maticniBroj.substring(4, 7));
        return (danIsti && mjesecIsti && godinaIsta);
    }

    public String azurirajOsobu(Integer id, String novoIme, String novoPrezime, String novaAdresa, Date noviDatumRodjenja, String noviMaticniBroj, Uloga novaUloga) {
        if(!provjeriMaticniBroj(noviMaticniBroj, noviDatumRodjenja)) {
            noviMaticniBroj = null;
        }
        if (novoIme.length() < 2 || novoIme.length() > 50) {
            novoIme = null;
        }
        Osoba trazenaOsoba = dajOsobuPoId(id);
        if(trazenaOsoba != null) {
            try {
                if (novoIme != null) {
                    trazenaOsoba.setIme(novoIme);
                }
                if (novoPrezime != null) {
                    trazenaOsoba.setPrezime(novoPrezime);
                }
                if (novaAdresa != null) {
                    trazenaOsoba.setAdresa(novaAdresa);
                }
                if (noviDatumRodjenja != null) {
                    trazenaOsoba.setDatumRodjenja(noviDatumRodjenja);
                }
                if (noviMaticniBroj != null) {
                    trazenaOsoba.setMaticniBroj(noviMaticniBroj);
                }
                if (novaUloga != null){
                    trazenaOsoba.setUloga(novaUloga);
                }
                return "Osoba je uspjesno azurirana!";
            }
            catch (IllegalArgumentException e) {
                return e.getMessage();
            }
        }
        return "Osoba nije pronadjena!";
    }

    public String obrisiOsobu(Integer id){
        if (osobe.removeIf(osoba -> osoba.getId() == id)) {
            return "Osoba je uspjesno obrisana!";
        } else {
            return "Osoba nije pronadjena!";
        }
    }

    public void napuni(){
        osobe.add(new Osoba(1,"Neko","Nekic","Neka adresa", new Date(97,8,25), "2509997123456", Uloga.STUDENT));
        osobe.add(new Osoba(2,"Neko 2","Nekic 2","Neka adresa 2",new Date(97,8,25), "2509997123456", Uloga.NASTAVNO_OSOBLJE));
    }

    public static void kreirajTabeluAkoNePostoji() {
        String kreirajOsobaTabeluSql = """
         CREATE TABLE IF NOT EXISTS Osoba (
         id INTEGER,
         ime TEXT,
         prezime TEXT,
         adresa TEXT,
         datumRodjenja TEXT,
         maticniBroj TEXT,
         uloga TEXT
         );
         """;
        try (Connection conn = connect();
             Statement stmt = conn.createStatement()) {
            stmt.execute(kreirajOsobaTabeluSql);
            System.out.println("Tabela je kreirana ili vec postoji!");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void napuniInicijalnimPodacima() {
        String insertSQL = """
         INSERT INTO Osoba (id, ime, prezime, adresa, datumRodjenja, maticniBroj, uloga)
         VALUES (?, ?, ?, ?, ?, ?, ?);
         """;
        try (Connection conn = connect();
             PreparedStatement pstmt = conn.prepareStatement(insertSQL))
        {
            pstmt.setInt(1, 1);
            pstmt.setString(2, "John");
            pstmt.setString(3, "Doe");
            pstmt.setString(4, "Some Address");
            pstmt.setString(5, "1995-01-15");
            pstmt.setString(6, "1501995123456");
            pstmt.setString(7, "STUDENT");
            pstmt.executeUpdate();
            pstmt.setInt(1, 2);
            pstmt.setString(2, "Alice");
            pstmt.setString(3, "Alister");
            pstmt.setString(4, "Another Address");
            pstmt.setString(5, "1980-05-20");
            pstmt.setString(6, "2005980444444");
            pstmt.setString(7, "NASTAVNO_OSOBLJE");
            pstmt.executeUpdate();
            System.out.println("Ubaceni pocetni podaci!");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void isprazniTabeluOsoba() {
        String upit = "DELETE FROM Osoba";
        try (Connection conn = connect();
             Statement stmt = conn.createStatement()) {
            int brojObrisanihRedova = stmt.executeUpdate(upit);
            System.out.println("Obrisani redovi tabele. Broj obrisanih redova: " + brojObrisanihRedova);
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
    }

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    public static List<Osoba> dajSveOsobe() throws Exception{
        List<Osoba> osobe = new ArrayList<>();
        String upit = "SELECT * FROM Osoba";
        try (Connection conn = connect();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(upit)) {
             while (rs.next()) {
                Osoba osoba = new Osoba(
                        rs.getInt("id"),
                        rs.getString("ime"),
                        rs.getString("prezime"),
                        rs.getString("adresa"),
                        OsobaModel.dateFormat.parse(rs.getString("datumRodjenja")),
                        rs.getString("maticniBroj"),
                        Uloga.valueOf(rs.getString("uloga"))
                );
                osobe.add(osoba);
             }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        catch (ParseException e) {
            System.out.println(e.getMessage());
        }
        return osobe;
    }

    public String obrisiOsobuPoId(Integer id) {
        Osoba osoba = dajOsobuPoId(id);
        if(osoba == null) {
            return "Ne postoji osoba sa datim id-em";
        }
        String upit = "DELETE FROM osoba WHERE id = ?";
        try (Connection konekcija = DriverManager.getConnection(DATABASE_URL);
             PreparedStatement ps = konekcija.prepareStatement(upit)) {
            ps.setInt(1, id);
            int brojObrisanihRedova = ps.executeUpdate();
            return "Osoba je uspjesno obrisana";
        } catch (SQLException e) {
            return e.getMessage();
        }
    }

}
