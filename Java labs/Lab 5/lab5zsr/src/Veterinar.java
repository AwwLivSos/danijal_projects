import Enumi.Specijalizacija;

import java.util.ArrayList;
import java.util.List;

class Veterinar implements Objekat {
    private String ime;
    private Specijalizacija specijalizacija;
    private List<Ljubimac> pregledi;

    public Veterinar(String ime, Specijalizacija specijalizacija) {
        this.ime = ime;
        this.specijalizacija = specijalizacija;
        this.pregledi = new ArrayList<>();
    }

    public void PregledajLjubimca(Ljubimac ljubimac) throws ValidacijaVrsteException {
        if ((specijalizacija == Specijalizacija.Psi && ljubimac instanceof Macka) || (specijalizacija == Specijalizacija.Macke && ljubimac instanceof Pas)) {
            throw new ValidacijaVrsteException("Veterinar nije specijaliziran za ovu vrstu.");
        }
        pregledi.add(ljubimac);
    }

    public List<Ljubimac> getPregledi() {
        return pregledi;
    }

    @Override
    public String PrikaziInformacije() {
        return "Veterinar: " + ime;
    }
}
