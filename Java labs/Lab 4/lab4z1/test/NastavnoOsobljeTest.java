import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

class NastavnoOsobljeTest {
    private static class NastavnoOsobljeConcrete extends NastavnoOsoblje {
        public NastavnoOsobljeConcrete(String ime, String prezime, String adresa, Date datumRodjenja, Integer norma, Integer godinaZaposlenja, String kancelarija) {
            super(ime, prezime, adresa, datumRodjenja, norma, godinaZaposlenja, kancelarija);
        }
    }

    @Test
    void testConstructor() {
        NastavnoOsoblje osoblje = new NastavnoOsobljeConcrete("Ime", "Prezime", "Adresa", new Date(99, 0, 1), 100, 2010, "1-23");
        assertNotNull(osoblje);  // Checks instance creation
    }
}
