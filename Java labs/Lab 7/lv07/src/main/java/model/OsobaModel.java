package model;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import java.util.*;

public class OsobaModel {
    private ObservableList<Osoba> osobe;            //sad je ovo kao kontejnerksa kolekcija za klasu

    public OsobaModel() {
        osobe = FXCollections.observableArrayList();
    }

    public String dodajOsobu(Integer id, String ime, String prezime, String adresa, Date datumRodjenja, String maticniBroj, Uloga uloga){
        try{
            Osoba newOsoba = new Osoba(id, ime, prezime, adresa, datumRodjenja, maticniBroj, uloga);
            osobe.add(newOsoba);
            return "Osoba je uspjesno dodana!";
        }
        catch(IllegalArgumentException e){
            return e.getMessage();              //uhvacen izuzetak i vracena njegova poruka kao string
        }
    }

    public ObservableList<Osoba> dajSviju() {
        return osobe;
    }

    public Osoba dajOsobuPoID(Integer id){
        for(Osoba osoba : osobe){
            if(osoba.getId() == id){
                return osoba;
            }
        }
        return null;
    }

    public String azurirajOsoabu(Integer id, String novoIme, String novoPrezime, String novaAdresa, Date noviDatum, String noviMaticniBroj, Uloga novaUloga){
        Osoba trazena = dajOsobuPoID(id);
        if(trazena != null){
            try{
                if (novoIme != null) {
                    trazena.setIme(novoIme);
                }
                if (novoPrezime != null) {
                    trazena.setPrezime(novoPrezime);
                }
                if (novaAdresa != null) {
                    trazena.setAdresa(novaAdresa);
                }
                if (noviDatum != null) {
                    trazena.setDatumRodjenja(noviDatum);
                }
                if (noviMaticniBroj != null) {
                    trazena.setMaticniBroj(noviMaticniBroj);
                }
                if (novaUloga != null){
                    trazena.setUloga(novaUloga);
                }
                return "Osoba je uspjesno azurirana!";
            }
            catch(IllegalArgumentException e){
                return e.getMessage();
            }
        }
        return "Osoba nije nadjena!";
    }

    public String obrisiOsobu(Integer id){
        if (osobe.removeIf(osoba -> osoba.getId() == id)) {
            return "Osoba je uspjesno obrisana!";
        } else {
            return "Osoba nije pronadjena!";
        }
    }

    public void napuni(){
        osobe.add(new Osoba(1,"Neko","Nekic","Neka adresa", new Date(97,8,25), "2509997123456", Uloga.STUDENT));
        osobe.add(new Osoba(2,"Neko 2","Nekic 2","Neka adresa 2",new Date(97,8,25), "2509997123456", Uloga.NASTAVNO_OSOBLJE));
    }
}
