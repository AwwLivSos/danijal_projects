import java.util.Date;

public class  Student extends Osoba implements  IInformacije{
    private String brojIndeksa;
    private int godinaStudija;
    private double prosjek;

    public Student(String ime, String prezime, String adresa, Date datumRodjenja,  String brojIndeksa, int godinaStudija, double prosjek) {
        super(ime, prezime, adresa, datumRodjenja);
        //trebamo dodati super konstruktor koji konstruise za roditelj klasu
        //poslije toga mozemo definisati ostale atribute
        this.brojIndeksa = brojIndeksa;
        this.godinaStudija = godinaStudija;
        this.prosjek = prosjek;
        //naravno bolje je koristiti settere ako sam ih definisao
    }

    @Override
    public String DajInformacije() {
        return "Student: " + ime + " " + prezime + ", broj indeksa: " +brojIndeksa;
    }
}

