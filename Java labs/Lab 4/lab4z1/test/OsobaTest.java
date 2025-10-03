import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

class OsobaTest {
    private static class OsobaConcrete extends Osoba {
        public OsobaConcrete(String ime, String prezime, String adresa, Date datumRodjenja) {
            super(ime, prezime, adresa, datumRodjenja);
        }
    }

    @Test
    void testProvjeriMaticniBroj() {
        Date datum = new Date(99, 0, 1);
        Osoba osoba = new OsobaConcrete("Test", "Test", "Test Address", datum);
        assertTrue(osoba.ProvjeriMaticniBroj("0101999123456"));
        assertFalse(osoba.ProvjeriMaticniBroj("3112991123456"));
    }

    @Test
    void testDajInformacije() {
        Osoba osoba = new OsobaConcrete("Ime", "Prezime", "Adresa", new Date(99, 0, 1));
        assertEquals("Ime i prezime: Ime Prezime", osoba.DajInformacije());
    }
}
