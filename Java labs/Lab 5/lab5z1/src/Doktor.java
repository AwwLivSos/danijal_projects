import java.util.*;

public class Doktor extends MedicinskoOsoblje implements IPlate{
    private Double coeff;
    private List<Pacijent> pacijenti;

    public Doktor(String imeIPrezime, Date datumZaposlenja, Zaposlenje vrstaZaposlenja, Double broj) throws ValidacijaImenaException, ValidacijaDatumaException {
        super(imeIPrezime, datumZaposlenja, vrstaZaposlenja);
        this.coeff = broj;
    }

    public void RadSaPacijentima(VrstaOperacije vrsta, Pacijent p, String ime){
        if(vrsta == VrstaOperacije.Dodavanje) pacijenti.add(p);
        else {
            int idx= -1;
            for(int i=0; i<pacijenti.size(); i++){
                if(pacijenti.get(i).getImeIPrezime().equals(ime)) {
                    if (vrsta == VrstaOperacije.Izmjena)
                        pacijenti.set(i, p);
                    else idx = i;
                }
            }
            if(vrsta == VrstaOperacije.Brisanje) pacijenti.remove(idx);
        }
    }

    public Double getKoeficijentPlate() {
        return coeff;
    }

    public List<Pacijent> getPacijenti() {
        return pacijenti;
    }

    @Override
    public Double DajPlatu() {
        return coeff*385;
    }
}
