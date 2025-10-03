package Model;

import java.sql.Connection;
import java.sql.DriverManager;

public class Database {
    private static final String DB_URL = "jdbc:sqlite:baza.db";
    public static Connection connect() {
        Connection conn = null;
        try {
            conn = DriverManager.getConnection(DB_URL);
            System.out.println("Povezano s bazom podataka!");
        } catch (Exception e) {
            System.out.println(e.getMessage());
        }
        return conn;
    }
}