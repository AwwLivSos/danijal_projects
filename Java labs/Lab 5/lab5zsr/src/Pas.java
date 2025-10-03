import Enumi.VrstaPsa;

import java.util.Date;

class Pas extends Ljubimac {
    private VrstaPsa vrsta;

    public Pas(String ime, Date datumRodjenja, String bolest, VrstaPsa vrsta) {
        super(ime, datumRodjenja, bolest);
        this.vrsta = vrsta;
    }

    @Override
    public String PrikaziInformacije() {
        return "Pas: " + vrsta;
    }
}
