package model;

import model.Osoba.Uloga;
import model.Osoba;
import org.junit.jupiter.api.Test;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import static org.junit.jupiter.api.Assertions.*;

class OsobaTest {
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");

    @Test
    void testIspravnoKreiranaOsoba() throws ParseException {
        Date datumRodjenja = dateFormat.parse("1995-08-25");
        Osoba osoba = new Osoba(1, "Dani", "Alibegović", "Adresa", datumRodjenja, "2508951234567", Uloga.STUDENT);
        assertNotNull(osoba);
    }

    @Test
    void testImePrekratko() {
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Osoba(1, "A", "Prezime", "Adresa", new Date(), "1234567890123", Uloga.STUDENT));
        assertEquals("Ime mora imati između 2 i 50 znakova.", exception.getMessage());
    }

    @Test
    void testImePredugo() {
        String longName = "A".repeat(51);
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Osoba(1, longName, "Prezime", "Adresa", new Date(), "1234567890123", Uloga.STUDENT));
        assertEquals("Ime mora imati između 2 i 50 znakova.", exception.getMessage());
    }

    @Test
    void testMaticniBrojNeispravnaDuzina() {
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Osoba(1, "Dani", "Prezime", "Adresa", new Date(), "1234567", Uloga.STUDENT));
        assertEquals("Matični broj mora imati tačno 13 karaktera!", exception.getMessage());
    }

    @Test
    void testMaticniBrojNePodudaraDatum() throws ParseException {
        Date datumRodjenja = dateFormat.parse("1995-08-25");
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Osoba(1, "Dani", "Prezime", "Adresa", datumRodjenja, "1508951234567", Uloga.STUDENT));
        assertEquals("Matični broj se ne poklapa sa datumom rođenja!", exception.getMessage());
    }

    @Test
    void testInvalidFilePath() {
        Exception exception = assertThrows(Exception.class, () -> Osoba.ucitajOsobeIzTxtDatoteke("nepostojeca_putanja.txt"));
        assertFalse(exception.getMessage().contains("Greška"));
    }
}
