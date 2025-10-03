import java.util.*;

public class MedicinskoOsoblje {
    protected String imeIPrezime;
    protected Date datumZaposlenja;
    protected Zaposlenje vrstaZaposlenja;

    public void setImeIPrezime(String imeIPrezime) throws ValidacijaImenaException {
        if (imeIPrezime != null && imeIPrezime.length() > 0)        //zasto je potrebna dupla provjera
            this.imeIPrezime = imeIPrezime;
        else throw new ValidacijaImenaException("Neispravno uneseno ime i prezime!");
    }

    public void setDatumZaposlenja(Date datumZaposlenja) throws ValidacijaDatumaException {
        if (datumZaposlenja.before(new Date()))
            this.datumZaposlenja = datumZaposlenja;
        else throw new ValidacijaDatumaException("Neispravno unesen datum zaposlenja!");
    }

    public void setVrstaZaposlenja(Zaposlenje vrstaZaposlenja) {
        this.vrstaZaposlenja = vrstaZaposlenja; }

    public MedicinskoOsoblje (String imeIPrezime, Date datumZaposlenja, Zaposlenje vrstaZaposlenja) throws ValidacijaImenaException, ValidacijaDatumaException {
        setImeIPrezime(imeIPrezime);
        setDatumZaposlenja(datumZaposlenja);
        setVrstaZaposlenja(vrstaZaposlenja);
    }
}
