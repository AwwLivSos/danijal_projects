import org.junit.jupiter.api.Test;
import java.util.Arrays;
import static org.junit.jupiter.api.Assertions.*;

public class MesoTest {
    @Test
    public void testDajBrojKalorija() {
        Meso meso = new Meso("BiH", Arrays.asList(50, 100, 150), VrsteMesa.piletina);
        assertEquals(360.0, meso.DajBrojKalorija());
    }

    @Test
    public void testZdravlje() {
        Meso meso = new Meso("BiH", Arrays.asList(50, 100, 150), VrsteMesa.teletina);
        assertTrue(meso.zdravlje(0.96));
        assertFalse(meso.zdravlje(0.5));
    }
}
