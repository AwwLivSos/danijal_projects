import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

class AsistentTest {
    @Test
    void testKonstruktor() {
        Asistent asistent = new Asistent("Ime", "Prezime", "Adresa", new Date(99, 0, 1), 100, 2020, "2-01", "Lab1", "10:00-12:00");
        assertNotNull(asistent);  // Just checks the instance is created
    }
}
