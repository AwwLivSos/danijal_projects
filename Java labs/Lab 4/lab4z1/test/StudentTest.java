import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import java.util.Date;

class StudentTest {
    @Test
    void testDajInformacije() {
        Student student = new Student("Ime", "Prezime", "Adresa", new Date(99, 0, 1), "IB12345", 3, 8.5);
        assertEquals("Student: Ime Prezime, broj indeksa: IB12345", student.DajInformacije());
    }
}
