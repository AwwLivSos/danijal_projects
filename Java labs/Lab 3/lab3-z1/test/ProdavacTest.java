import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

public class ProdavacTest {
    @Test
    public void testZdravlje() {
        Prodavac prodavac1 = new Prodavac("Ivan", "Ivić", 30, "AB12301");
        Prodavac prodavac2 = new Prodavac("Marko", "Marić", 40, "XY98765");
        assertTrue(prodavac1.zdravlje(0.0));
        assertFalse(prodavac2.zdravlje(0.0));
    }
}
