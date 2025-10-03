import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import java.util.Arrays;
import static org.junit.jupiter.api.Assertions.*;

public class NamirniceTest {
    private Namirnice namirnica;

    @BeforeEach
    public void setUp() {
        namirnica = new Namirnice("Hrvatska", Arrays.asList(10, 20, 30)) {
            @Override
            public boolean zdravlje(double koeficijent) {
                return koeficijent > 0.5;
            }
        };
    }

    @Test
    public void testDajBrojKalorija() {
        assertEquals(60.0, namirnica.DajBrojKalorija());
    }

    @Test
    public void testGetSetPorijeklo() {
        namirnica.setPorijeklo("Srbija");
        assertEquals("Srbija", namirnica.getPorijeklo());
    }

    @Test
    public void testGetSetPodaci() {
        namirnica.setPodaci(Arrays.asList(5, 15));
        assertEquals(Arrays.asList(5, 15), namirnica.getPodaci());
    }
}
