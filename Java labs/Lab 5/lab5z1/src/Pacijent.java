import java.util.*;

public class Pacijent implements  IPlate{
    private String ime_prezime, dijagnoza;
    private Date datum;
    private Double pare;
    private Boolean penzioner;

    public Pacijent(String name, String diagnosis, Date datum, Double pare, Boolean nesto) {
        this.ime_prezime = name;
        this.dijagnoza = diagnosis;
        this.datum = datum;
        this.pare = pare;
        this.penzioner = nesto;
    }

    public Double getUkupnaPrimanja() {
        return pare;
    }

    public String getImeIPrezime(){
        return ime_prezime;
    }

    public Double DajPlatu() {
        return (penzioner) ? 0 : pare;
    }
}
