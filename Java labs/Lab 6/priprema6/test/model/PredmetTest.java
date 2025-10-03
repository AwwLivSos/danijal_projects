package model;

import model.Predmet;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class PredmetTest {
    @Test
    void testIspravnoKreiranPredmet() {
        Predmet predmet = new Predmet("Programiranje", 7.5);
        assertNotNull(predmet);
    }

    @Test
    void testNeispravanNaziv() {
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Predmet("abc", 7.5));
        assertEquals("Naziv mora imati između 5 i 100 znakova.", exception.getMessage());
    }

    @Test
    void testNeispravanECTS() {
        Exception exception = assertThrows(IllegalArgumentException.class,
                () -> new Predmet("Programiranje", 7.3));
        assertEquals("ECTS mora biti između 5.0 i 20.0 i može imati samo 0 ili 5 kao prvu decimalu.",
                exception.getMessage());
    }
}
