import java.util.Date;
import java.util.Scanner;
import java.util.List;

public abstract class  Osoba{
    protected String ime, prezime, adresa;
    protected Date datumRodjenja;
    //atributi

    public String getIme() {
        return ime;
    }

    public void setIme(String ime) {
        this.ime = ime;
    }

    public String getPrezime() {
        return prezime;
    }

    public void setPrezime(String prezime) {
        this.prezime = prezime;
    }

    public String getAdresa() {
        return adresa;
    }

    public void setAdresa(String adresa) {
        this.adresa = adresa;
    }

    public Date getDatumRodjenja() {
        return datumRodjenja;
    }

    public void setDatumRodjenja(Date datumRodjenja) {
        this.datumRodjenja = datumRodjenja;
    }

    public Osoba(String ime, String prezime, String adresa, Date datumRodjenja) {
        this.ime = ime;
        this.prezime = prezime;
        this.adresa = adresa;                   //konstruktor
        this.datumRodjenja = datumRodjenja;
    }

    public boolean ProvjeriMaticniBroj(String maticniBroj) {
        boolean danIsti = datumRodjenja.getDate() == Integer.parseInt(maticniBroj.substring(0, 2));
        boolean mjesecIsti = datumRodjenja.getMonth() + 1 == Integer.parseInt(maticniBroj.substring(2, 4));
        boolean godinaIsta = datumRodjenja.getYear() + 900 == Integer.parseInt(maticniBroj.substring(4, 7));
        return (danIsti && mjesecIsti && godinaIsta);
    }

    public String DajInformacije(){
        return "Ime i prezime: " + ime + " " + prezime;
    }
}

