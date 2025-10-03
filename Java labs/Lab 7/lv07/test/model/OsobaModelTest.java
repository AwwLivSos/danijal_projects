package model;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class OsobaModelTest {
    @Test
    public void testNapuni() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        assertEquals("Neko", model.dajSviju().get(0).getIme());
    }

    @Test
    public void testAzurirajOsobu() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        String res = model.azurirajOsoabu(1, "NovoIme", null, null, null, null, null);
        assertEquals("Osoba je uspjesno azurirana!", res);
        assertEquals("NovoIme", model.dajOsobuPoID(1).getIme());
    }

    @Test
    public void testAzurirajOsobu_nema() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        String res = model.azurirajOsoabu(10, "NovoIme", null, null, null, null, null);
        assertEquals("Osoba nije nadjena!", res);
    }

    @Test
    public void testAzurirajOsobu_malo() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        String res = model.azurirajOsoabu(1, "NekoIme", "NovoPrezime", null, null, null, null);
        assertEquals("Osoba je uspjesno azurirana!", res);
        assertEquals("NovoPrezime", model.dajOsobuPoID(1).getPrezime());
    }

    @Test
    public void testObrisiOsobu() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        String result = model.obrisiOsobu(1);
        assertEquals("Osoba je uspjesno obrisana!", result);
        assertNull(model.dajOsobuPoID(1));
    }

    @Test
    public void testObrisiOsobu_nema() {
        OsobaModel model = new OsobaModel();
        model.napuni();

        String result = model.obrisiOsobu(10);
        assertEquals("Osoba nije pronadjena!", result);
    }

}