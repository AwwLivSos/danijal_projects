import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class MainTest {
    static Pacijent p1, p2;
    static Doktor d;
    static  MedicinskaSestra ms;

    @BeforeAll
    public static void Inicijalizacija(){          //zasto ovo nije throws exception?
        try{
            p1 = new Pacijent("Emin Begic", "AIDS", new Date(90,1,1), 10000.0, false);
            p2 = new Pacijent("Mirnes Fehric", "Cancer", new Date(91,5,6), 5000.0, true);
            d = new Doktor("Danijal Alibegovic", new Date(103, 14, 9), Zaposlenje.TrajnoZaposlen,10.85);
            ms = new MedicinskaSestra("Ajdin Dželo", new Date(104, 11, 22), Zaposlenje.TrajnoZaposlen,11);
        }
        catch(Exception e){
            fail();        //ili assertFalse(true), sto je realno niđe veze
        }
    }

    @Test
    public void TestValidacija() {
        assertThrows(ValidacijaImenaException.class, () ->          //ovo su zapravo lambda funkcije??
        {
            MedicinskoOsoblje doktor = new Doktor(null, new Date(103, 14, 9), Zaposlenje.TrajnoZaposlen,10.85);
        });
        assertThrows(ValidacijaImenaException.class, () ->
        {
            MedicinskoOsoblje nurse = new MedicinskaSestra(null, new Date(104, 11, 22), Zaposlenje.TrajnoZaposlen, 11);
        });
        assertThrows(ValidacijaDatumaException.class, () ->
        {
            Doktor doktor = new Doktor(null, new Date(103, 14, 9), Zaposlenje.TrajnoZaposlen, 10.85);
        });
    }

    @Test
    public void TestPacijent() {
        d.RadSaPacijentima(VrstaOperacije.Dodavanje, p1, "");
        assertEquals(d.getPacijenti().size(), 1);

        d.RadSaPacijentima(VrstaOperacije.Izmjena, p2, p1.getImeIPrezime());
        assertTrue(d.getPacijenti().get(0).getImeIPrezime().equals(p2.getImeIPrezime()));

        d.RadSaPacijentima(VrstaOperacije.Brisanje, null, p2.getImeIPrezime());
        assertFalse(d.getPacijenti().size() > 0);
    }

    @Test
    public void TestPlate() {
        List<IPlate> osobe = new ArrayList<IPlate>();
        osobe.add(p1);
        osobe.add(p2);
        osobe.add(d);
        osobe.add(ms);
        Double ukupniRezultat = 0.0, ocekivaniRezultat = 0.0;

        for(IPlate osoba : osobe) ukupniRezultat+=osoba.DajPlatu();

        ocekivaniRezultat = p1.getUkupnaPrimanja() + 0.0 + d.getKoeficijentPlate() * 385 + ms.getOsnovica() * ms.getBrojNormaSati();

        assertEquals(ukupniRezultat, ocekivaniRezultat);
    }

    @Test public void TestPlateDoktora () {             //ma ok je ugl, ne prolazi sve kjer nisu mi tacne vrijednosti nekih atributa
        Double plata = d.getKoeficijentPlate() * 385;
        assertEquals(d.DajPlatu(), plata);

        d.setVrstaZaposlenja(Zaposlenje.Zaposlen20Posto);
        assertEquals(d.DajPlatu(), plata * 0.2);

        d.setVrstaZaposlenja(Zaposlenje.Zaposlen50Posto);
        assertEquals(d.DajPlatu(), plata * 0.5);
    }
}