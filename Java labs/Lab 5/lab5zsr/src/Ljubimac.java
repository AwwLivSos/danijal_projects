import java.util.Date;

abstract class Ljubimac implements Objekat {
    protected String ime;
    protected Date datumRodjenja;
    protected String bolest;

    public Ljubimac(String ime, Date datumRodjenja, String bolest) {
        this.ime = ime;
        this.datumRodjenja = datumRodjenja;
        this.bolest = bolest;
    }
}
