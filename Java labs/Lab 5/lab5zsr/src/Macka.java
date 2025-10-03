import Enumi.VrstaMacke;

import java.util.Date;

class Macka extends Ljubimac {
    private VrstaMacke vrsta;

    public Macka(String ime, Date datumRodjenja, String bolest, VrstaMacke vrsta) {
        super(ime, datumRodjenja, bolest);
        this.vrsta = vrsta;
    }

    @Override
    public String PrikaziInformacije() {
        return "Mačka: " + vrsta;
    }
}
