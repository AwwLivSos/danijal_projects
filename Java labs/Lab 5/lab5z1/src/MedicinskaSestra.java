import  java.util.*;

public class MedicinskaSestra extends MedicinskoOsoblje implements IPlate{
    private Integer broj_sati;
    private int osnova = 20;

    public MedicinskaSestra(String imeIPrezime, Date datumZaposlenja, Zaposlenje vrstaZaposlenja, Integer sati) throws ValidacijaImenaException, ValidacijaDatumaException {
        super(imeIPrezime, datumZaposlenja, vrstaZaposlenja);
        this.broj_sati = sati;
    }

    public Integer getBrojNormaSati() {
        return broj_sati;
    }

    public int getOsnovica() {
        return osnova;
    }

    @Override
    public Double DajPlatu() {
        return (double)broj_sati*osnova;
    }
}