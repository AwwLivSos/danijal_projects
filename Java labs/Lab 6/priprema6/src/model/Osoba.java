package model;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class Osoba {
    private Integer id;
    private String ime, prezime, adresa;
    private Date datumRodjenja;
    private String maticniBroj;
    private Uloga uloga;

    public enum Uloga {
        STUDENT, NASTAVNO_OSOBLJE
    }

    public Osoba(Integer id, String ime, String prezime, String adresa, Date datumRodjenja, String maticniBroj, Uloga uloga) {
        setId(id);
        setIme(ime);
        setPrezime(prezime);
        setAdresa(adresa);
        setDatumRodjenja(datumRodjenja);
        setMaticniBroj(maticniBroj);
        setUloga(uloga);
    }

    public void setUloga(Uloga uloga) {
        this.uloga = uloga;
    }

    public void setId(Integer id) {
        if (id == null || id <= 0) {
            throw new IllegalArgumentException("ID mora biti pozitivan broj.");
        }
        this.id = id;
    }

    public void setIme(String ime) {
        if (ime == null || ime.length() < 2 || ime.length() > 50) {
            throw new IllegalArgumentException("Ime mora imati između 2 i 50 znakova.");
        }
        this.ime = ime;
    }

    public void setPrezime(String prezime) {
        if (prezime == null || prezime.isEmpty()) {
            throw new IllegalArgumentException("Prezime ne može biti prazno.");
        }
        this.prezime = prezime;
    }

    public void setAdresa(String adresa) {
        if (adresa == null || adresa.isEmpty()) {
            throw new IllegalArgumentException("Adresa ne može biti prazna.");
        }
        this.adresa = adresa;
    }

    public void setDatumRodjenja(Date datumRodjenja) {
        if (datumRodjenja == null) {
            throw new IllegalArgumentException("Datum rođenja ne može biti null.");
        }
        this.datumRodjenja = datumRodjenja;
    }

    public void setMaticniBroj(String maticniBroj) {
        if (maticniBroj == null || maticniBroj.trim().isEmpty() || maticniBroj.length() != 13) {
            throw new IllegalArgumentException("Matični broj mora imati tačno 13 karaktera!");
        }
        if (!ProvjeriMaticniBroj(maticniBroj)) {
            throw new IllegalArgumentException("Matični broj se ne poklapa sa datumom rođenja!");
        }
        this.maticniBroj = maticniBroj;
    }

    public boolean ProvjeriMaticniBroj(String maticniBroj) {
        if (datumRodjenja == null) {
            throw new IllegalStateException("Datum rođenja nije postavljen!");
        }

        SimpleDateFormat sdf = new SimpleDateFormat("ddMMyy");
        String formattedDatumRodjenja = sdf.format(datumRodjenja);
        return maticniBroj.substring(0, 6).equals(formattedDatumRodjenja);
    }

    public boolean mozeUcestvovatiUProjektu(boolean voditeljProjekta) {
        return this.uloga == Uloga.NASTAVNO_OSOBLJE || (!voditeljProjekta && this.uloga == Uloga.STUDENT);
    }

    public boolean imaPravoNaStipendiju() {
        return this.uloga == Uloga.STUDENT;
    }

    @Override
    public String toString() {
        return "Osoba{" +
                "id=" + id +
                ", ime='" + ime + '\'' +
                ", prezime='" + prezime + '\'' +
                ", adresa='" + adresa + '\'' +
                ", datumRodjenja=" + datumRodjenja +
                ", maticniBroj='" + maticniBroj + '\'' +
                ", uloga=" + uloga +
                '}';
    }

    public static List<Osoba> ucitajOsobeIzTxtDatoteke(String putanjaDoDatoteke) throws IOException, ParseException {
        List<Osoba> osobe = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(putanjaDoDatoteke));
        String line;

        while ((line = reader.readLine()) != null) {
            String[] fields = line.split(",");
            if (fields.length == 7) {
                Integer id = Integer.parseInt(fields[0]);
                String ime = fields[1];
                String prezime = fields[2];
                String adresa = fields[3];
                Date datumRodjenja = new SimpleDateFormat("yyyy-MM-dd").parse(fields[4]);
                String maticniBroj = fields[5];
                Uloga uloga = Uloga.valueOf(fields[6].toUpperCase());

                Osoba osoba = new Osoba(id, ime, prezime, adresa, datumRodjenja, maticniBroj, uloga);
                osobe.add(osoba);
            }
        }
        reader.close();
        return osobe;
    }
}
