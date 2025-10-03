public class Prodavac implements Zdravlje{
    private String ime;
    private String prezime;
    private int brojs;
    private String ID;

    public Prodavac(String ime, String prezime, int brojs, String ID) {
        this.ime = ime;
        this.prezime = prezime;
        this.brojs = brojs;
        this.ID = ID;
    }

    @Override
    public boolean zdravlje(double koeficijent) {
        return ID.endsWith("01"); //??? op funkcija
    }
}
