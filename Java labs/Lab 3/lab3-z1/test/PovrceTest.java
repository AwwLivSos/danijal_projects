import org.junit.jupiter.api.Test;
import java.util.Arrays;
import static org.junit.jupiter.api.Assertions.*;

public class PovrceTest {
    @Test
    public void testZdravlje() {
        Povrce povrce = new Povrce("Italija", Arrays.asList(20, 30), "Solanum", "Italija");
        assertTrue(povrce.zdravlje(0.6));
        assertFalse(povrce.zdravlje(0.8)); // koeficijent izvan granica
        assertFalse(povrce.zdravlje(0.4)); // koeficijent ispod granica
    }
}
