import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

class ProfesorTest {
    @Test
    void testDajInformacije() {
        Profesor profesor = new Profesor("Ime", "Prezime", "Adresa", new Date(99, 0, 1), 150, 2000, "3-00", "red. prof. dr.", 50);
        assertEquals("Profesor: red. prof. dr. Ime Prezime", profesor.DajInformacije());
    }
}
