import org.junit.jupiter.api.Test;
import java.util.Arrays;
import static org.junit.jupiter.api.Assertions.*;

public class VoceTest {
    @Test
    public void testZdravlje() {
        Voce voce = new Voce("Španija", Arrays.asList(10, 20), "Citrus", "Španija");
        assertTrue(voce.zdravlje(0.8));
        assertFalse(voce.zdravlje(0.7)); // koeficijent premali
    }
}
