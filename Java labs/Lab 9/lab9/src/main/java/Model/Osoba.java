package Model;

import javafx.beans.property.*;

import java.util.Date;

public class Osoba {
    private IntegerProperty id;
    private StringProperty ime, prezime, adresa, maticniBroj;
    private ObjectProperty<Date> datumRodjenja;
    private ObjectProperty<Uloga> uloga;

    public Osoba(Integer id, String ime, String prezime, String adresa, Date datumRodjenja, String maticniBroj, Uloga uloga) {
        this.id = new SimpleIntegerProperty(id);
        this.ime = new SimpleStringProperty(ime);                   //preko ove sintakse se konstruise konstruktor
        this.prezime = new SimpleStringProperty();
        this.adresa = new SimpleStringProperty(adresa);
        this.datumRodjenja = new SimpleObjectProperty<>(datumRodjenja);
        this.maticniBroj = new SimpleStringProperty();
        this.uloga = new SimpleObjectProperty<>(uloga);

        setIme(ime);                    //za ova dva nismo odmah inicijalizirali preko this.prezime= jer zelimo provjeriti validnost prvo, ali moramo prvo stvaoriti taj objekat
        setMaticniBroj(maticniBroj);
    }

    public int getId() {
        return id.get();
    }

    public IntegerProperty idProperty() {
        return id;
    }

    public void setId(int id) {
        this.id.set(id);
    }

    public String getPrezime() {
        return prezime.get();
    }

    public StringProperty prezimeProperty() {
        return prezime;
    }

    public void setPrezime(String prezime) {
        if(prezime == null || prezime.isEmpty() || prezime.length() > 50)
            throw new IllegalArgumentException("Neispravno ime!");
        this.prezime.set(prezime);
    }

    public String getAdresa() {
        return adresa.get();
    }

    public StringProperty adresaProperty() {
        return adresa;
    }

    public void setAdresa(String adresa) {
        this.adresa.set(adresa);
    }

    public String getMaticniBroj() {
        return maticniBroj.get();
    }

    public StringProperty maticniBrojProperty() {
        return maticniBroj;
    }

    public void setMaticniBroj(String maticniBroj) {
        if(maticniBroj == null || maticniBroj.length() != 13)
            throw new IllegalArgumentException("Neispravan maticni broj!");
        this.maticniBroj.set(maticniBroj);
    }

    public Date getDatumRodjenja() {
        return datumRodjenja.get();
    }

    public ObjectProperty<Date> datumRodjenjaProperty() {
        return datumRodjenja;
    }

    public void setDatumRodjenja(Date datumRodjenja) {
        this.datumRodjenja.set(datumRodjenja);
    }

    public Uloga getUloga() {
        return uloga.get();
    }

    public ObjectProperty<Uloga> ulogaProperty() {
        return uloga;
    }

    public void setUloga(Uloga uloga) {
        this.uloga.set(uloga);
    }

    public String getIme(){
        return ime.get();           //na ovaj fazon se valjda mora vracati vrijednost imena
    }//ova metoda zapravo vraca sami string, koji se nalazi unutar klase StringProperty, koja ima svoj getter

    public StringProperty imeProperty(){
        return ime;                 //al onda sad ima i ovaj fazon koji je plaho slican al nije isti
    }//ova metoda vraca StringProperty kao takav, pa se moze vratiti samo "ime"

    public void setIme(String ime){
        this.ime.set(ime);          //i onda se ovako setta nesto
    }//opet, klasa StringProperty ima svoj setter
}
