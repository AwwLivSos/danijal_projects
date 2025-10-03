import java.sql.*;
import java.util.ArrayList;
import java.io.File;

final public class PodaciDAO {
    private static Connection connection;

    private static Connection connect() {
        try {
            File baza = new File("folder/baza.db");
            if (connection == null) {
                connection = DriverManager.getConnection("jdbc:sqlite:" + baza.getAbsolutePath());
            }
        } catch (SQLException e) {
            System.out.println(e.getMessage());
            connection = null;
        }
        return connection;
    }

    public static ArrayList<String> dajImena() {
        ArrayList<String> imena = new ArrayList<>();
        Connection c = connect();
        if(c==null) return imena;
        String upit = "SELECT ime FROM kontakt";
        try(PreparedStatement ps = c.prepareStatement(upit);
            ResultSet rs = ps.executeQuery()){
            while(rs.next()) imena.add(rs.getString("Ime"));
        }
        catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return imena;
    }

    public static Kontakt dodajKontakt(Kontakt kontakt) {
        Connection c = connect();
        if(c==null) return null;


        String insert = "INSERT INTO kontakt(Ime, Adresa) VALUES (?, ?);";
        try(PreparedStatement ps = c.prepareStatement(insert)){
            ps.setString(1, kontakt.getIme());
            ps.setString(2, kontakt.getAdresa());
            int redovi = ps.executeUpdate();
            if(redovi>0){
                return kontakt;
            }
        }
        catch (SQLException e) {
            System.out.println(e.getMessage());
        }
        return null;
    }

    public static class Kontakt {
        private String ime;
        private String adresa;

        public String getIme() {
            return ime;
        }

        public String getAdresa() {
            return adresa;
        }

        public void setIme(String ime) {
            this.ime = ime;
        }

        public void setAdresa(String adresa) {
            this.adresa = adresa;
        }

        public Kontakt() {
            this.ime = "";
            this.adresa = "";
        }

        public Kontakt(String ime, String adresa) {
            this.ime = ime;
            this.adresa = adresa;
        }
    }
}
